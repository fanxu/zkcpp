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

#include <gtest/gtest.h>
#include <boost/thread/condition.hpp>
#include <zookeeper/zookeeper.hh>
#include <zookeeper/logging.hh>
ENABLE_LOGGING;

#include "zk_server.hh"

using namespace boost;
using namespace org::apache::zookeeper;

class TestWatch : public Watch {
  public:
    TestWatch() : created(false) {}
    void process(WatchEvent::type event, SessionState::type state,
                 const std::string& path) {
      if (event == WatchEvent::ZnodeCreated) {
        boost::lock_guard<boost::mutex> lock(mutex);
        created = true;
        cond.notify_all();
      }
    }

    bool waitForCreated(uint32_t timeoutMs) {
      boost::system_time const timeout=boost::get_system_time() +
        boost::posix_time::milliseconds(timeoutMs);

      boost::mutex::scoped_lock lock(mutex);
      while (!created) {
        if(!cond.timed_wait(lock,timeout)) {
          return false;
        }
      }
      return true;
    }

    boost::condition_variable cond;
    boost::mutex mutex;
    bool created;
};

TEST(Watch, testExists) {
  ZooKeeper zk, zk2;
  data::Stat stat;
  std::string pathCreated;
  std::vector<data::ACL> acl;
  data::ACL temp;
  temp.getid().getscheme() = "world";
  temp.getid().getid() = "anyone";
  temp.setperms(Permission::All);
  acl.push_back(temp);

  EXPECT_EQ(ReturnCode::Ok,
            zk.init(ZkServer::HOST_PORT, 30000, shared_ptr<Watch>()));

  // This should leave a watch on the znode.
  shared_ptr<TestWatch> watch(new TestWatch());
  ReturnCode::type rc = zk.exists("/testExists", watch, stat);
  EXPECT_EQ(ReturnCode::NoNode, rc);

  rc = zk.create("/testExists", "data",  acl, CreateMode::Persistent,
                 pathCreated);
  EXPECT_EQ(ReturnCode::Ok, rc);
  EXPECT_EQ(std::string("/testExists"), pathCreated);
  EXPECT_TRUE(watch->waitForCreated(1000));
}
