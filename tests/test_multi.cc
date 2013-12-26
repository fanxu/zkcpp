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
#include <algorithm>
#include <boost/thread/condition.hpp>
#include <zookeeper/logging.hh>
#include <zookeeper/zookeeper.hh>
#include "zk_server.hh"
#include "zookeeper.h"

using namespace boost;
using namespace org::apache::zookeeper;
ENABLE_LOGGING;

/**
 * Test basic multi-op create functionality
 */
TEST(Multi, testCreate) {
  ZooKeeper zk;
  data::Stat stat;
  std::string pathCreated;
  std::vector<data::ACL> acl;
  data::ACL temp;
  temp.getid().getscheme() = "world";
  temp.getid().getid() = "anyone";
  temp.setperms(Permission::All);
  acl.push_back(temp);

  EXPECT_EQ(ReturnCode::Ok, zk.init(ZkServer::HOST_PORT, 30000,
        shared_ptr<Watch>()));

  boost::ptr_vector<Op> ops;
  ops.push_back(new Op::Create("/multi1", "", acl, CreateMode::Persistent));
  ops.push_back(new Op::Create("/multi1/a", "", acl, CreateMode::Persistent));
  ops.push_back(new Op::Create("/multi1/b", "", acl, CreateMode::Persistent));
  boost::ptr_vector<OpResult> results;
  EXPECT_EQ(ReturnCode::Ok, zk.multi(ops, results));
  EXPECT_EQ(3, (int)results.size());

  EXPECT_EQ(OpCode::Create, results[0].getType());
  OpResult::Create& res = dynamic_cast<OpResult::Create&>(results[0]);
  EXPECT_EQ(std::string("/multi1"), res.getPathCreated());
  EXPECT_EQ(ReturnCode::Ok, res.getReturnCode());

  EXPECT_EQ(OpCode::Create, results[1].getType());
  res = dynamic_cast<OpResult::Create&>(results[1]);
  EXPECT_EQ(std::string("/multi1/a"), res.getPathCreated());
  EXPECT_EQ(ReturnCode::Ok, res.getReturnCode());

  EXPECT_EQ(OpCode::Create, results[2].getType());
  res = dynamic_cast<OpResult::Create&>(results[2]);
  EXPECT_EQ(std::string("/multi1/b"), res.getPathCreated());
  EXPECT_EQ(ReturnCode::Ok, res.getReturnCode());
}

/**
 * Test create failure.
 */
TEST(Multi, testCreateFailure) {
  ZooKeeper zk;
  data::Stat stat;
  std::string pathCreated;
  std::vector<data::ACL> acl;
  data::ACL temp;
  temp.getid().getscheme() = "world";
  temp.getid().getid() = "anyone";
  temp.setperms(Permission::All);
  acl.push_back(temp);

  EXPECT_EQ(ReturnCode::Ok, zk.init(ZkServer::HOST_PORT, 30000,
        shared_ptr<Watch>()));

  boost::ptr_vector<Op> ops;
  ops.push_back(new Op::Create("/multi2", "", acl, CreateMode::Persistent));
  ops.push_back(new Op::Create("/multi2/a", "", acl, CreateMode::Persistent));
  ops.push_back(new Op::Create("/multi2/a", "", acl, CreateMode::Persistent));
  boost::ptr_vector<OpResult> results;
  EXPECT_EQ(ReturnCode::NodeExists, zk.multi(ops, results));
  EXPECT_EQ(3, (int)results.size());
}


/**
 * Test create followed by delete.
 */
TEST(Multi, testCreateDelete) {
  ZooKeeper zk;
  data::Stat stat;
  std::string pathCreated;
  std::vector<data::ACL> acl;
  data::ACL temp;
  temp.getid().getscheme() = "world";
  temp.getid().getid() = "anyone";
  temp.setperms(Permission::All);
  acl.push_back(temp);

  EXPECT_EQ(ReturnCode::Ok, zk.init(ZkServer::HOST_PORT, 30000,
        shared_ptr<Watch>()));

  boost::ptr_vector<Op> ops;
  ops.push_back(new Op::Create("/multi5", "", acl, CreateMode::Persistent));
  ops.push_back(new Op::Remove("/multi5", -1));
  boost::ptr_vector<OpResult> results;
  EXPECT_EQ(ReturnCode::Ok, zk.multi(ops, results));
  EXPECT_EQ(2, (int)results.size());

  // '/multi5' should have been deleted
  EXPECT_EQ(ReturnCode::NoNode, zk.exists("/multi5", boost::shared_ptr<Watch>(), stat));
}

