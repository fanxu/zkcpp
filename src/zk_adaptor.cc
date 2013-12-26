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

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include "zk_adaptor.h"
#include "zookeeper/logging.hh"
ENABLE_LOGGING;

#include <boost/interprocess/detail/atomic.hpp>
#include <boost/thread.hpp>
#include <boost/version.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <fcntl.h>
#include <assert.h>
#include <errno.h>

#include <signal.h>
#include <poll.h>
#include <unistd.h>
#include <sys/time.h>

void do_io(zhandle_t* zh);
void do_completion(zhandle_t* zh);

ReturnCode::type
wakeup_io_thread(zhandle_t *zh) {
  char c = 0;
  return write(zh->threads.self_pipe[1],&c,1)==1?
    ReturnCode::Ok : ReturnCode::Error;
}

static int
set_nonblock(int fd) {
  long l = fcntl(fd, F_GETFL);
  if(l & O_NONBLOCK) return 0;
  return fcntl(fd, F_SETFL, l | O_NONBLOCK);
}

void
wait_for_others(zhandle_t* zh) {
  boost::unique_lock<boost::mutex> lock(zh->threads.lock);
  while(zh->threads.threadsToWait>0) {
    zh->threads.cond.wait(lock);
  }
}

void
notify_thread_ready(zhandle_t* zh) {
  boost::unique_lock<boost::mutex> lock(zh->threads.lock);
  zh->threads.threadsToWait--;
  zh->threads.cond.notify_all();
  while(zh->threads.threadsToWait>0) {
    zh->threads.cond.wait(lock);
  }
}

int
adaptor_init(zhandle_t *zh) {
  if(pipe(zh->threads.self_pipe)==-1) {
    LOG_ERROR("Can't make a pipe " << errno);
    return -1;
  }
  set_nonblock(zh->threads.self_pipe[1]);
  set_nonblock(zh->threads.self_pipe[0]);

  // start threads
  zh->threads.threadsToWait=2;  // wait for 2 threads before opening the barrier
  LOG_DEBUG("starting threads...");
  zh->threads.io = boost::thread(do_io, zh);
  zh->threads.completion = boost::thread(do_completion, zh);
  wait_for_others(zh);
  return 0;
}

int
adaptor_send_queue(zhandle_t *zh, int timeout) {
  if(!zh->close_requested)
    return wakeup_io_thread(zh);
  // don't rely on the IO thread to send the messages if the app has
  // requested to close
  return flush_send_queue(zh, timeout);
}

/* These two are declared here because we will run the event loop
 * and not the client */
int zookeeper_interest(zhandle_t *zh, int *fd, int *interest,
        struct timeval *tv);
int zookeeper_process(zhandle_t *zh, int events);

void
do_io(zhandle_t* zh) {
  struct pollfd fds[2];

  notify_thread_ready(zh);
  LOG_DEBUG("started IO thread");
  fds[0].fd=zh->threads.self_pipe[0];
  fds[0].events=POLLIN;
  while (!zh->close_requested) {
    struct timeval tv;
    int fd;
    int interest;
    int timeout;
    int maxfd=1;
    zookeeper_interest(zh, &fd, &interest, &tv);

    if (fd != -1) {
      fds[1].fd=fd;
      fds[1].events=(interest&ZOOKEEPER_READ)?POLLIN:0;
      fds[1].events|=(interest&ZOOKEEPER_WRITE)?POLLOUT:0;
      maxfd=2;
    }
    timeout=tv.tv_sec * 1000 + (tv.tv_usec/1000);

    poll(fds,maxfd,timeout);
    if (fd != -1) {
      interest=(fds[1].revents&POLLIN)?ZOOKEEPER_READ:0;
      interest|=((fds[1].revents&POLLOUT)||(fds[1].revents&POLLHUP))?ZOOKEEPER_WRITE:0;
    }
    if(fds[0].revents&POLLIN){
      // flush the pipe
      char b[128];
      while(read(zh->threads.self_pipe[0],b,sizeof(b))==sizeof(b)){}
    }
    // dispatch zookeeper events
    zookeeper_process(zh, interest);

    // check the current state of the zhandle and terminate
    // if it is_unrecoverable()
    if(is_unrecoverable(zh)) {
      break;
    }
  }
  LOG_DEBUG("IO thread terminated");
}

void
do_completion(zhandle_t* zh) {
  notify_thread_ready(zh);
  LOG_DEBUG("started completion thread");
  ReturnCode::type rc = ReturnCode::Ok;
  while(rc != ReturnCode::InvalidState) {
    boost::unique_lock<boost::mutex> lock(*(zh->completions_to_process.lock));
    while(zh->completions_to_process.completions.empty() &&
          !zh->close_requested) {
      (*(zh->completions_to_process.cond)).wait(lock);
    }
    lock.unlock();
    rc = process_completions(zh);
  }
  zh->threads.io.join();
  free_completions(zh, ReturnCode::InvalidState);
  process_completions(zh);
  LOG_DEBUG("completion thread terminated");
}

int32_t
get_xid() {
  static uint32_t xid = 0;
#if BOOST_VERSION / 100 % 1000 >= 46
  return boost::interprocess::ipcdetail::atomic_inc32(&xid);
#else
  return boost::interprocess::detail::atomic_inc32(&xid);
#endif
}
