/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef DLL_EXPORT
#  define USE_STATIC_LIB
#endif

#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/ptr_container/ptr_list.hpp>
#include <boost/thread/locks.hpp>
#include <boost/foreach.hpp>
#include <boost/random.hpp>
#include <boost/random/mersenne_twister.hpp> // mt19937
#include <boost/random/normal_distribution.hpp>
#include <string>
#include <zookeeper.h>
#include <memory_in_stream.hh>
#include <string_out_stream.hh>
#include <recordio.hh>
#include <binarchive.hh>
#include "zk_adaptor.h"
#include "zookeeper/zookeeper.hh"
#include "zookeeper/logging.hh"
ENABLE_LOGGING;
#include "path_utils.hh"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <assert.h>
#include <stdarg.h>
#include <limits.h>

#include <sys/time.h>
#include <sys/socket.h>
#include <poll.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include "config.h"

#ifdef HAVE_SYS_UTSNAME_H
#include <sys/utsname.h>
#endif

#ifdef HAVE_GETPWUID_R
#include <pwd.h>
#endif

using namespace org::apache::zookeeper;
completion_list_t zhandle_t::completionOfDeath;
const int ZOOKEEPER_WRITE = 1 << 0;
const int ZOOKEEPER_READ = 1 << 1;

#define COMPLETION_WATCH -1
#define COMPLETION_VOID 0
#define COMPLETION_STAT 1
#define COMPLETION_DATA 2
#define COMPLETION_STRINGLIST 3
#define COMPLETION_STRINGLIST_STAT 4
#define COMPLETION_ACLLIST 5
#define COMPLETION_STRING 6
#define COMPLETION_MULTI 7

const char*err2string(int err);
static int queue_session_event(zhandle_t *zh, SessionState::type state);
static const char* format_endpoint_info(const struct sockaddr_storage* ep);
static const char* format_current_endpoint_info(zhandle_t* zh);

/* deserialize forward declarations */
static void deserialize_response(int type, int xid, ReturnCode::type rc,
    completion_list_t *cptr, hadoop::IBinArchive& iarchive,
     const std::string& chroot);
static int deserialize_multi(int xid, completion_list_t *cptr,
                             hadoop::IBinArchive& iarchive,
                             boost::ptr_vector<OpResult>& results);

/* completion routine forward declarations */
static int add_completion(zhandle_t *zh, int xid, int completion_type,
        const void *dc, const void *data, WatchRegistration* wo,
        boost::ptr_vector<OpResult>* results, bool isSynchronous);
static completion_list_t* create_completion_entry(int xid, int completion_type,
        const void *dc, const void *data, WatchRegistration* wo,
        boost::ptr_vector<OpResult>* results, bool isSynchronous);
static void destroy_completion_entry(completion_list_t* c);
static void queue_completion(completion_head_t *list, completion_list_t *c);
static ReturnCode::type handle_socket_error_msg(zhandle_t *zh, int line, ReturnCode::type rc,
                                  const std::string& message);
static void cleanup_bufs(zhandle_t *zh, int rc);

static int disable_conn_permute=0; // permute enabled by default

static void deliverWatchers(zhandle_t *zh, int type, int state, const char *path,
                     std::list<boost::shared_ptr<Watch> >& watches) {
  // session event's don't have paths
  std::string client_path =
    type == WatchEvent::SessionStateChanged ? path :
                                              PathUtils::stripChroot(path, zh->chroot);
  BOOST_FOREACH(boost::shared_ptr<Watch>& watch, watches) {
    LOG_DEBUG("Calling watch");
    watch->process((WatchEvent::type)type,
        (SessionState::type)state, client_path);
  }
}

static ssize_t zookeeper_send(int s, const void* buf, size_t len)
{
#ifdef __linux__
  return send(s, buf, len, MSG_NOSIGNAL);
#else
  return send(s, buf, len, 0);
#endif
}

int zoo_recv_timeout(zhandle_t *zh)
{
    return zh->recv_timeout;
}

bool is_unrecoverable(zhandle_t *zh) {
  return zh->state < 0;
}

zhandle_t::
~zhandle_t() {
    /* call any outstanding completions with a special error code */
    cleanup_bufs(this, ReturnCode::InvalidState);
    if (input_buffer != NULL) {
      delete input_buffer;
      input_buffer = 0;
    }

    if (hostname != 0) {
        free(hostname);
        hostname = NULL;
    }
    if (fd != -1) {
        close(fd);
        fd = -1;
        // TODO introduce closed state?
        state = (SessionState::type)0;
    }
    if (addrs != 0) {
        free(addrs);
        addrs = NULL;
    }
    close(threads.self_pipe[0]);
    close(threads.self_pipe[1]);
}

/**
 * get the errno from the return code of get addrinfo. Errno is not set with the
 * call to getaddrinfo, so thats why we have to do this.
 */
static int getaddrinfo_errno(int rc) {
    switch(rc) {
    case EAI_NONAME:
// ZOOKEEPER-1323 EAI_NODATA and EAI_ADDRFAMILY are deprecated in FreeBSD.
#if defined EAI_NODATA && EAI_NODATA != EAI_NONAME
    case EAI_NODATA:
#endif
        return ENOENT;
    case EAI_MEMORY:
        return ENOMEM;
    default:
        return EINVAL;
    }
}

/**
 * fill in the addrs array of the zookeeper servers in the zhandle. after filling
 * them in, we will permute them for load balancing.
 */
int getaddrs(zhandle_t *zh) {
  char *hosts = strdup(zh->hostname);
  char *host;
  char *strtok_last;
  struct sockaddr_storage *addr;
  int i;
  int rc;
  int alen = 0; /* the allocated length of the addrs array */

  zh->addrs_count = 0;
  if (zh->addrs) {
    free(zh->addrs);
    zh->addrs = 0;
  }
  if (!hosts) {
    LOG_ERROR("out of memory");
    errno=ENOMEM;
    return ReturnCode::SystemError;
  }
  zh->addrs = 0;
  host=strtok_r(hosts, ",", &strtok_last);
  while(host) {
    char *port_spec = strrchr(host, ':');
    char *end_port_spec;
    int port;
    if (!port_spec) {
      LOG_ERROR("no port in " << host);
      errno=EINVAL;
      rc=ReturnCode::BadArguments;
      goto fail;
    }
    *port_spec = '\0';
    port_spec++;
    port = strtol(port_spec, &end_port_spec, 0);
    if (!*port_spec || *end_port_spec || port == 0) {
      LOG_ERROR("invalid port in " << host);
      errno=EINVAL;
      rc=ReturnCode::BadArguments;
      goto fail;
    }
    {
      struct addrinfo hints, *res, *res0;

      memset(&hints, 0, sizeof(hints));
#ifdef AI_ADDRCONFIG
      hints.ai_flags = AI_ADDRCONFIG;
#else
      hints.ai_flags = 0;
#endif
      hints.ai_family = AF_UNSPEC;
      hints.ai_socktype = SOCK_STREAM;
      hints.ai_protocol = IPPROTO_TCP;

      while(isspace(*host) && host != strtok_last)
        host++;

      if ((rc = getaddrinfo(host, port_spec, &hints, &res0)) != 0) {
        //bug in getaddrinfo implementation when it returns
        //EAI_BADFLAGS or EAI_ADDRFAMILY with AF_UNSPEC and 
        // ai_flags as AI_ADDRCONFIG
#ifdef AI_ADDRCONFIG
        if ((hints.ai_flags == AI_ADDRCONFIG) && 
            // ZOOKEEPER-1323 EAI_NODATA and EAI_ADDRFAMILY are deprecated in FreeBSD.
#ifdef EAI_ADDRFAMILY
            ((rc ==EAI_BADFLAGS) || (rc == EAI_ADDRFAMILY))) {
#else
          (rc == EAI_BADFLAGS)) {
#endif
            //reset ai_flags to null
            hints.ai_flags = 0;
            //retry getaddrinfo
            rc = getaddrinfo(host, port_spec, &hints, &res0);
          }
          if (rc != 0) {
            errno = getaddrinfo_errno(rc);
#if __linux__ && __GNUC__
            LOG_ERROR("getaddrinfo: " << gai_strerror(rc));
#else
            LOG_ERROR("getaddrinfo: " << strerror(errno));
#endif
            rc=ReturnCode::SystemError;
            goto fail;
          }
        }

        for (res = res0; res; res = res->ai_next) {
          // Expand address list if needed
          if (zh->addrs_count == alen) {
            void *tmpaddr;
            alen += 16;
            tmpaddr = realloc(zh->addrs, sizeof(*zh->addrs)*alen);
            if (tmpaddr == 0) {
              LOG_ERROR("out of memory");
              errno=ENOMEM;
              rc=ReturnCode::SystemError;
              freeaddrinfo(res0);
              goto fail;
            }
            zh->addrs=(sockaddr_storage*)tmpaddr;
          }

          // Copy addrinfo into address list
          addr = &zh->addrs[zh->addrs_count];
          switch (res->ai_family) {
            case AF_INET:
#if defined(AF_INET6)
            case AF_INET6:
#endif
              memcpy(addr, res->ai_addr, res->ai_addrlen);
              ++zh->addrs_count;
              break;
            default:
              LOG_WARN(
                  boost::format("skipping unknown address family %x for %s") %
                  res->ai_family % zh->hostname);
              break;
          }
        }

        freeaddrinfo(res0);

        host = strtok_r(0, ",", &strtok_last);
      }
#endif
    }
    free(hosts);

    if(!disable_conn_permute){
      srandom(time(NULL));
      /* Permute */
      for (i = zh->addrs_count - 1; i > 0; --i) {
        long int j = random()%(i+1);
        if (i != j) {
          struct sockaddr_storage t = zh->addrs[i];
          zh->addrs[i] = zh->addrs[j];
          zh->addrs[j] = t;
        }
      }
    }
    return ReturnCode::Ok;
fail:
    if (zh->addrs) {
      free(zh->addrs);
      zh->addrs=0;
    }
    if (hosts) {
      free(hosts);
    }
    return rc;
  }