/**
 * Test nested creates that rely on state in earlier op in multi
 */
TEST(Multi, testNestedCreate) {
  ZooKeeper zk;
  data::Stat stat;
  std::string pathCreated;
  std::vector<data::ACL> acl;
  data::ACL temp;
  temp.getid().getscheme() = "world";
  temp.getid().getid() = "anyone";
  temp.setperms(Permission::All);
  acl.push_back(temp);

  EXPECT_EQ(ReturnCode::Ok, zk.init(ZkServer::HOST_PORT, 30000,
        shared_ptr<Watch>()));

  boost::ptr_vector<Op> ops;
  ops.push_back(new Op::Create("/multi5", "", acl, CreateMode::Persistent));
  ops.push_back(new Op::Create("/multi5/a", "", acl, CreateMode::Persistent));
  ops.push_back(new Op::Create("/multi5/a/1", "", acl, CreateMode::Persistent));
  ops.push_back(new Op::Remove("/multi5/a/1", 0));
  ops.push_back(new Op::Remove("/multi5/a", 0));
  ops.push_back(new Op::Remove("/multi5", 0));

  boost::ptr_vector<OpResult> results;
  EXPECT_EQ(ReturnCode::Ok, zk.multi(ops, results));
  EXPECT_EQ(6, (int)results.size());

  // '/multi5' should have been deleted
  EXPECT_EQ(ReturnCode::NoNode,
      zk.exists("/multi5/a/1", boost::shared_ptr<Watch>(), stat));
  EXPECT_EQ(ReturnCode::NoNode,
      zk.exists("/multi5/a", boost::shared_ptr<Watch>(), stat));
  EXPECT_EQ(ReturnCode::NoNode,
      zk.exists("/multi5", boost::shared_ptr<Watch>(), stat));
}

/**
 * Test setdata functionality
 */
TEST(Multi, testSetData) {
  ZooKeeper zk;
  data::Stat stat;
  std::string pathCreated, data;
  std::vector<data::ACL> acl;
  data::ACL temp;
  temp.getid().getscheme() = "world";
  temp.getid().getid() = "anyone";
  temp.setperms(Permission::All);
  acl.push_back(temp);

  EXPECT_EQ(ReturnCode::Ok, zk.init(ZkServer::HOST_PORT, 30000,
        shared_ptr<Watch>()));

  boost::ptr_vector<Op> ops;
  ops.push_back(new Op::Create("/multi6", "", acl, CreateMode::Persistent));
  ops.push_back(new Op::Create("/multi6/a", "", acl, CreateMode::Persistent));
  boost::ptr_vector<OpResult> results;
  EXPECT_EQ(ReturnCode::Ok, zk.multi(ops, results));
  EXPECT_EQ(2, (int)results.size());

  ops.clear();
  ops.push_back(new Op::SetData("/multi6", "1", 0));
  ops.push_back(new Op::SetData("/multi6/a", "2", 0));
  EXPECT_EQ(ReturnCode::Ok, zk.multi(ops, results));
  EXPECT_EQ(2, (int)results.size());

  EXPECT_EQ(ReturnCode::Ok,
      zk.get("/multi6", boost::shared_ptr<Watch>(), data, stat));
  EXPECT_EQ(std::string("1"), data);
  EXPECT_EQ(ReturnCode::Ok,
      zk.get("/multi6/a", boost::shared_ptr<Watch>(), data, stat));
  EXPECT_EQ(std::string("2"), data);
}

/**
 * Test update conflicts
 */
