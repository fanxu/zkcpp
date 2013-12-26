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
#include "zookeeper_impl.hh"
#include "zk_adaptor.h"
#include <boost/thread/condition.hpp>
#include <cerrno>
#include <cstring>
#include "zookeeper/logging.hh"
ENABLE_LOGGING;

namespace org { namespace apache { namespace zookeeper {

class Waitable {
  public:
    Waitable() : completed_(false) {}

    void notifyCompleted() {
      {
        boost::lock_guard<boost::mutex> lock(mutex_);
        completed_ = true;
      }
      cond_.notify_all();
    }

    void waitForCompleted() {
      boost::unique_lock<boost::mutex> lock(mutex_);
      while (!completed_) {
        cond_.wait(lock);
      }
    }

  private:
    boost::condition_variable cond_;
    boost::mutex mutex_;
    bool completed_;
};

class MyAddAuthCallback : public AddAuthCallback, public Waitable {
  public:
    MyAddAuthCallback(const std::string& scheme, const std::string& cert) :
                     scheme_(scheme), cert_(cert) {}

    void process(ReturnCode::type rc,
                const std::string& scheme,
                const std::string& cert) {
      assert(scheme == scheme_);
      assert(cert == cert_);
      rc_ = rc;
      notifyCompleted();
    }

    ReturnCode::type rc_;
    const std::string& scheme_;
    const std::string& cert_;
};

class MyCreateCallback : public CreateCallback, public Waitable {
  public:
    MyCreateCallback(const std::string& pathRequested,
                     std::string& pathCreated) :
                     pathRequested_(pathRequested),
                     pathCreated_(pathCreated) {
    }

    void process(ReturnCode::type rc,
                const std::string& pathRequested,
                const std::string& pathCreated) {
      LOG_DEBUG(boost::format("rc=%s pathRequested='%s' pathCreated='%s'") %
                ReturnCode::toString(rc) % pathRequested % pathCreated);
      assert(pathRequested_ == pathRequested);
      rc_ = rc;
      if (rc == ReturnCode::Ok) {
        pathCreated_ = pathCreated;
      }
      notifyCompleted();
    }

    ReturnCode::type rc_;
    const std::string& pathRequested_;
    std::string& pathCreated_;
};

class MyExistsCallback : public ExistsCallback, public Waitable {
  public:
    MyExistsCallback(data::Stat& stat) : stat_(stat) {}
    void process(ReturnCode::type rc, const std::string& path,
                 const data::Stat& stat) {
      if (rc == ReturnCode::Ok) {
        stat_ = stat;
      }
      rc_ = rc;
      stat_ = stat;
      path_ = path;
      notifyCompleted();
    }

    ReturnCode::type rc_;
    std::string path_;
    data::Stat& stat_;
};

class MyGetCallback : public GetCallback, public Waitable {
  public:
    MyGetCallback(std::string& data, data::Stat& stat) :
      data_(data), stat_(stat) {}
    void process(ReturnCode::type rc, const std::string& path,
                 const std::string& data, const data::Stat& stat) {
      if (rc == ReturnCode::Ok) {
        data_ = data;
        stat_ = stat;
      }
      rc_ = rc;
      path_ = path;
      notifyCompleted();
    }

    ReturnCode::type rc_;
    std::string path_;
    std::string& data_;
    data::Stat& stat_;
};

class MyGetAclCallback : public GetAclCallback, public Waitable {
  public:
    MyGetAclCallback(std::vector<data::ACL>& acl, data::Stat& stat) :
      acl_(acl), stat_(stat) {}

    void process(ReturnCode::type rc, const std::string& path,
                 const std::vector<data::ACL>& acl,
                 const data::Stat& stat) {
      if (rc == ReturnCode::Ok) {
        acl_ = acl;
        stat_ = stat;
      }
      rc_ = rc;
      path_ = path;
      notifyCompleted();
    }

    ReturnCode::type rc_;
    std::string path_;
    std::vector<data::ACL>& acl_;
    data::Stat& stat_;
};

class MySetAclCallback : public SetAclCallback, public Waitable {
  public:
    MySetAclCallback() {}