struct sockaddr* zookeeper_get_connected_host(zhandle_t *zh,
                 struct sockaddr *addr, socklen_t *addr_len)
{
    if (zh->state!=SessionState::Connected) {
        return NULL;
    }
    if (getpeername(zh->fd, addr, addr_len)==-1) {
        return NULL;
    }
    return addr;
}

// TODO Fix these macros
static void log_env() {
  LOG_INFO("Initializing a ZooKeeper client: version=" << PACKAGE_STRING);
}

/**
 * Create a zookeeper handle associated with the given host and port.
 */
zhandle_t *zookeeper_init(const char *host, boost::shared_ptr<Watch> watch,
  int recv_timeout, int flags)
{
    int errnosave = 0;
    zhandle_t *zh = NULL;
    char *index_chroot = NULL;

    log_env();
    LOG_INFO(
      boost::format("Initiating client connection, host=%s sessionTimeout=%d "
                    "watcher=%p flags=%d") %
                    host % recv_timeout % watch % flags);

    zh = new zhandle_t();
    zh->sent_requests.lock.reset(new boost::mutex());
    zh->sent_requests.cond.reset(new boost::condition_variable());
    zh->completions_to_process.lock.reset(new boost::mutex());
    zh->completions_to_process.cond.reset(new boost::condition_variable());

    zh->fd = -1;
    zh->state = SessionState::Connecting;
    zh->recv_timeout = recv_timeout;
    zh->watchManager = boost::shared_ptr<WatchManager>(new WatchManager());
    zh->watchManager->setDefaultWatch(watch);
    if (host == 0 || *host == 0) { // what we shouldn't dup
        errno=EINVAL;
        goto abort;
    }
    //parse the host to get the chroot if
    //available
    index_chroot = (char*)strchr(host, '/');
    if (index_chroot) {
        zh->chroot = index_chroot;
        // if chroot is just / set it to null
        if (zh->chroot == "/") {
          zh->chroot = "";
        }
        LOG_DEBUG("Using chroot: " << zh->chroot);
        // cannot use strndup so allocate and strcpy
        zh->hostname = (char *) malloc(index_chroot - host + 1);
        zh->hostname = strncpy(zh->hostname, host, (index_chroot - host));
        //strncpy does not null terminate
        *(zh->hostname + (index_chroot - host)) = '\0';

    } else {
        zh->hostname = strdup(host);
    }
    if (zh->hostname == 0) {
        goto abort;
    }
    if(getaddrs(zh)!=0) {
        goto abort;
    }
    zh->connect_index = 0;
    zh->sessionId = 0;
    zh->sessionPassword = "";
    zh->last_zxid = 0;
    zh->next_deadline.tv_sec=zh->next_deadline.tv_usec=0;

    if (adaptor_init(zh) == -1) {
        goto abort;
    }

    LOG_DEBUG("Init completed. ZooKeeper state=" <<
              SessionState::toString(zh->state));
    return zh;
abort:
    errnosave=errno;
    delete zh;
    errno=errnosave;
    return 0;
}

static buffer_t *dequeue_buffer(buffer_list_t *list) {
  boost::lock_guard<boost::recursive_mutex> lock(list->mutex_);
  if (list->bufferList_.empty()) {
    return NULL;
  }
  return list->bufferList_.release(list->bufferList_.begin()).release();
}

static int remove_buffer(buffer_list_t *list)
{
    buffer_t *b = dequeue_buffer(list);
    if (!b) {
        return 0;
    }
    delete b;
    return 1;
}

static void
queue_buffer(buffer_list_t *list, buffer_t* b) {
  boost::lock_guard<boost::recursive_mutex> lock(list->mutex_);
  list->bufferList_.push_back(b);
}

/* returns:
 * -1 if send failed,
 * 0 if send would block while sending the buffer (or a send was incomplete),
 * 1 if success
 */
static int
send_buffer(int fd, buffer_t* buff) {
  int32_t len = buff->buffer.size();
  int32_t off = buff->offset;
  int rc = -1;

  if (off < (int32_t)sizeof(int32_t)) {
    /* we need to send the length at the beginning */
    int32_t nlen = htonl(len);
    char *b = (char*)&nlen;
    rc = zookeeper_send(fd, b + off, sizeof(nlen) - off);
    if (rc == -1) {
      if (errno != EAGAIN) {
        return -1;
      } else {
        return 0;
      }
    } else {
      buff->offset  += rc;
    }
    off = buff->offset;
  }
  if (off >= 4) {
    /* want off to now represent the offset into the buffer */
    off -= sizeof(len);
    rc = zookeeper_send(fd, buff->buffer.data() + off, len - off);
    if (rc == -1) {
      if (errno != EAGAIN) {
        return -1;
      }
    } else {
      buff->offset += rc;
    }
  }
  return buff->offset == len + (int)sizeof(uint32_t);
}

/* returns:
 * -1 if recv call failed,
 * 0 if recv would block,
 * 1 if success
 */
static int
recv_buffer(int fd, buffer_t *buff) {
  int off = buff->offset;
  int rc = 0;

  /* if buffer is less than 4, we are reading in the length */
  if (off < (int32_t)sizeof(int32_t)) {
    char *buffer = (char*)&(buff->length);
    rc = recv(fd, buffer+off, sizeof(int32_t) - off, 0);
    switch(rc) {
      case 0:
        errno = EHOSTDOWN;
      case -1:
        if (errno == EAGAIN) {
          return 0;
        }
        return -1;
      default:
        buff->offset += rc;
    }
    off = buff->offset;
    if (buff->offset == sizeof(uint32_t)) {
      buff->length = ntohl(buff->length);
      buff->buffer.resize(buff->length);
    }
  }
  if (buff->offset >= (int32_t) sizeof(int32_t)) {
    /* want off to now represent the offset into the buffer */
    off -= sizeof(int32_t);

    rc = recv(fd, (char*)buff->buffer.data() + off, buff->length - off, 0);
    switch(rc) {
      case 0:
        errno = EHOSTDOWN;
      case -1:
        if (errno == EAGAIN) {
          break;
        }
        return -1;
      default:
        buff->offset += rc;
    }
  }
  return buff->offset == buff->length + (int32_t) sizeof(int32_t);
}

void free_buffers(buffer_list_t *list)
{
    while (remove_buffer(list))
        ;
}

void free_completions(zhandle_t *zh, int reason) {
  {
    boost::lock_guard<boost::mutex> lock(*(zh->sent_requests.lock));
    while (!zh->sent_requests.completions.empty()) {
      completion_list_t *cptr = zh->sent_requests.completions.front();
      zh->sent_requests.completions.pop();

      if (cptr == &zhandle_t::completionOfDeath) {
        LOG_DEBUG("Packet of death! do somethign");
      } else if(cptr->xid == PING_XID){
        // Nothing to do with a ping response
        destroy_completion_entry(cptr);
      } else if (cptr->c.isSynchronous) {
        MemoryInStream stream(NULL, 0);
        hadoop::IBinArchive iarchive(stream);
        deserialize_response(cptr->c.type, cptr->xid,
            (ReturnCode::type)reason, cptr, iarchive, zh->chroot);
        destroy_completion_entry(cptr);
      } else {
        // Fake the response
        LOG_DEBUG(boost::format("Enqueueing a fake response: xid=%#08x") %
            cptr->xid);
        buffer_t *bptr = new buffer_t();
        StringOutStream stream(bptr->buffer);
        hadoop::OBinArchive oarchive(stream);
        proto::ReplyHeader header;
        header.setxid(cptr->xid);
        header.setzxid(-1);
        header.seterr(reason);
        header.serialize(oarchive, "header");
        cptr->buffer = bptr;
        queue_completion(&zh->completions_to_process, cptr);
      }
    }
    (*(zh->sent_requests.cond)).notify_all();
  }
  {
    boost::lock_guard<boost::mutex> lock(zh->mutex);
    BOOST_FOREACH(auth_info& info, zh->authList_) {
      if (info.completion) {
        info.completion(reason, info.data);
        info.data = NULL;
        info.completion = NULL;
      }
    }
  }
}

static void cleanup_bufs(zhandle_t *zh, int rc) {
  {
    boost::lock_guard<boost::mutex> lock(zh->mutex);
    free_buffers(&zh->to_send);
    free_buffers(&zh->to_process);
  }
  free_completions(zh, rc);
}

static void handle_error(zhandle_t *zh, ReturnCode::type rc)
{
    close(zh->fd);
    if (is_unrecoverable(zh)) {
        queue_session_event(zh, zh->state);
    } else if (zh->state == SessionState::Connected) {
        zh->state = SessionState::Connecting;
        queue_session_event(zh, SessionState::Connecting);
    }
    // TODO(michim) need to handle completion callbacks
    cleanup_bufs(zh, rc);
    zh->fd = -1;
    zh->connect_index++;
    if (!is_unrecoverable(zh)) {
      // TODO introduce closed state?
      zh->state = SessionState::Connecting;
    }
}

static ReturnCode::type handle_socket_error_msg(zhandle_t *zh, int line, ReturnCode::type rc,
        const std::string& message)
{
    LOG_ERROR(boost::format("%s:%d Socket [%s] zk retcode=%s, errno=%d(%s): %s") %
            __func__ % line % format_current_endpoint_info(zh) %
            ReturnCode::toString(rc) % errno % strerror(errno) % message);
    handle_error(zh,rc);
    return rc;
}