TEST(Multi, testUpdateConflict) {
  ZooKeeper zk;
  data::Stat stat;
  std::string pathCreated, data;
  std::vector<data::ACL> acl;
  data::ACL temp;
  temp.getid().getscheme() = "world";
  temp.getid().getid() = "anyone";
  temp.setperms(Permission::All);
  acl.push_back(temp);

  EXPECT_EQ(ReturnCode::Ok, zk.init(ZkServer::HOST_PORT, 30000,
        shared_ptr<Watch>()));

  boost::ptr_vector<Op> ops;
  ops.push_back(new Op::Create("/multi7", "", acl, CreateMode::Persistent));
  ops.push_back(new Op::SetData("/multi7", "X", 0));
  ops.push_back(new Op::SetData("/multi7", "Y", 0));
  boost::ptr_vector<OpResult> results;
  EXPECT_EQ(ReturnCode::BadVersion, zk.multi(ops, results));
  EXPECT_EQ(3, (int)results.size());

  //Updating version solves conflict -- order matters
  dynamic_cast<Op::SetData&>(ops[2]).setVersion(1);
  EXPECT_EQ(ReturnCode::Ok, zk.multi(ops, results));
  EXPECT_EQ(3, (int)results.size());

  EXPECT_EQ(ReturnCode::Ok,
      zk.get("/multi7", boost::shared_ptr<Watch>(), data, stat));
  EXPECT_EQ(std::string("Y"), data);
}

/**
 * Test invalid versions
 */
TEST(Multi, testInvalidVersion) {
  ZooKeeper zk;
  data::Stat stat;
  std::string pathCreated;
  std::vector<data::ACL> acl;
  data::ACL temp;
  temp.getid().getscheme() = "world";
  temp.getid().getid() = "anyone";
  temp.setperms(Permission::All);
  acl.push_back(temp);

  EXPECT_EQ(ReturnCode::Ok, zk.init(ZkServer::HOST_PORT, 30000,
        shared_ptr<Watch>()));

  boost::ptr_vector<Op> ops;
  ops.push_back(new Op::Create("/multi3", "", acl, CreateMode::Persistent));
  ops.push_back(new Op::Remove("/multi3", 1));
  ops.push_back(new Op::Create("/multi3", "", acl, CreateMode::Persistent));
  ops.push_back(new Op::Create("/multi3/a", "", acl, CreateMode::Persistent));

  boost::ptr_vector<OpResult> results;
  EXPECT_EQ(ReturnCode::BadVersion, zk.multi(ops, results));
  EXPECT_EQ(4, (int)results.size());

  EXPECT_EQ(ReturnCode::Ok, results[0].getReturnCode());
  EXPECT_EQ(ReturnCode::BadVersion, results[1].getReturnCode());
  EXPECT_EQ(ReturnCode::RuntimeInconsistency,
      results[2].getReturnCode());
  EXPECT_EQ(ReturnCode::RuntimeInconsistency,
      results[3].getReturnCode());
}

/**
 * Test delete-update conflicts
 */
TEST(Multi, testDeleteUpdateConflict) {
  ZooKeeper zk;
  data::Stat stat;
  std::string pathCreated;
  std::vector<data::ACL> acl;
  data::ACL temp;
  temp.getid().getscheme() = "world";
  temp.getid().getid() = "anyone";
  temp.setperms(Permission::All);
  acl.push_back(temp);

  EXPECT_EQ(ReturnCode::Ok, zk.init(ZkServer::HOST_PORT, 30000,
        shared_ptr<Watch>()));

  boost::ptr_vector<Op> ops;
  ops.push_back(new Op::Create("/multi8", "", acl, CreateMode::Persistent));
  ops.push_back(new Op::Remove("/multi8", 0));
  ops.push_back(new Op::SetData("/multi8", "X", 0));

  boost::ptr_vector<OpResult> results;
  EXPECT_EQ(ReturnCode::NoNode, zk.multi(ops, results));
  EXPECT_EQ(3, (int)results.size());

  EXPECT_EQ(ReturnCode::Ok, results[0].getReturnCode());
  EXPECT_EQ(ReturnCode::Ok, results[1].getReturnCode());
  EXPECT_EQ(ReturnCode::NoNode, results[2].getReturnCode());

  // '/multi' should never have been created as entire op should fail
  EXPECT_EQ(ReturnCode::NoNode,
      zk.exists("/multi8", boost::shared_ptr<Watch>(), stat));
}

/**
 * Test basic multi-op check functionality
 */