    void process(ReturnCode::type rc, const std::string& path) {
      rc_ = rc;
      path_ = path;
      notifyCompleted();
    }

    ReturnCode::type rc_;
    std::string path_;
};

class MyGetChildrenCallback : public GetChildrenCallback, public Waitable {
  public:
    MyGetChildrenCallback(std::vector<std::string>& children, data::Stat& stat) :
      children_(children), stat_(stat) {}

    void process(ReturnCode::type rc, const std::string& path,
                 const std::vector<std::string>& children,
                 const data::Stat& stat) {
      if (rc == ReturnCode::Ok) {
        children_ = children;
        stat_ = stat;
      }
      rc_ = rc;
      path_ = path;
      notifyCompleted();
    }

    ReturnCode::type rc_;
    std::string path_;
    std::vector<std::string>& children_;
    data::Stat& stat_;
};


class MySetCallback : public SetCallback, public Waitable {
  public:
    MySetCallback(data::Stat& stat) : stat_(stat) {}
    void process(ReturnCode::type rc, const std::string& path,
                 const data::Stat& stat) {
      if (rc == ReturnCode::Ok) {
        stat_ = stat;
      }
      rc_ = rc;
      path_ = path;
      notifyCompleted();
    }

    ReturnCode::type rc_;
    std::string path_;
    data::Stat& stat_;
};

class MyRemoveCallback : public RemoveCallback, public Waitable {
  public:
    MyRemoveCallback() {}
    void process(ReturnCode::type rc, const std::string& path) {
      rc_ = rc;
      path_ = path;
      notifyCompleted();
    }

    ReturnCode::type rc_;
    std::string path_;
};

class MyMultiCallback : public MultiCallback, public Waitable {
  public:
    MyMultiCallback(boost::ptr_vector<OpResult>& results) :
      results_(results) {}
    void process(ReturnCode::type rc,
                 const boost::ptr_vector<OpResult>& results) {
      boost::ptr_vector<OpResult>& res = (boost::ptr_vector<OpResult>&)results;
      results_.clear();
      while (res.begin() != res.end()) {
        results_.push_back(res.release(res.begin()).release());
      }
      rc_ = rc;
      notifyCompleted();
    }