static void auth_completion_func(int rc, zhandle_t* zh) {
  if (zh == NULL) {
    return;
  }

  boost::lock_guard<boost::mutex> lock(zh->mutex);
  if (rc != 0) {
    LOG_ERROR("Authentication scheme " << zh->authList_.front().scheme <<
        " failed. Connection closed.");
    zh->state = SessionState::AuthFailed;
  }else{
    LOG_INFO("Authentication scheme " << zh->authList_.front().scheme <<
        " succeeded.");
    //change state for all auths
    BOOST_FOREACH(auth_info& info, zh->authList_) {
      if (info.completion) {
        info.completion(rc, info.data);
        info.completion = NULL;
        info.data = NULL;
      }
    }
  }
}

/**
 * The caller must acquire zh->mutex before calling this function.
 */
static int send_info_packet(zhandle_t *zh, auth_info* auth) {
  int rc = 0;
  buffer_t* buffer = new buffer_t();
  StringOutStream stream(buffer->buffer);
  hadoop::OBinArchive oarchive(stream);

  proto::RequestHeader header;
  header.setxid(AUTH_XID);
  header.settype(OpCode::SetAuth);
  header.serialize(oarchive, "header");

  proto::AuthPacket req;
  req.settype(0); // ignored by the server
  req.getscheme() = auth->scheme;
  req.getauth() = auth->auth;
  req.serialize(oarchive, "req");

  queue_buffer(&zh->to_send, buffer);
  adaptor_send_queue(zh, 0);
  return rc;
}

/** send all auths, not just the last one **/
static int send_auth_info(zhandle_t *zh) {
    int rc = 0;
    {
        boost::lock_guard<boost::mutex> lock(zh->mutex);
        BOOST_FOREACH(auth_info& info, zh->authList_) {
          rc = send_info_packet(zh, &info);
        }
    }
    LOG_DEBUG("Sending all auth info request to " << format_current_endpoint_info(zh));
    return (rc <0) ? ReturnCode::MarshallingError:ReturnCode::Ok;
}

static int send_last_auth_info(zhandle_t *zh) {
  int rc = 0;
  {
    boost::lock_guard<boost::mutex> lock(zh->mutex);
    if (zh->authList_.empty()) {
      return ReturnCode::Ok; // there is nothing to send
    }
    rc = send_info_packet(zh, &(zh->authList_.back()));
  }
  LOG_DEBUG("Sending auth info request to " << format_current_endpoint_info(zh));
  return (rc < 0) ? ReturnCode::MarshallingError : ReturnCode::Ok;
}

static void
send_set_watches(zhandle_t *zh) {
  proto::SetWatches req;
  zh->watchManager->getExistsPaths(req.getexistWatches());
  zh->watchManager->getGetChildrenPaths(req.getchildWatches());
  zh->watchManager->getGetDataPaths(req.getdataWatches());

  // return if there are no pending watches
  if (req.getdataWatches().empty() && req.getexistWatches().empty() &&
      req.getchildWatches().empty()) {
    return;
  }

  buffer_t* buffer = new buffer_t();
  StringOutStream stream(buffer->buffer);
  hadoop::OBinArchive oarchive(stream);

  proto::RequestHeader header;
  header.setxid(SET_WATCHES_XID);
  header.settype(OpCode::SetWatches);

  std::vector<std::string> paths;
  req.setrelativeZxid(zh->last_zxid);

  header.serialize(oarchive, "header");
  req.serialize(oarchive, "req");

  {
    boost::lock_guard<boost::mutex> lock(zh->mutex);
    queue_buffer(&zh->to_send, buffer);
  }
  adaptor_send_queue(zh, 0);
  LOG_DEBUG("Sending SetWatches request to " << format_current_endpoint_info(zh));
}

static ReturnCode::type
sendConnectRequest(zhandle_t *zh) {
  int rc;
  std::string serialized;
  StringOutStream stream(serialized);
  hadoop::OBinArchive oarchive(stream);

  proto::ConnectRequest request;
  request.setprotocolVersion(0);
  request.settimeOut(zh->recv_timeout);
  request.setlastZxidSeen(zh->last_zxid);
  request.setsessionId(zh->sessionId);
  request.getpasswd() = zh->sessionPassword;
  request.serialize(oarchive, "connect");
  uint32_t len = htonl(serialized.size());
  rc=zookeeper_send(zh->fd, &len, sizeof(len));
  rc=rc<0 ? rc : zookeeper_send(zh->fd, serialized.data(), serialized.size());
  if (rc<0) {
    return handle_socket_error_msg(zh, __LINE__, ReturnCode::ConnectionLoss, "");
  }
  zh->state = SessionState::Associating;
  return ReturnCode::Ok;
}

static inline int calculate_interval(const struct timeval *start,
        const struct timeval *end)
{
    int interval;
    struct timeval i = *end;
    i.tv_sec -= start->tv_sec;
    i.tv_usec -= start->tv_usec;
    interval = i.tv_sec * 1000 + (i.tv_usec/1000);
    return interval;
}

static struct timeval get_timeval(int interval)
{
    struct timeval tv;
    if (interval < 0) {
        interval = 0;
    }
    tv.tv_sec = interval/1000;
    tv.tv_usec = (interval%1000)*1000;
    return tv;
}

 static int add_void_completion(zhandle_t *zh, int xid, void_completion_t dc,
     const void *data, bool isSynchronous);
 static int add_string_completion(zhandle_t *zh, int xid,
     string_completion_t dc, const void *data, bool isSynchronous);

int
send_ping(zhandle_t* zh) {
  int rc = 0;
  std::string serialized;
  buffer_t* buffer = new buffer_t();
  StringOutStream stream(buffer->buffer);
  hadoop::OBinArchive oarchive(stream);

  proto::RequestHeader header;
  header.setxid(PING_XID);
  header.settype(OpCode::Ping);
  header.serialize(oarchive, "header");
  {
    boost::lock_guard<boost::mutex> lock(zh->mutex);
    gettimeofday(&zh->last_ping, 0);
    rc = rc < 0 ? rc : add_void_completion(zh, header.getxid(), 0, 0, false);
    queue_buffer(&zh->to_send, buffer);
  }
  return rc<0 ? rc : adaptor_send_queue(zh, 0);
}