TEST(Multi, testCheck) {
  ZooKeeper zk;
  data::Stat stat;
  std::string pathCreated;
  std::vector<data::ACL> acl;
  data::ACL temp;
  temp.getid().getscheme() = "world";
  temp.getid().getid() = "anyone";
  temp.setperms(Permission::All);
  acl.push_back(temp);

  EXPECT_EQ(ReturnCode::Ok, zk.init(ZkServer::HOST_PORT, 30000,
        shared_ptr<Watch>()));
  LOG_DEBUG("INIT DONE");
  EXPECT_EQ(ReturnCode::Ok,
      zk.create("/multi0", "", acl, CreateMode::Persistent, pathCreated));
  LOG_DEBUG("CREATE DONE");

  // Conditionally create /multi0/a' only if '/multi0' at version 0
  boost::ptr_vector<Op> ops;
  ops.push_back(new Op::Check("/multi0", 0));
  ops.push_back(new Op::Create("/multi0/a", "", acl, CreateMode::Persistent));
  boost::ptr_vector<OpResult> results;
  EXPECT_EQ(ReturnCode::Ok, zk.multi(ops, results));
  EXPECT_EQ(2, (int)results.size());
  EXPECT_EQ(ReturnCode::Ok, results[0].getReturnCode());
  EXPECT_EQ(ReturnCode::Ok, results[1].getReturnCode());

  // '/multi0/a' should have been created as it passed version check
  EXPECT_EQ(ReturnCode::Ok,
      zk.exists("/multi0/a", boost::shared_ptr<Watch>(), stat));

  // Only create '/multi0/b' if '/multi0' at version 10 (which it's not)
  ops.clear();
  ops.push_back(new Op::Check("/multi0", 10));
  ops.push_back(new Op::Create("/multi0/b", "", acl, CreateMode::Persistent));
  EXPECT_EQ(ReturnCode::BadVersion, zk.multi(ops, results));
  EXPECT_EQ(2, (int)results.size());
  EXPECT_EQ(ReturnCode::BadVersion, results[0].getReturnCode());
  EXPECT_EQ(ReturnCode::RuntimeInconsistency,
      results[1].getReturnCode());

  // '/multi0/b' should NOT have been created
  EXPECT_EQ(ReturnCode::NoNode,
      zk.exists("/multi0/b", boost::shared_ptr<Watch>(), stat));
}

class MultiWatch : public Watch {
  public:
    ZooKeeper& zk_;
    MultiWatch(ZooKeeper& zk) : zk_(zk) {}
    void process(WatchEvent::type event, SessionState::type state,
        const std::string& path) {
      std::string data;
      data::Stat stat;
      boost::ptr_vector<Op> ops;
      ops.push_back(new Op::SetData("/multiwatch", "X", 1));
      boost::ptr_vector<OpResult> results;
      EXPECT_EQ(ReturnCode::Ok, zk_.multi(ops, results));
      EXPECT_EQ(1, (int)results.size());
      EXPECT_EQ(ReturnCode::Ok, results[0].getReturnCode());
      EXPECT_EQ(ReturnCode::Ok,
          zk_.get("/multiwatch", boost::shared_ptr<Watch>(), data, stat));
      EXPECT_EQ(std::string("X"), data);
    }
};

/**
 * Test multi-op called from a watch
 */
TEST(Multi, testWatch) {
  ZooKeeper zk;
  std::string data;
  data::Stat stat;
  std::string pathCreated;
  std::vector<data::ACL> acl;
  data::ACL temp;
  temp.getid().getscheme() = "world";
  temp.getid().getid() = "anyone";
  temp.setperms(Permission::All);
  acl.push_back(temp);

  EXPECT_EQ(ReturnCode::Ok, zk.init(ZkServer::HOST_PORT, 30000,
        shared_ptr<Watch>()));
  EXPECT_EQ(ReturnCode::Ok,
      zk.create("/multiwatch", "", acl, CreateMode::Persistent, pathCreated));

  boost::shared_ptr<MultiWatch> multiWatch(new MultiWatch(zk));
  EXPECT_EQ(ReturnCode::Ok, zk.get("/multiwatch", multiWatch, data, stat));

  // setdata on node '/multiwatch' this should trip the watch
  EXPECT_EQ(ReturnCode::Ok,
      zk.set("/multiwatch", "", -1, stat));
  sleep(1);
}