    ReturnCode::type rc_;
    boost::ptr_vector<OpResult>& results_;
};

class CompletionContext {
  public:
    CompletionContext(boost::shared_ptr<void> callback,
                    std::string path) : callback_(callback), path_(path) {}
    boost::shared_ptr<void> callback_;
    std::string path_;
};

class AuthCompletionContext {
  public:
    AuthCompletionContext(boost::shared_ptr<void> callback,
                          const std::string& scheme,
                          const std::string& cert) :
      callback_(callback), scheme_(scheme), cert_(cert) {}
    boost::shared_ptr<void> callback_;
    std::string scheme_;
    std::string cert_;
};

class MultiCompletionContext {
  public:
    MultiCompletionContext(boost::shared_ptr<void> callback) :
      callback_(callback) {}
    boost::shared_ptr<void> callback_;
};

void ZooKeeperImpl::
stringCompletion(int rc, const std::string& value, const void* data) {
  CompletionContext* context = (CompletionContext*)data;
  CreateCallback* callback = (CreateCallback*)context->callback_.get();
  if (callback) {
    callback->process((ReturnCode::type)rc, context->path_, value);
  }
  delete context;
}

void ZooKeeperImpl::
removeCompletion(int rc, const void* data) {
  CompletionContext* context = (CompletionContext*)data;
  RemoveCallback* callback = (RemoveCallback*)context->callback_.get();
  if (callback) {
    callback->process((ReturnCode::type)rc, context->path_);
  }
  delete context;
}

void ZooKeeperImpl::
setAclCompletion(int rc, const void* data) {
  CompletionContext* context = (CompletionContext*)data;
  SetAclCallback* callback = (SetAclCallback*)context->callback_.get();
  if (callback) {
    callback->process((ReturnCode::type)rc, context->path_);
  }
  delete context;
}

void ZooKeeperImpl::
existsCompletion(int rc, const data::Stat& stat,
                           const void* data) {
  CompletionContext* context = (CompletionContext*)data;
  ExistsCallback* callback = (ExistsCallback*)context->callback_.get();
  if (callback) {
    callback->process((ReturnCode::type)rc, context->path_, stat);
  }
  delete context;
}

void ZooKeeperImpl::
setCompletion(int rc, const data::Stat& stat, const void* data) {
  CompletionContext* context = (CompletionContext*)data;
  SetCallback* callback = (SetCallback*)context->callback_.get();
  if (callback) {
    callback->process((ReturnCode::type)rc, context->path_, stat);
  }
  delete context;
}

void ZooKeeperImpl::
dataCompletion(int rc, const std::string& value,
               const data::Stat& stat, const void *data) {
  CompletionContext* context = (CompletionContext*)data;
  std::string result;
  GetCallback* callback = (GetCallback*)context->callback_.get();
  if (callback) {
    callback->process((ReturnCode::type)rc, context->path_, value, stat);
  }
  delete context;
}

void ZooKeeperImpl::
childrenCompletion(int rc, const std::vector<std::string>& children,
                   const data::Stat& stat, const void *data) {
  CompletionContext* context = (CompletionContext*)data;
  LOG_DEBUG("getChildren() for " << context->path_ << " returned: " <<
            ReturnCode::toString((ReturnCode::type)rc));
  GetChildrenCallback* callback =
    (GetChildrenCallback*)context->callback_.get();
  assert(callback);
  callback->process((ReturnCode::type)rc, context->path_, children, stat);
  delete context;
}

void ZooKeeperImpl::
aclCompletion(int rc, const std::vector<data::ACL>& acl,
              const data::Stat& stat, const void *data) {
  CompletionContext* context = (CompletionContext*)data;
  LOG_DEBUG("getAcl() for " << context->path_ << " returned: " <<
            ReturnCode::toString((ReturnCode::type)rc));
  GetAclCallback* callback = (GetAclCallback*)context->callback_.get();
  assert(callback);
  if (rc == ReturnCode::Ok) {
    for (size_t i = 0; i < acl.size(); i++) {
      LOG_DEBUG(
        boost::format("Got an ACL for %s: %s:%s:'%s'") %
                      context->path_ % acl[i].getid().getscheme() %
                      acl[i].getid().getid() %
                      Permission::toString(acl[i].getperms()));
    }
  }
  callback->process((ReturnCode::type)rc, context->path_, acl, stat);
  delete context;
}

void ZooKeeperImpl::
authCompletion(int rc, const void* data) {
  AuthCompletionContext* context = (AuthCompletionContext*)data;
  AddAuthCallback* callback = (AddAuthCallback*)context->callback_.get();
  LOG_DEBUG(boost::format("rc=%d, scheme='%s', cert='%s'") % rc %
                          context->scheme_.c_str() % context->cert_.c_str());
  assert(callback);
  callback->process((ReturnCode::type)rc, context->scheme_, context->cert_);
  delete context;
}

void ZooKeeperImpl::
syncCompletion(int rc, const std::string& value, const void* data) {
  CompletionContext* context = (CompletionContext*)data;
  std::string result;
  SyncCallback* callback = (SyncCallback*)context->callback_.get();
  if (callback) {
    callback->process((ReturnCode::type)rc, context->path_);
  }
  delete context;
}

void ZooKeeperImpl::
multiCompletion(int rc, const boost::ptr_vector<OpResult>& results,
                const void *data) {
  MultiCompletionContext* context = (MultiCompletionContext*)data;
  MultiCallback* callback = (MultiCallback*)context->callback_.get();
  if (callback) {
    callback->process((ReturnCode::type)rc, results);
  }
  delete context;
}

ZooKeeperImpl::
ZooKeeperImpl() : handle_(NULL), inited_(false), state_(SessionState::Expired) {
}

ZooKeeperImpl::
~ZooKeeperImpl() {
  close();
}

ReturnCode::type ZooKeeperImpl::
init(const std::string& hosts, int32_t sessionTimeoutMs,
     boost::shared_ptr<Watch> watch) {
  handle_ = zookeeper_init(hosts.c_str(), watch, sessionTimeoutMs, 0);
  if (handle_ == NULL) {
    return ReturnCode::Error;
  }
  inited_ = true;
  return ReturnCode::Ok;
}

ReturnCode::type ZooKeeperImpl::
addAuth(const std::string& scheme, const std::string& cert,
        boost::shared_ptr<AddAuthCallback> callback,
        bool isSynchronous) {
  void_completion_t completion = NULL;
  AuthCompletionContext* context = NULL;
  if (callback.get()) {
    completion = &authCompletion;
    context = new AuthCompletionContext(callback, scheme, cert);
  }
  int rc = zoo_add_auth(handle_, scheme.c_str(), cert.c_str(),
                        cert.size(), completion, (void*)context,
                        isSynchronous);
  return (ReturnCode::type)rc;
}

ReturnCode::type ZooKeeperImpl::
addAuth(const std::string& scheme, const std::string& cert) {
  boost::shared_ptr<MyAddAuthCallback> callback(
    new MyAddAuthCallback(scheme, cert));
  ReturnCode::type rc = addAuth(scheme, cert, callback, true);
  if (rc != ReturnCode::Ok) {
    return rc;
  }
  callback->waitForCompleted();
  return callback->rc_;
}


ReturnCode::type ZooKeeperImpl::
create(const std::string& path, const std::string& data,
                  const std::vector<data::ACL>& acl, CreateMode::type mode,
                  boost::shared_ptr<CreateCallback> callback,
                  bool isSynchronous) {
  string_completion_t completion = NULL;
  CompletionContext* context = NULL;
  if (callback.get()) {
    completion = &stringCompletion;
    context = new CompletionContext(callback, path);
  }
  ReturnCode::type rc = zoo_acreate(handle_, path.c_str(), data.c_str(), data.size(),
                       acl, mode, completion, (void*)context, isSynchronous);
  if (rc != ReturnCode::Ok) {
    delete context;
  }
  return (ReturnCode::type)rc;
}

ReturnCode::type ZooKeeperImpl::
create(const std::string& path, const std::string& data,
       const std::vector<data::ACL>& acl, CreateMode::type mode,
       std::string& pathCreated) {
  LOG_DEBUG("Entering create()");
  boost::shared_ptr<MyCreateCallback> callback(
    new MyCreateCallback(path, pathCreated));
  ReturnCode::type rc = create(path, data, acl, mode, callback, true);
  if (rc != ReturnCode::Ok) {
    return rc;
  }
  LOG_DEBUG("wait for callback: create()");
  callback->waitForCompleted();
  return callback->rc_;
}

ReturnCode::type ZooKeeperImpl::
remove(const std::string& path, int32_t version,
       boost::shared_ptr<RemoveCallback> callback,
       bool isSynchronous) {
  void_completion_t completion = NULL;
  CompletionContext* context = NULL;
  if (callback.get()) {
    completion = &removeCompletion;
    context = new CompletionContext(callback, path);
  }
  int rc = zoo_adelete(handle_, path.c_str(), version,
         completion, (void*)context, isSynchronous);
  return (ReturnCode::type)rc;
}

ReturnCode::type ZooKeeperImpl::
remove(const std::string& path, int32_t version) {
  boost::shared_ptr<MyRemoveCallback> callback(new MyRemoveCallback());
  ReturnCode::type rc = remove(path, version, callback, true);
  if (rc != ReturnCode::Ok) {
    return rc;
  }
  callback->waitForCompleted();
  return callback->rc_;
}

ReturnCode::type ZooKeeperImpl::
exists(const std::string& path, boost::shared_ptr<Watch> watch,
       boost::shared_ptr<ExistsCallback> cb,
       bool isSynchronous) {
  stat_completion_t completion = NULL;
  CompletionContext* completionContext = NULL;

  if (cb.get()) {
    completion = &existsCompletion;
    completionContext = new CompletionContext(cb, path);
  }
  int rc = zoo_awexists(handle_, path.c_str(), watch,
                        completion,  (void*)completionContext, isSynchronous);
  return (ReturnCode::type)rc;
}

ReturnCode::type ZooKeeperImpl::
exists(const std::string& path, boost::shared_ptr<Watch> watch,
       data::Stat& stat) {
  boost::shared_ptr<MyExistsCallback> callback(new MyExistsCallback(stat));
  ReturnCode::type rc = exists(path, watch, callback, true);
  if (rc != ReturnCode::Ok) {
    return rc;
  }
  callback->waitForCompleted();
  return callback->rc_;
}

ReturnCode::type ZooKeeperImpl::
get(const std::string& path, boost::shared_ptr<Watch> watch,
    boost::shared_ptr<GetCallback> cb,
    bool isSynchronous) {
  data_completion_t completion = NULL;
  CompletionContext* context = NULL;

  if (cb.get()) {
    completion = &dataCompletion;
    context = new CompletionContext(cb, path);
  }

  int rc = zoo_awget(handle_, path.c_str(), watch,
                    completion, (void*)context, isSynchronous);
  return (ReturnCode::type)rc;
}

ReturnCode::type ZooKeeperImpl::
get(const std::string& path, boost::shared_ptr<Watch> watch,
    std::string& data, data::Stat& stat) {
  boost::shared_ptr<MyGetCallback> callback(new MyGetCallback(data, stat));
  ReturnCode::type rc = get(path, watch, callback, true);
  if (rc != ReturnCode::Ok) {
    return rc;
  }
  callback->waitForCompleted();
  return callback->rc_;
}

ReturnCode::type ZooKeeperImpl::
set(const std::string& path, const std::string& data,
               int32_t version, boost::shared_ptr<SetCallback> cb,
               bool isSynchronous) {
  stat_completion_t completion = NULL;
  CompletionContext* context = NULL;
  if (cb.get()) {
    completion = &setCompletion;
    context = new CompletionContext(cb, path);
  }

  int rc = zoo_aset(handle_, path.c_str(), data.c_str(), data.size(), version,
                    completion, (void*)context, isSynchronous);
  return (ReturnCode::type)rc;
}


ReturnCode::type ZooKeeperImpl::
set(const std::string& path, const std::string& data,
    int32_t version, data::Stat& stat) {
  boost::shared_ptr<MySetCallback> callback(new MySetCallback(stat));
  ReturnCode::type rc = set(path, data, version, callback, true);
  if (rc != ReturnCode::Ok) {
    return rc;
  }
  callback->waitForCompleted();
  assert(path == callback->path_);
  return callback->rc_;
}

ReturnCode::type ZooKeeperImpl::
getChildren(const std::string& path, boost::shared_ptr<Watch> watch,
            boost::shared_ptr<GetChildrenCallback> cb,
            bool isSynchronous) {
  strings_stat_completion_t completion = NULL;
  CompletionContext* context = NULL;

  if (cb.get()) {
    completion = &childrenCompletion;
    context = new CompletionContext(cb, path);
  }

  int rc = zoo_awget_children2(handle_, path.c_str(), watch,
                               completion, (void*)context,
                               isSynchronous);
  return (ReturnCode::type)rc;
}

ReturnCode::type ZooKeeperImpl::
getChildren(const std::string& path, boost::shared_ptr<Watch> watch,
            std::vector<std::string>& children, data::Stat& stat) {
  boost::shared_ptr<MyGetChildrenCallback>
    callback(new MyGetChildrenCallback(children, stat));
  ReturnCode::type rc = getChildren(path, watch, callback, true);
  if (rc != ReturnCode::Ok) {
    return rc;
  }
  callback->waitForCompleted();
  assert(path == callback->path_);
  return callback->rc_;
}

ReturnCode::type ZooKeeperImpl::
getAcl(const std::string& path, boost::shared_ptr<GetAclCallback> cb,
       bool isSynchronous) {
  acl_completion_t completion = NULL;
  CompletionContext* context = NULL;
  if (cb.get()) {
    completion = &aclCompletion;
    context = new CompletionContext(cb, path);
  }
  int rc = zoo_aget_acl(handle_, path.c_str(), completion, (void*)context,
                        isSynchronous);
  return (ReturnCode::type)rc;
}


ReturnCode::type ZooKeeperImpl::
getAcl(const std::string& path,
       std::vector<data::ACL>& acl, data::Stat& stat) {
  boost::shared_ptr<MyGetAclCallback> callback(new MyGetAclCallback(acl, stat));
  ReturnCode::type rc = getAcl(path, callback, true);
  if (rc != ReturnCode::Ok) {
    return rc;
  }
  callback->waitForCompleted();
  assert(path == callback->path_);
  return callback->rc_;
}

ReturnCode::type ZooKeeperImpl::
setAcl(const std::string& path, int32_t version,
       const std::vector<data::ACL>& acl,
       boost::shared_ptr<SetAclCallback> cb,
       bool isSynchronous) {
  void_completion_t completion = NULL;
  CompletionContext* context = NULL;
  if (cb.get()) {
    completion = &setAclCompletion;
    context = new CompletionContext(cb, path);
  }

  int rc = zoo_aset_acl(handle_, path.c_str(), version, acl, completion,
                         (void*)context, isSynchronous);
  return (ReturnCode::type)rc;
}

ReturnCode::type ZooKeeperImpl::
setAcl(const std::string& path, int32_t version,
       const std::vector<data::ACL>& acl) {
  boost::shared_ptr<MySetAclCallback> callback(new MySetAclCallback());
  ReturnCode::type rc = setAcl(path, version, acl, callback, true);
  if (rc != ReturnCode::Ok) {
    return rc;
  }
  callback->waitForCompleted();
  assert(path == callback->path_);
  return callback->rc_;
}

ReturnCode::type ZooKeeperImpl::
sync(const std::string& path, boost::shared_ptr<SyncCallback> cb) {
  string_completion_t completion = NULL;
  CompletionContext* context = NULL;
  if (cb.get()) {
    completion = &syncCompletion;
    context = new CompletionContext(cb, path);
  }
  return (ReturnCode::type)zoo_async(handle_, path.c_str(),
         completion, context);
}

ReturnCode::type ZooKeeperImpl::
multi(const boost::ptr_vector<Op>& ops,
      boost::shared_ptr<MultiCallback> cb,
      bool isSynchronous) {
  multi_completion_t completion = NULL;
  MultiCompletionContext* context = NULL;
  if (cb.get()) {
    completion = &multiCompletion;
    context = new MultiCompletionContext(cb);
  }
  int rc = zoo_amulti(handle_, ops, completion, context, isSynchronous);
  return (ReturnCode::type)rc;
}

ReturnCode::type ZooKeeperImpl::
multi(const boost::ptr_vector<Op>& ops,
      boost::ptr_vector<OpResult>& results) {
  boost::shared_ptr<MyMultiCallback> callback(new MyMultiCallback(results));
  ReturnCode::type rc = multi(ops, callback, true);
  if (rc != ReturnCode::Ok) {
    return rc;
  }
  callback->waitForCompleted();
  return callback->rc_;
}

ReturnCode::type ZooKeeperImpl::
close() {
  if (!inited_) {
    return ReturnCode::Error;
  }
  inited_ = false;
  // XXX handle return codes.
  zookeeper_close(handle_);
  handle_ = NULL;
  return ReturnCode::Ok;
}

SessionState::type ZooKeeperImpl::
getState() {
  if (!inited_) {
    return SessionState::Expired;
  }
  return zoo_state(handle_);
}

void ZooKeeperImpl::
setState(SessionState::type state) {
  state_ = state;
}

}}}  // namespace org::apache::zookeeper