int zookeeper_interest(zhandle_t *zh, int *fd, int *interest,
     struct timeval *tv) {
    struct timeval now;
    if(zh==0 || fd==0 ||interest==0 || tv==0)
        return ReturnCode::BadArguments;
    if (is_unrecoverable(zh))
        return ReturnCode::InvalidState;
    gettimeofday(&now, 0);
    if(zh->next_deadline.tv_sec!=0 || zh->next_deadline.tv_usec!=0){
        int time_left = calculate_interval(&zh->next_deadline, &now);
        int max_exceed = zh->recv_timeout / 10 > 200 ? 200 : 
                         (zh->recv_timeout / 10);
        if (time_left > max_exceed)
            LOG_WARN("Exceeded deadline by " << time_left << "ms");
    }
    *fd = zh->fd;
    *interest = 0;
    tv->tv_sec = 0;
    tv->tv_usec = 0;
    if (*fd == -1) {
        if (zh->connect_index == zh->addrs_count) {
            /* Wait a bit before trying again so that we don't spin */
            zh->connect_index = 0;
        }else {
            int rc;
            int enable_tcp_nodelay = 1;
            int ssoresult;

            zh->fd = socket(zh->addrs[zh->connect_index].ss_family, SOCK_STREAM, 0);
            if (zh->fd < 0) {
                return handle_socket_error_msg(zh,__LINE__,
                    ReturnCode::SystemError, "socket() call failed");
            }
            ssoresult = setsockopt(zh->fd, IPPROTO_TCP, TCP_NODELAY, &enable_tcp_nodelay, sizeof(enable_tcp_nodelay));
            if (ssoresult != 0) {
                LOG_WARN("Unable to set TCP_NODELAY, operation latency may be effected");
            }
            fcntl(zh->fd, F_SETFL, O_NONBLOCK|fcntl(zh->fd, F_GETFL, 0));
#if defined(AF_INET6)
            if (zh->addrs[zh->connect_index].ss_family == AF_INET6) {
                rc = connect(zh->fd, (struct sockaddr*) &zh->addrs[zh->connect_index], sizeof(struct sockaddr_in6));
            } else {
#else
               LOG_DEBUG("[zk] connect()");
            {
#endif
                rc = connect(zh->fd, (struct sockaddr*) &zh->addrs[zh->connect_index], sizeof(struct sockaddr_in));
            }
            if (rc == -1) {
                /* we are handling the non-blocking connect according to
                 * the description in section 16.3 "Non-blocking connect"
                 * in UNIX Network Programming vol 1, 3rd edition */
                if (errno == EWOULDBLOCK || errno == EINPROGRESS)
                    zh->state = SessionState::Connecting;
                else
                    return handle_socket_error_msg(zh,__LINE__,
                            ReturnCode::ConnectionLoss,"connect() call failed");
            } else {
                if((rc=sendConnectRequest(zh))!=0)
                    return rc;
                LOG_INFO("Initiated connection to server: " <<
                        format_endpoint_info(&zh->addrs[zh->connect_index]));
            }
        }
        *fd = zh->fd;
        *tv = get_timeval(zh->recv_timeout/3);
        zh->last_recv = now;
        zh->last_send = now;
        zh->last_ping = now;
    }
    if (zh->fd != -1) {
        int idle_recv = calculate_interval(&zh->last_recv, &now);
        int idle_send = calculate_interval(&zh->last_send, &now);
        int recv_to = zh->recv_timeout*2/3 - idle_recv;
        int send_to = zh->recv_timeout/3;
        // have we exceeded the receive timeout threshold?
        if (recv_to <= 0) {
            // We gotta cut our losses and connect to someone else
            errno = ETIMEDOUT;
            *fd=-1;
            *interest=0;
            *tv = get_timeval(0);
            return handle_socket_error_msg(zh, __LINE__,ReturnCode::OperationTimeout, "");
        }
        // We only allow 1/3 of our timeout time to expire before sending
        // a PING
        if (zh->state==SessionState::Connected) {
            send_to = zh->recv_timeout/3 - idle_send;
            if (send_to <= 0 && zh->sent_requests.completions.empty()) {
//                LOG_DEBUG(("Sending PING to %s (exceeded idle by %dms)",
//                                format_current_endpoint_info(zh),-send_to));
                int rc=send_ping(zh);
                if (rc < 0){
                    //LOG_ERROR("failed to send PING request (zk retcode=" << rc << ")");
                    return rc;
                }
                send_to = zh->recv_timeout/3;
            }
        }
        // choose the lesser value as the timeout
        *tv = get_timeval(recv_to < send_to? recv_to:send_to);
        zh->next_deadline.tv_sec = now.tv_sec + tv->tv_sec;
        zh->next_deadline.tv_usec = now.tv_usec + tv->tv_usec;
        if (zh->next_deadline.tv_usec > 1000000) {
            zh->next_deadline.tv_sec += zh->next_deadline.tv_usec / 1000000;
            zh->next_deadline.tv_usec = zh->next_deadline.tv_usec % 1000000;
        }
        *interest = ZOOKEEPER_READ;
        /* we are interested in a write if we are connected and have something
         * to send, or we are waiting for a connect to finish. */
        if ((!zh->to_send.bufferList_.empty() &&
            zh->state == SessionState::Connected) ||
            zh->state == SessionState::Connecting) {
            *interest |= ZOOKEEPER_WRITE;
        }
    }
    return ReturnCode::Ok;
}

static ReturnCode::type
check_events(zhandle_t *zh, int events)
{
    if (zh->fd == -1)
        return ReturnCode::InvalidState;
    if ((events&ZOOKEEPER_WRITE)&&(zh->state == SessionState::Connecting)) {
        int rc, error;
        ReturnCode::type returnCode;
        socklen_t len = sizeof(error);
        rc = getsockopt(zh->fd, SOL_SOCKET, SO_ERROR, &error, &len);
        /* the description in section 16.4 "Non-blocking connect"
         * in UNIX Network Programming vol 1, 3rd edition, points out
         * that sometimes the error is in errno and sometimes in error */
        if (rc < 0 || error) {
            if (rc == 0)
                errno = error;
            return handle_socket_error_msg(zh, __LINE__,ReturnCode::ConnectionLoss,
                "server refused to accept the client");
        }
        if((returnCode = sendConnectRequest(zh)) != ReturnCode::Ok)
            return returnCode;
        LOG_INFO("initiated connection to server: " <<
                format_endpoint_info(&zh->addrs[zh->connect_index]));
        return ReturnCode::Ok;
    }
    if (!(zh->to_send.bufferList_.empty()) && (events&ZOOKEEPER_WRITE)) {
        /* make the flush call non-blocking by specifying a 0 timeout */
        ReturnCode::type returnCode = flush_send_queue(zh,0);
        if (returnCode == ReturnCode::InvalidState) {
          return returnCode;
        } else if (returnCode != ReturnCode::Ok) {
            return handle_socket_error_msg(zh,__LINE__,ReturnCode::ConnectionLoss,
                "failed while flushing send queue");
        }
    }
    if (events&ZOOKEEPER_READ) {
        int rc;
        if (zh->input_buffer == 0) {
            zh->input_buffer = new buffer_t();
        }

        rc = recv_buffer(zh->fd, zh->input_buffer);
        LOG_DEBUG("buffer size: " << zh->input_buffer->buffer.size());
        if (rc < 0) {
            delete zh->input_buffer;
            zh->input_buffer = NULL;
            return handle_socket_error_msg(zh, __LINE__,ReturnCode::ConnectionLoss,
                "failed while receiving a server response");
        }
        if (rc > 0) {
            gettimeofday(&zh->last_recv, 0);
            if (zh->state != SessionState::Associating) {
                boost::lock_guard<boost::recursive_mutex> lock(zh->to_process.mutex_);
                zh->to_process.bufferList_.push_back(zh->input_buffer);
            } else  {
                // Process connect response.
                int64_t oldid,newid;
                MemoryInStream istream(zh->input_buffer->buffer.data(),
                                       zh->input_buffer->length);
                hadoop::IBinArchive iarchive(istream);
                zh->connectResponse.deserialize(iarchive,"connect");

                /* We are processing the connect response , so we need to finish
                 * the connection handshake */
                oldid = zh->sessionId;
                newid = zh->connectResponse.getsessionId();
                delete zh->input_buffer;
                zh->input_buffer = NULL;
                if (oldid != 0 && oldid != newid) {
                    zh->state = SessionState::Expired;
                    errno = ESTALE;
                    return handle_socket_error_msg(zh,__LINE__,ReturnCode::SessionExpired,
                    "");
                } else {
                    zh->recv_timeout = zh->connectResponse.gettimeOut();
                    zh->sessionId = newid;
                    zh->sessionPassword = zh->connectResponse.getpasswd();
                    zh->state = SessionState::Connected;
                    LOG_INFO(
                      boost::format("session establishment complete on server [%s], sessionId=%#llx, negotiated timeout=%d") %
                              format_endpoint_info(&zh->addrs[zh->connect_index]) % newid % zh->recv_timeout);
                    /* we want the auth to be sent for, but since both call push to front
                       we need to call send_watch_set first */
                    send_set_watches(zh);
                    /* send the authentication packet now */
                    send_auth_info(zh);
                    queue_session_event(zh, SessionState::Connected);
                }
            }
            zh->input_buffer = 0;
        }
    }
    return ReturnCode::Ok;
}

// IO thread queues session events to be processed by the completion thread
static int queue_session_event(zhandle_t *zh, SessionState::type state) {
  LOG_DEBUG("Notifying watches of a session event: new state=" <<
            SessionState::toString(state));
  buffer_t* buffer = new buffer_t();
  StringOutStream stream(buffer->buffer);
  hadoop::OBinArchive oarchive(stream);
  completion_list_t *cptr;
  proto::ReplyHeader header;
  header.setxid(WATCHER_EVENT_XID);
  header.setzxid(0);
  header.seterr(0);
  proto::WatcherEvent event;
  event.settype(WatchEvent::SessionStateChanged);
  event.setstate(state);
  event.getpath() = "";

  header.serialize(oarchive, "header");
  event.serialize(oarchive, "event");
  cptr = create_completion_entry(WATCHER_EVENT_XID,-1,0,0,0,0, false);

  cptr->buffer = buffer;
  cptr->buffer->offset = buffer->buffer.size();
  zh->watchManager->getWatches(WatchEvent::SessionStateChanged,
      zh->state, "", cptr->c.watches);
  queue_completion(&zh->completions_to_process, cptr);
  return ReturnCode::Ok;
}

completion_list_t*
dequeue_completion(completion_head_t* list) {
  boost::lock_guard<boost::mutex> lock(*(list->lock));
  completion_list_t *cptr = NULL;
  if (!list->completions.empty()) {
    cptr = list->completions.front();
    list->completions.pop();
  }
  list->cond->notify_all();
  return cptr;
}

static int
deserialize_multi(int xid, completion_list_t *cptr,
                  hadoop::IBinArchive& iarchive,
                  boost::ptr_vector<OpResult>& results) {

  boost::ptr_vector<OpResult> temp;
  int rc = 0;
  boost::ptr_vector<OpResult>* clist = cptr->c.results.get();
  assert(clist);
  proto::MultiHeader mheader;
  mheader.deserialize(iarchive, "multiheader");
  while (!mheader.getdone()) {
    if (mheader.gettype() == -1) {
      proto::ErrorResponse errorResponse;
      errorResponse.deserialize(iarchive, "error");
      ReturnCode::type error = (ReturnCode::type)errorResponse.geterr();
      LOG_DEBUG("got error response for: " << ReturnCode::toString(error));
      OpResult* result = new OpResult::Error();
      result->setReturnCode(error);
      results.push_back(result);
      if (rc == 0 && (int)error != 0 && (int)error != ReturnCode::RuntimeInconsistency) {
        rc = (int)error;
      }
    } else {
      switch((clist->begin())->getType()) {
        case OpCode::Remove: {
          ReturnCode::type zrc = (ReturnCode::type) mheader.geterr();
          LOG_DEBUG("got delete response for: " << ReturnCode::toString(zrc));
          OpResult* result = clist->release(clist->begin()).release();
          results.push_back(result);
          break;
          }
        case OpCode::Create: {
          proto::CreateResponse res;
          res.deserialize(iarchive, "reply");
          ReturnCode::type zrc = (ReturnCode::type) mheader.geterr();
          LOG_DEBUG("got create response for: " << res.getpath() << ": " <<
                    ReturnCode::toString(zrc));
          OpResult* result = clist->release(clist->begin()).release();
          dynamic_cast<OpResult::Create*>(result)->setPathCreated(res.getpath());
          results.push_back(result);
          break;
        }
        case OpCode::SetData: {
          proto::SetDataResponse res;
          res.deserialize(iarchive, "reply");
          ReturnCode::type zrc = (ReturnCode::type) mheader.geterr();
          LOG_DEBUG("got setData response: " << ReturnCode::toString(zrc));
          OpResult* result = clist->release(clist->begin()).release();
          dynamic_cast<OpResult::SetData*>(result)->setStat(res.getstat());
          results.push_back(result);
          break;
        }
        case OpCode::Check: {
          ReturnCode::type zrc = (ReturnCode::type) mheader.geterr();
          LOG_DEBUG("got check response for: " << ReturnCode::toString(zrc));
          OpResult* result = clist->release(clist->begin()).release();
          results.push_back(result);
          break;
        }
        default:
          LOG_ERROR("Unknown multi operation type: " << (clist->begin())->getType());
      }
    }
    mheader.deserialize(iarchive, "multiheader");
  }
  LOG_DEBUG("returning: " << ReturnCode::toString((ReturnCode::type)rc));
  return rc;
}

static void deserialize_response(int type, int xid, ReturnCode::type rc,
    completion_list_t *cptr, hadoop::IBinArchive& iarchive,
    const std::string& chroot) {
  switch (type) {
    case COMPLETION_DATA:
      LOG_DEBUG(boost::format("Calling COMPLETION_DATA for xid=%#08x rc=%s") %
          cptr->xid % ReturnCode::toString(rc));
      if (rc != ReturnCode::Ok) {
        data::Stat stat;
        cptr->c.data_result(rc, "", stat, cptr->data);
      } else {
        proto::GetDataResponse res;
        res.deserialize(iarchive, "reply");
        cptr->c.data_result(rc, res.getdata(), res.getstat(), cptr->data);
      }
      break;
    case COMPLETION_STAT:
      LOG_DEBUG(boost::format("Calling COMPLETION_STAT for xid=%#08x rc=%s") %
          cptr->xid % ReturnCode::toString(rc));
      if (rc != ReturnCode::Ok) {
        data::Stat stat;
        cptr->c.stat_result(rc, stat, cptr->data);
      } else {
        proto::SetDataResponse res;
        res.deserialize(iarchive, "reply");
        cptr->c.stat_result(rc, res.getstat(), cptr->data);
      }
      break;
    case COMPLETION_STRINGLIST:
      LOG_DEBUG(boost::format("Calling COMPLETION_STRINGLIST for xid=%#08x rc=%s") %
          cptr->xid % ReturnCode::toString(rc));
      if (rc != ReturnCode::Ok) {
        std::vector<std::string> res;
        cptr->c.strings_result(rc, res, cptr->data);
      } else {
        proto::GetChildrenResponse res;
        res.deserialize(iarchive, "reply");
        cptr->c.strings_result(rc, res.getchildren(), cptr->data);
      }
      break;
    case COMPLETION_STRINGLIST_STAT:
      LOG_DEBUG(boost::format("Calling COMPLETION_STRINGLIST_STAT for xid=%#08x rc=%s") %
          cptr->xid % ReturnCode::toString(rc));
      if (rc != ReturnCode::Ok) {
        std::vector<std::string> children;
        data::Stat stat;
        cptr->c.strings_stat_result(rc, children, stat, cptr->data);
      } else {
        proto::GetChildren2Response res;
        res.deserialize(iarchive, "reply");
        cptr->c.strings_stat_result(rc, res.getchildren(), res.getstat(), cptr->data);
      }
      break;
    case COMPLETION_STRING:
      LOG_DEBUG(boost::format("Calling COMPLETION_STRING for xid=%#08x rc=%s") %
          cptr->xid % ReturnCode::toString(rc));
      if (rc != ReturnCode::Ok) {
        cptr->c.string_result(rc, "", cptr->data);
      } else {
        proto::CreateResponse res;
        res.deserialize(iarchive, "reply");

        //ZOOKEEPER-1027
        cptr->c.string_result(rc, PathUtils::stripChroot(res.getpath(), chroot), cptr->data);
      }
      break;
    case COMPLETION_ACLLIST:
      LOG_DEBUG(boost::format("Calling COMPLETION_ACLLIST for xid=%#08x rc=%s") %
          cptr->xid % ReturnCode::toString(rc));
      if (rc != ReturnCode::Ok) {
        std::vector<data::ACL> acl;
        data::Stat stat;
        cptr->c.acl_result(rc, acl, stat, cptr->data);
      } else {
        proto::GetACLResponse res;
        res.deserialize(iarchive, "reply");
        cptr->c.acl_result(rc, res.getacl(), res.getstat(), cptr->data);
      }
      break;
    case COMPLETION_VOID:
      LOG_DEBUG(boost::format("Calling COMPLETION_VOID for xid=%#08x rc=%s") %
          cptr->xid % ReturnCode::toString(rc));
      if (xid != PING_XID && cptr->c.void_result) {
        cptr->c.void_result(rc, cptr->data);
      }
      break;
    case COMPLETION_MULTI: {
      assert(cptr);
      LOG_DEBUG(boost::format("Calling COMPLETION_MULTI for xid=%#08x rc=%s") %
          cptr->xid % ReturnCode::toString(rc));
      if (rc != ReturnCode::Ok) {
        boost::ptr_vector<OpResult> results;
        cptr->c.multi_result(rc, results, cptr->data);
      } else {
        boost::ptr_vector<OpResult> results;
        int returnCode = deserialize_multi(xid, cptr, iarchive, results);
        if (cptr->c.multi_result) {
          cptr->c.multi_result(returnCode, results, cptr->data);
        }
      }
      break;
    }
    default:
      LOG_DEBUG("Unsupported completion type=" << cptr->c.type);
  }
}


/* handles async completion (both single- and multithreaded) */
ReturnCode::type process_completions(zhandle_t *zh) {
  completion_list_t *cptr;
  while ((cptr = dequeue_completion(&zh->completions_to_process)) != 0) {
    if (cptr == &zhandle_t::completionOfDeath) {
      LOG_DEBUG("Received the completion of death");
      return ReturnCode::InvalidState;
    }
    buffer_t *bptr = cptr->buffer;
    MemoryInStream stream(bptr->buffer.data(), bptr->buffer.size());
    hadoop::IBinArchive iarchive(stream);
    proto::ReplyHeader header;
    header.deserialize(iarchive, "header");

    if (header.getxid() == WATCHER_EVENT_XID) {
      /* We are doing a notification, so there is no pending request */
      int type, state;
      proto::WatcherEvent event;
      event.deserialize(iarchive, "event");
      type = event.gettype();
      state = event.getstate();
      LOG_DEBUG(boost::format("Calling a watcher for node [%s], type = %d event=%s") %
          event.getpath() % cptr->c.type %
          WatchEvent::toString((WatchEvent::type)type));
      deliverWatchers(zh,type,state,event.getpath().c_str(), cptr->c.watches);
    } else {
      deserialize_response(cptr->c.type, header.getxid(),
          (ReturnCode::type)header.geterr(), cptr, iarchive, zh->chroot);
    }
    destroy_completion_entry(cptr);
  }
  return ReturnCode::Ok;
}

int
zookeeper_process(zhandle_t *zh, int events) {
  buffer_t *bptr;
  ReturnCode::type rc;

  if (zh==NULL)
    return ReturnCode::BadArguments;
  if (is_unrecoverable(zh))
    return ReturnCode::InvalidState;
  rc = check_events(zh, events);
  if (rc!=ReturnCode::Ok) {
    return rc;
  }
  while (rc >= 0 && (bptr=dequeue_buffer(&zh->to_process))) {
    MemoryInStream stream(bptr->buffer.data(), bptr->length);
    hadoop::IBinArchive iarchive(stream);
    proto::ReplyHeader header;
    header.deserialize(iarchive, "header");

    if (header.getzxid() > 0) {
      zh->last_zxid = header.getzxid();
    }

    if (header.getxid() == WATCHER_EVENT_XID) {
      LOG_DEBUG("Processing WATCHER_EVENT");
      proto::WatcherEvent event;
      event.deserialize(iarchive, "event");
      completion_list_t* c =
        create_completion_entry(WATCHER_EVENT_XID,-1,0,0,0,0, false);
      c->buffer = bptr;
      zh->watchManager->getWatches((WatchEvent::type)event.gettype(),
                                   zh->state, event.getpath(), c->c.watches);
      queue_completion(&zh->completions_to_process, c);
    } else if (header.getxid() == SET_WATCHES_XID) {
      LOG_DEBUG("Processing SET_WATCHES");
      delete bptr;
    } else if (header.getxid() == AUTH_XID) {
      LOG_DEBUG("Processing AUTH_XID");
      // special handling for the AUTH response as it may come back out-of-band
      auth_completion_func(header.geterr(), zh);
      delete bptr;
      // auth completion may change the connection state to unrecoverable
      if(is_unrecoverable(zh)){
        handle_error(zh, ReturnCode::AuthFailed);
        return ReturnCode::AuthFailed;
      }
    } else {
      rc = (ReturnCode::type)header.geterr();
      /* Find the request corresponding to the response */
      completion_list_t *cptr = dequeue_completion(&zh->sent_requests);

      /* [ZOOKEEPER-804] Don't assert if zookeeper_close has been called. */
      if (zh->close_requested == 1 && !cptr) {
        return ReturnCode::InvalidState;
      }
      assert(cptr);
      /* The requests are going to come back in order */
      if (cptr->xid != header.getxid()) {
        // received unexpected (or out-of-order) response
        LOG_DEBUG("Processing unexpected or out-of-order response!");
        delete bptr;
        // put the completion back on the queue (so it gets properly
        // signaled and deallocated) and disconnect from the server
        // TODO destroy completion
        queue_completion(&zh->sent_requests,cptr);
        return handle_socket_error_msg(zh, __LINE__,ReturnCode::RuntimeInconsistency,
            "");
      }
      if (cptr->watch.get() != NULL) {
        LOG_DEBUG(boost::format("Checking whether the watch should be registered:"
              " xid=%#08x path=%s, rc=%s") % header.getxid() %
            "FIXME" % ReturnCode::toString(rc));
        cptr->watch->activate(rc);
      }
      if (header.getxid() == PING_XID) {
        int elapsed = 0;
        struct timeval now;
        gettimeofday(&now, 0);
        elapsed = calculate_interval(&zh->last_ping, &now);
        LOG_DEBUG("Got ping response in " << elapsed << "ms");
        delete bptr;
        destroy_completion_entry(cptr);
      } else {
        if (cptr->c.isSynchronous) {
          LOG_DEBUG(boost::format("Processing synchronous request "
                "from the IO thread: xid=%#08x") % header.getxid());
          deserialize_response(cptr->c.type, header.getxid(),
              (ReturnCode::type)header.geterr(), cptr, iarchive, zh->chroot);
          delete bptr;
          destroy_completion_entry(cptr);
        } else {
          cptr->buffer = bptr;
          queue_completion(&zh->completions_to_process, cptr);
        }
      }
    }
  }
  return ReturnCode::Ok;
}

SessionState::type zoo_state(zhandle_t *zh)
{
  if (!zh) {
    return (SessionState::type)0;
  }
  return zh->state;
}

static completion_list_t* create_completion_entry(int xid, int completion_type,
    const void *dc, const void *data, WatchRegistration* wo,
    boost::ptr_vector<OpResult>* results, bool isSynchronous) {
  completion_list_t *c = new completion_list_t();
  c->c.type = completion_type;
  c->data = data;
  switch(c->c.type) {
    case COMPLETION_VOID:
      c->c.void_result = (void_completion_t)dc;
      break;
    case COMPLETION_STRING:
      c->c.string_result = (string_completion_t)dc;
      break;
    case COMPLETION_DATA:
      c->c.data_result = (data_completion_t)dc;
      break;
    case COMPLETION_STAT:
      c->c.stat_result = (stat_completion_t)dc;
      break;
    case COMPLETION_STRINGLIST:
      c->c.strings_result = (strings_completion_t)dc;
      break;
    case COMPLETION_STRINGLIST_STAT:
      c->c.strings_stat_result = (strings_stat_completion_t)dc;
      break;
    case COMPLETION_ACLLIST:
      c->c.acl_result = (acl_completion_t)dc;
      break;
    case COMPLETION_MULTI:
      assert(results);
      c->c.multi_result = (multi_completion_t)dc;
      c->c.results.reset(results);
      break;
  }
  c->xid = xid;
  c->watch.reset(wo);
  c->c.isSynchronous = isSynchronous;
  return c;
}

static void destroy_completion_entry(completion_list_t* c) {
  if(c != NULL) {
    if(c->buffer != NULL)
      delete c->buffer;
    delete c;
  }
}

static void
queue_completion(completion_head_t *list, completion_list_t *c) {
  boost::lock_guard<boost::mutex> lock(*(list->lock));
  list->completions.push(c);
  list->cond->notify_all();
}

static int add_completion(zhandle_t *zh, int xid, int completion_type,
    const void *dc, const void *data, WatchRegistration* wo,
    boost::ptr_vector<OpResult>* results, bool isSynchronous) {
  completion_list_t *c =create_completion_entry(xid, completion_type, dc, data,
                                                wo, results, isSynchronous);
  int rc = 0;
  if (!c) {
    return ReturnCode::SystemError;
  }
  if (zh->close_requested != 1) {
    queue_completion(&zh->sent_requests, c);
    rc = ReturnCode::Ok;
  } else {
    free(c);
    rc = ReturnCode::InvalidState;
  }
  return rc;
}

static int add_data_completion(zhandle_t *zh, int xid, data_completion_t dc,
        const void *data, WatchRegistration* wo, bool isSynchronous)
{
    return add_completion(zh, xid, COMPLETION_DATA, (const void*)dc, data, wo, 0, isSynchronous);
}

static int add_stat_completion(zhandle_t *zh, int xid, stat_completion_t dc,
        const void *data, WatchRegistration* wo, bool isSynchronous)
{
    return add_completion(zh, xid, COMPLETION_STAT, (const void*)dc, data, wo, 0, isSynchronous);
}

static int add_strings_stat_completion(zhandle_t *zh, int xid,
        strings_stat_completion_t dc, const void *data,
        WatchRegistration* wo, bool isSynchronous)
{
    return add_completion(zh, xid, COMPLETION_STRINGLIST_STAT, (const void*)dc, data, wo, 0, isSynchronous);
}

static int add_acl_completion(zhandle_t *zh, int xid, acl_completion_t dc,
        const void *data, bool isSynchronous)
{
    return add_completion(zh, xid, COMPLETION_ACLLIST, (const void*)dc, data, 0, 0, isSynchronous);
}

static int add_void_completion(zhandle_t *zh, int xid, void_completion_t dc,
        const void *data, bool isSynchronous)
{
    return add_completion(zh, xid, COMPLETION_VOID, (const void*)dc, data, 0, 0, isSynchronous);
}

static int add_string_completion(zhandle_t *zh, int xid,
        string_completion_t dc, const void *data, bool isSynchronous)
{
    return add_completion(zh, xid, COMPLETION_STRING, (const void*)dc, data, 0, 0, isSynchronous);
}

static int add_multi_completion(zhandle_t *zh, int xid, multi_completion_t dc,
        const void *data, boost::ptr_vector<OpResult>* results, bool isSynchronous) {
    return add_completion(zh, xid, COMPLETION_MULTI, (const void*)dc, data, 0, results, isSynchronous);
}

int
zookeeper_close(zhandle_t *zh) {
  int rc = ReturnCode::Ok;
  if (zh == 0)
    return ReturnCode::BadArguments;
  if (zh->close_requested) {
    return ReturnCode::Ok;
  }
  zh->close_requested = 1;
  LOG_DEBUG("Enqueueing the completion of death");
  queue_completion(&zh->completions_to_process, &zhandle_t::completionOfDeath);
  if (boost::this_thread::get_id() == zh->threads.completion.get_id()) {
    // completion thread
    wakeup_io_thread(zh);
  } else if (boost::this_thread::get_id() == zh->threads.io.get_id()) {
    // io thread
  } else {
    // some other thread
    wakeup_io_thread(zh);
    zh->threads.io.join();
    zh->threads.completion.join();
    delete zh;
  }
  return ReturnCode::Ok;

  /* No need to decrement the counter since we're just going to
   * destroy the handle later. */
  if(zh->state==SessionState::Connected){
    buffer_t* buffer = new buffer_t();
    StringOutStream stream(buffer->buffer);
    hadoop::OBinArchive oarchive(stream);

    proto::RequestHeader header;
    header.setxid(get_xid());
    header.settype(OpCode::CloseSession);
    header.serialize(oarchive, "header");
    LOG_INFO(boost::format("Closing zookeeper sessionId=%#llx to [%s]\n") %
        zh->sessionId % format_current_endpoint_info(zh));
    {
      boost::lock_guard<boost::mutex> lock(zh->mutex);
      queue_buffer(&zh->to_send, buffer);
    }

    /* make sure the close request is sent; we set timeout to an arbitrary
     * (but reasonable) number of milliseconds since we want the call to block*/
    rc = adaptor_send_queue(zh, 3000);
  }
  return rc;
}

static ReturnCode::type getRealString(zhandle_t *zh, int flags, const std::string& path,
    std::string& pathStr) {
  if (zh == NULL) {
    return ReturnCode::BadArguments;
  }
  pathStr = PathUtils::prependChroot(path, zh->chroot);
  LOG_DEBUG("Real request path: " << pathStr);
  return ReturnCode::Ok;
}

/*---------------------------------------------------------------------------*
 * ASYNC API
 *---------------------------------------------------------------------------*/
int zoo_awget(zhandle_t *zh, const std::string& path,
        boost::shared_ptr<Watch> watch,
        data_completion_t dc, const void *data, bool isSynchronous)
{
  std::string pathStr;
  int rc = getRealString(zh, 0, path, pathStr);
  if (rc != ReturnCode::Ok) {
    return rc;
  }

  buffer_t* buffer = new buffer_t();
  StringOutStream stream(buffer->buffer);
  hadoop::OBinArchive oarchive(stream);

  proto::RequestHeader header;
  header.setxid(get_xid());
  header.settype(OpCode::GetData);
  header.serialize(oarchive, "header");

  proto::GetDataRequest req;
  req.getpath() = pathStr;
  req.setwatch(watch.get() != NULL);
  req.serialize(oarchive, "req");

  WatchRegistration* reg = NULL;
  if (watch.get() != NULL) {
    reg = new GetDataWatchRegistration(zh->watchManager, pathStr, watch);
  }
  {
    boost::lock_guard<boost::mutex> lock(zh->mutex);
    rc = rc < 0 ? rc : add_data_completion(zh, header.getxid(), dc, data,
        reg, isSynchronous);
    queue_buffer(&zh->to_send, buffer);
  }

  LOG_DEBUG(boost::format("Sending a get request xid=%#08x for path [%s] to %s") %
      header.getxid() % pathStr % format_current_endpoint_info(zh));
  /* make a best (non-blocking) effort to send the requests asap */
  adaptor_send_queue(zh, 0);
  return (rc < 0)?ReturnCode::MarshallingError:ReturnCode::Ok;
}

int zoo_aset(zhandle_t *zh, const std::string& path, const char *buf, int buflen,
        int version, stat_completion_t dc, const void *data, bool isSynchronous)
{
  std::string pathStr;
  int rc = getRealString(zh, 0, path, pathStr);
  if (rc != ReturnCode::Ok) {
    return rc;
  }

  buffer_t* buffer = new buffer_t();
  StringOutStream stream(buffer->buffer);
  hadoop::OBinArchive oarchive(stream);

  proto::RequestHeader header;
  header.setxid(get_xid());
  header.settype(OpCode::SetData);
  header.serialize(oarchive, "header");

  proto::SetDataRequest req;
  req.getpath() = pathStr;
  if (buf != NULL && buflen >= 0) {
    req.getdata() = std::string(buf, buflen);
  } else {
    req.getdata() = "";
  }
  req.setversion(version);
  req.serialize(oarchive, "req");

  {
    boost::lock_guard<boost::mutex> lock(zh->mutex);
    rc = rc < 0 ? rc : add_stat_completion(zh, header.getxid(), dc, data,0,
        isSynchronous);
    queue_buffer(&zh->to_send, buffer);
  }

  LOG_DEBUG(boost::format("Sending set request xid=%#08x for path [%s] to %s") %
      header.getxid() % path % format_current_endpoint_info(zh));
  /* make a best (non-blocking) effort to send the requests asap */
  adaptor_send_queue(zh, 0);
  return (rc < 0)?ReturnCode::MarshallingError:ReturnCode::Ok;
}

ReturnCode::type zoo_acreate(zhandle_t *zh, const std::string& path, const char *value,
        int valuelen, const std::vector<org::apache::zookeeper::data::ACL>& acl,
        int flags, string_completion_t completion, const void *data,
        bool isSynchronous) {
  LOG_DEBUG("Entering zoo_acreate()");
  std::string pathStr;
  ReturnCode::type rc = getRealString(zh, flags, path, pathStr);
  if (rc != ReturnCode::Ok) {
    return rc;
  }

  buffer_t* buffer = new buffer_t();
  StringOutStream stream(buffer->buffer);
  hadoop::OBinArchive oarchive(stream);

  proto::RequestHeader header;
  header.setxid(get_xid());
  header.settype(OpCode::Create);
  header.serialize(oarchive, "header");

  proto::CreateRequest req;
  req.getpath() = pathStr;
  if (value != NULL && valuelen >= 0) {
    req.getdata() = std::string(value, valuelen);
  } else {
    req.getdata() = "";
  }
  req.getacl() = acl;
  req.setflags(flags);
  req.serialize(oarchive, "req");

  {
    boost::lock_guard<boost::mutex> lock(zh->mutex);
    add_string_completion(zh, header.getxid(), completion, data, isSynchronous);
    queue_buffer(&zh->to_send, buffer);
  }

  LOG_DEBUG(boost::format("Sending a create request: path=[%s], server=%s, xid=%#08x") %
      path % format_current_endpoint_info(zh) % header.getxid());
  /* make a best (non-blocking) effort to send the requests asap */
  adaptor_send_queue(zh, 0);
  return (rc < 0)?ReturnCode::MarshallingError:ReturnCode::Ok;
}

int zoo_adelete(zhandle_t *zh, const std::string& path, int version,
        void_completion_t completion, const void *data, bool isSynchronous) {
  std::string pathStr;
  int rc = getRealString(zh, 0, path, pathStr);
  if (rc != ReturnCode::Ok) {
    return rc;
  }

  buffer_t* buffer = new buffer_t();
  StringOutStream stream(buffer->buffer);
  hadoop::OBinArchive oarchive(stream);

  proto::RequestHeader header;
  header.setxid(get_xid());
  header.settype(OpCode::Remove);
  header.serialize(oarchive, "header");

  proto::DeleteRequest req;
  req.getpath() = pathStr;
  req.setversion(version);
  req.serialize(oarchive, "req");

  {
    boost::lock_guard<boost::mutex> lock(zh->mutex);
    rc = rc < 0 ? rc : add_void_completion(zh, header.getxid(), completion, data,
        isSynchronous);
    queue_buffer(&zh->to_send, buffer);
  }

  LOG_DEBUG(boost::format("Sending request xid=%#08x for path [%s] to %s") %
      header.getxid() % path % format_current_endpoint_info(zh));
  /* make a best (non-blocking) effort to send the requests asap */
  adaptor_send_queue(zh, 0);
  return (rc < 0)?ReturnCode::MarshallingError:ReturnCode::Ok;
}

int zoo_awexists(zhandle_t *zh, const std::string& path,
                 boost::shared_ptr<Watch> watch,
                 stat_completion_t completion,
                 const void *data, bool isSynchronous) {
  std::string pathStr;
  int rc = getRealString(zh, 0, path, pathStr);
  if (rc != ReturnCode::Ok) {
    return rc;
  }

  buffer_t* buffer = new buffer_t();
  StringOutStream stream(buffer->buffer);
  hadoop::OBinArchive oarchive(stream);

  proto::RequestHeader header;
  header.setxid(get_xid());
  header.settype(OpCode::Exists);
  header.serialize(oarchive, "header");

  proto::ExistsRequest req;
  req.getpath() = pathStr;
  req.setwatch(watch.get() != NULL);
  req.serialize(oarchive, "req");

  WatchRegistration* reg = NULL;
  if (watch.get() != NULL) {
    LOG_DEBUG(boost::format("Adding an exists watch: %#08x") % header.getxid());
    reg = new ExistsWatchRegistration(zh->watchManager, req.getpath(), watch);
  }
  {
    boost::lock_guard<boost::mutex> lock(zh->mutex);
    rc = rc < 0 ? rc : add_stat_completion(zh, header.getxid(), completion,
        data, reg, isSynchronous);
    queue_buffer(&zh->to_send, buffer);
  }

  LOG_DEBUG(boost::format("Sending request xid=%#08x for path [%s] to %s") %
      header.getxid() % path % format_current_endpoint_info(zh));
  /* make a best (non-blocking) effort to send the requests asap */
  adaptor_send_queue(zh, 0);
  return (rc < 0)?ReturnCode::MarshallingError:ReturnCode::Ok;
}

int zoo_awget_children2(zhandle_t *zh, const std::string& path,
         boost::shared_ptr<Watch> watch,
         strings_stat_completion_t ssc, const void *data,
         bool isSynchronous) {
  std::string pathStr;
  int rc = getRealString(zh, 0, path, pathStr);
  if (rc != ReturnCode::Ok) {
    return rc;
  }

  buffer_t* buffer = new buffer_t();
  StringOutStream stream(buffer->buffer);
  hadoop::OBinArchive oarchive(stream);

  proto::RequestHeader header;
  header.setxid(get_xid());
  header.settype(OpCode::GetChildren2);
  header.serialize(oarchive, "header");

  proto::GetChildren2Request req;
  req.getpath() = pathStr;
  req.setwatch(watch.get() != NULL);
  req.serialize(oarchive, "req");

  WatchRegistration* reg = NULL;
  if (watch.get() != NULL) {
    reg = new GetChildrenWatchRegistration(zh->watchManager, req.getpath(),
                                           watch);
  }
  {
    boost::lock_guard<boost::mutex> lock(zh->mutex);
    rc = rc < 0 ? rc : add_strings_stat_completion(zh, header.getxid(), ssc,
        data, reg, isSynchronous);
    queue_buffer(&zh->to_send, buffer);
  }

  LOG_DEBUG(boost::format("Sending request xid=%#08x for path [%s] to %s") %
      header.getxid() % path % format_current_endpoint_info(zh));
  /* make a best (non-blocking) effort to send the requests asap */
  adaptor_send_queue(zh, 0);
  return (rc < 0)?ReturnCode::MarshallingError:ReturnCode::Ok;
}

int zoo_async(zhandle_t *zh, const std::string& path,
              string_completion_t completion, const void *data) {
  std::string pathStr;
  int rc = getRealString(zh, 0, path, pathStr);
  if (rc != ReturnCode::Ok) {
    return rc;
  }

  buffer_t* buffer = new buffer_t();
  StringOutStream stream(buffer->buffer);
  hadoop::OBinArchive oarchive(stream);

  proto::RequestHeader header;
  header.setxid(get_xid());
  header.settype(OpCode::Sync);
  header.serialize(oarchive, "header");

  proto::SyncRequest req;
  req.getpath() = pathStr;
  req.serialize(oarchive, "req");

  {
    boost::lock_guard<boost::mutex> lock(zh->mutex);
    rc = rc < 0 ? rc : add_string_completion(zh, header.getxid(), completion, data, false) ;
    queue_buffer(&zh->to_send, buffer);
  }

  LOG_DEBUG(boost::format("Sending request xid=%#08x for path [%s] to %s") %
      header.getxid() % path % format_current_endpoint_info(zh));
  /* make a best (non-blocking) effort to send the requests asap */
  adaptor_send_queue(zh, 0);
  return (rc < 0)?ReturnCode::MarshallingError:ReturnCode::Ok;

}

int zoo_aget_acl(zhandle_t *zh, const std::string& path, acl_completion_t completion,
        const void *data, bool isSynchronous) {
  std::string pathStr;
  int rc = getRealString(zh, 0, path, pathStr);
  if (rc != ReturnCode::Ok) {
    return rc;
  }

  buffer_t* buffer = new buffer_t();
  StringOutStream stream(buffer->buffer);
  hadoop::OBinArchive oarchive(stream);

  proto::RequestHeader header;
  header.setxid(get_xid());
  header.settype(OpCode::GetAcl);
  header.serialize(oarchive, "header");

  proto::GetACLRequest req;
  req.getpath() = pathStr;
  req.serialize(oarchive, "req");

  {
    boost::lock_guard<boost::mutex> lock(zh->mutex);
    rc = rc < 0 ? rc : add_acl_completion(zh, header.getxid(), completion, data,
        isSynchronous);
    queue_buffer(&zh->to_send, buffer);
  }

  LOG_DEBUG(boost::format("Sending request xid=%#08x for path [%s] to %s") %
      header.getxid() % path % format_current_endpoint_info(zh));
  /* make a best (non-blocking) effort to send the requests asap */
  adaptor_send_queue(zh, 0);
  return (rc < 0)?ReturnCode::MarshallingError:ReturnCode::Ok;
}

int zoo_aset_acl(zhandle_t *zh, const std::string& path, int version,
        const std::vector<data::ACL>& acl, void_completion_t completion,
        const void *data, bool isSynchronous) {
  std::string pathStr;
  int rc = getRealString(zh, 0, path, pathStr);
  if (rc != ReturnCode::Ok) {
    return rc;
  }

  buffer_t* buffer = new buffer_t();
  StringOutStream stream(buffer->buffer);
  hadoop::OBinArchive oarchive(stream);

  proto::RequestHeader header;
  header.setxid(get_xid());
  header.settype(OpCode::SetAcl);
  header.serialize(oarchive, "header");

  proto::SetACLRequest req;
  req.getpath() = pathStr;
  req.setversion(version);
  req.getacl() = acl;
  req.serialize(oarchive, "req");

  {
    boost::lock_guard<boost::mutex> lock(zh->mutex);
    rc = rc < 0 ? rc : add_void_completion(zh, header.getxid(), completion, data,
        isSynchronous);
    queue_buffer(&zh->to_send, buffer);
  }

  LOG_DEBUG(boost::format("Sending a set acl request xid=%#08x for path [%s] to %s") %
      header.getxid() % pathStr % format_current_endpoint_info(zh));
  /* make a best (non-blocking) effort to send the requests asap */
  adaptor_send_queue(zh, 0);
  return (rc < 0)?ReturnCode::MarshallingError:ReturnCode::Ok;
}

int zoo_amulti(zhandle_t *zh,
    const boost::ptr_vector<org::apache::zookeeper::Op>& ops,
    multi_completion_t completion, const void *data, bool isSynchronous) {
  buffer_t* buffer = new buffer_t();
  StringOutStream stream(buffer->buffer);
  hadoop::OBinArchive oarchive(stream);

  proto::RequestHeader header;
  header.setxid(get_xid());
  header.settype(OpCode::Multi);
  header.serialize(oarchive, "header");
  boost::ptr_vector<OpResult>* results = new boost::ptr_vector<OpResult>();

  size_t index = 0;
  for (index = 0; index < ops.size(); index++) {
    std::string pathStr;
    proto::MultiHeader mheader;
    mheader.settype(ops[index].getType());
    mheader.setdone(0);
    mheader.seterr(-1);
    mheader.serialize(oarchive, "req");

    int rc = getRealString(zh, 0, ops[index].getPath().c_str(), pathStr);
    if (rc != ReturnCode::Ok) {
      return rc;
    }

    switch(ops[index].getType()) {
      case OpCode::Create: {
        const Op::Create* createOp = dynamic_cast<const Op::Create*>(&(ops[index]));
        assert(createOp != NULL);
        proto::CreateRequest createReq;
        createReq.getpath() = pathStr;
        createReq.getdata() = createOp->getData();
        createReq.getacl() = createOp->getAcl();
        createReq.setflags(createOp->getMode());
        createReq.serialize(oarchive, "req");
        results->push_back(new OpResult::Create());
        break;
      }
      case OpCode::Remove: {
        const Op::Remove* removeOp = dynamic_cast<const Op::Remove*>(&(ops[index]));
        assert(removeOp != NULL);
        proto::DeleteRequest removeReq;
        removeReq.getpath() = pathStr;
        removeReq.setversion(removeOp->getVersion());
        removeReq.serialize(oarchive, "req");
        results->push_back(new OpResult::Remove());
        break;
      }

      case OpCode::SetData: {
        const Op::SetData* setDataOp = dynamic_cast<const Op::SetData*>(&(ops[index]));
        assert(setDataOp != NULL);
        proto::SetDataRequest setDataReq;
        setDataReq.getpath() = pathStr;
        setDataReq.getdata() = setDataOp->getData();
        setDataReq.setversion(setDataOp->getVersion());
        setDataReq.serialize(oarchive, "req");
        results->push_back(new OpResult::SetData());
        break;
     }

      case OpCode::Check: {
        const Op::Check* checkOp = dynamic_cast<const Op::Check*>(&(ops[index]));
        assert(checkOp != NULL);
        proto::CheckVersionRequest checkReq;
        checkReq.getpath() = pathStr;
        checkReq.setversion(checkOp->getVersion());
        checkReq.serialize(oarchive, "req");
        results->push_back(new OpResult::Check());
        break;
      }
      default:
        LOG_ERROR("Unimplemented sub-op type=" << ops[index].getType()
                                               << " in multi-op.");
        return ReturnCode::Unimplemented;
    }
  }

  // End of multi request.
  proto::MultiHeader mheader;
  mheader.settype(-1);
  mheader.setdone(1);
  mheader.seterr(-1);
  mheader.serialize(oarchive, "req");
  {
    boost::lock_guard<boost::mutex> lock(zh->mutex);
    add_multi_completion(zh, header.getxid(), completion, data, results, isSynchronous);
    queue_buffer(&zh->to_send, buffer);
  }

  LOG_DEBUG(boost::format("Sending multi request xid=%#08x with %d subrequests to %s") %
      header.getxid() % index % format_current_endpoint_info(zh));
  /* make a best (non-blocking) effort to send the requests asap */
  adaptor_send_queue(zh, 0);
  return ReturnCode::Ok;
}

/* specify timeout of 0 to make the function non-blocking */
/* timeout is in milliseconds */
ReturnCode::type
flush_send_queue(zhandle_t*zh, int timeout) {
  int rc;
  struct timeval started;
  gettimeofday(&started,0);
  // we can't use dequeue_buffer() here because if (non-blocking) send_buffer()
  // returns EWOULDBLOCK we'd have to put the buffer back on the queue.
  // we use a recursive lock instead and only dequeue the buffer if a send was
  // successful
  {
    boost::lock_guard<boost::recursive_mutex> lock(zh->to_send.mutex_);
    while (!(zh->to_send.bufferList_.empty()) &&
           zh->state == SessionState::Connected) {
      if(timeout != 0){
        int elapsed;
        struct timeval now;
        gettimeofday(&now,0);
        elapsed=calculate_interval(&started,&now);
        if (elapsed>timeout) {
          return ReturnCode::OperationTimeout;
        }

        struct pollfd fds;
        fds.fd = zh->fd;
        fds.events = POLLOUT;
        fds.revents = 0;
        rc = poll(&fds, 1, timeout-elapsed);
        if (rc <= 0) {
          /* timed out or an error or POLLERR */
          return rc == 0 ? ReturnCode::OperationTimeout : ReturnCode::SystemError;
        }
      }

      rc = send_buffer(zh->fd, &(zh->to_send.bufferList_.front()));
      if(rc == 0 && timeout == 0){
        /* send_buffer would block while sending this buffer */
        return ReturnCode::Ok;
      }
      if (rc < 0) {
        return ReturnCode::ConnectionLoss;
      }
      // if the buffer has been sent successfully, remove it from the queue
      if (rc > 0) {
        remove_buffer(&zh->to_send);
      }
      gettimeofday(&zh->last_send, 0);
    }
  }
  return ReturnCode::Ok;
}

// TODO(michim) handle synchronous
int zoo_add_auth(zhandle_t *zh,const char* scheme,const char* cert,
        int certLen,void_completion_t completion, const void *data,
        bool isSynchronous)
{
    auth_info *authinfo;
    if(scheme==NULL || zh==NULL)
        return ReturnCode::BadArguments;

    if (is_unrecoverable(zh))
        return ReturnCode::InvalidState;

    // [ZOOKEEPER-800] zoo_add_auth should return ReturnCode::InvalidState if
    // the connection is closed. 
    if (zoo_state(zh) == 0) {
        return ReturnCode::InvalidState;
    }

    assert(cert != NULL);
    assert(certLen >= 0);
    {
        boost::lock_guard<boost::mutex> lock(zh->mutex);
        authinfo = new auth_info();
        authinfo->scheme = scheme;
        authinfo->auth = std::string(cert, certLen);
        authinfo->completion=completion;
        authinfo->data=(const char*)data;
        zh->authList_.push_back(authinfo);
    }

    if(zh->state == SessionState::Connected) {
        return send_last_auth_info(zh);
    }

    return ReturnCode::Ok;
}

static const char* format_endpoint_info(const struct sockaddr_storage* ep)
{
    static char buf[128];
    char addrstr[128];
    void *inaddr;
    int port;
    if(ep==0)
        return "null";

#if defined(AF_INET6)
    if(ep->ss_family==AF_INET6){
        inaddr=&((struct sockaddr_in6*)ep)->sin6_addr;
        port=((struct sockaddr_in6*)ep)->sin6_port;
    } else {
#endif
        inaddr=&((struct sockaddr_in*)ep)->sin_addr;
        port=((struct sockaddr_in*)ep)->sin_port;
#if defined(AF_INET6)
    }
#endif
    inet_ntop(ep->ss_family,inaddr,addrstr,sizeof(addrstr)-1);
    sprintf(buf,"%s:%d",addrstr,ntohs(port));
    return buf;
}

static const char* format_current_endpoint_info(zhandle_t* zh)
{
    return format_endpoint_info(&zh->addrs[zh->connect_index]);
}

void zoo_deterministic_conn_order(int yesOrNo)
{
    disable_conn_permute=yesOrNo;
}
