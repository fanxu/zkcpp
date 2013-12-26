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

class TestInitWatch : public Watch {
  public:
    TestInitWatch() : connected(false), connecting(false), authFailed(false) {}
    void process(WatchEvent::type event, SessionState::type state,
        const std::string& path) {
      LOG_DEBUG("session state: " << SessionState::toString(state));
      if (event == WatchEvent::SessionStateChanged) {
        if (state == SessionState::Connected) {
          {
            boost::lock_guard<boost::mutex> lock(mutex);
            connected = true;
          }
          cond.notify_all();
        } else if (state == SessionState::Connecting) {
          {
            boost::lock_guard<boost::mutex> lock(mutex);
            connecting = true;
          }
          cond.notify_all();
        } else if (state == SessionState::AuthFailed) {
          {
            boost::lock_guard<boost::mutex> lock(mutex);
            authFailed = true;
          }
          cond.notify_all();
        }
      }
    }

    bool waitForConnected(uint32_t timeoutMs) {
      boost::system_time const timeout=boost::get_system_time() +
        boost::posix_time::milliseconds(timeoutMs);

      boost::mutex::scoped_lock lock(mutex);
      while (!connected) {
        if(!cond.timed_wait(lock,timeout)) {
          return false;
        }
      }
      return true;
    }

    bool waitForConnecting(uint32_t timeoutMs) {
      boost::system_time const timeout=boost::get_system_time() +
        boost::posix_time::milliseconds(timeoutMs);

      boost::mutex::scoped_lock lock(mutex);
      while (!connecting) {
        if(!cond.timed_wait(lock,timeout)) {
          return false;
        }
      }
      return true;
    }


    bool waitForAuthFailed(uint32_t timeoutMs) {
      boost::system_time const timeout=boost::get_system_time() +
        boost::posix_time::milliseconds(timeoutMs);

      boost::mutex::scoped_lock lock(mutex);
      while (!authFailed) {
        if(!cond.timed_wait(lock,timeout)) {
          return false;
        }
      }
      return true;
    }

    boost::condition_variable cond;
    boost::mutex mutex;
    bool connected;
    bool connecting;
    bool authFailed;
};

TEST(CppClient, testInit) {
  ZooKeeper zk;
  shared_ptr<TestInitWatch> watch(new TestInitWatch());
  EXPECT_EQ(ReturnCode::Ok, zk.init(ZkServer::HOST_PORT, 30000, watch));
  EXPECT_TRUE(watch->waitForConnected(1000));
  EXPECT_EQ(SessionState::Connected, zk.getState());
  ZkServer::stop();
  EXPECT_TRUE(watch->waitForConnecting(1000));
  EXPECT_EQ(SessionState::Connecting, zk.getState());
  ZkServer::start();
}

TEST(CppClient, testCreate) {
  ZooKeeper zk, zk2;
  data::Stat stat;
  std::string pathCreated;
  std::vector<data::ACL> acl;
  data::ACL temp;
  temp.getid().getscheme() = "world";
  temp.getid().getid() = "anyone";
  temp.setperms(Permission::All);
  acl.push_back(temp);

  EXPECT_EQ(SessionState::Expired, zk.getState());
  ReturnCode::type rc = zk.create("/hello", "world",  acl,
      CreateMode::Persistent, pathCreated);
  EXPECT_EQ(ReturnCode::BadArguments, rc);

  shared_ptr<TestInitWatch> watch(new TestInitWatch());

  EXPECT_EQ(ReturnCode::Ok, zk2.init("localhost:12346", 30000,
        watch));

  EXPECT_EQ(ReturnCode::Ok, zk.init(ZkServer::HOST_PORT, 30000, watch));

  rc = zk.exists("/hello", boost::shared_ptr<Watch>(), stat);
  EXPECT_EQ(ReturnCode::NoNode, rc);

  rc = zk.create("/hello", "world",  acl, CreateMode::Persistent,
      pathCreated);
  EXPECT_EQ(ReturnCode::Ok, rc);
  EXPECT_EQ(std::string("/hello"), pathCreated);

  rc = zk.exists("/hello", boost::shared_ptr<Watch>(new TestInitWatch()),
      stat);
  EXPECT_EQ(ReturnCode::Ok, rc);

  rc = zk.exists("/hello", boost::shared_ptr<Watch>(new TestInitWatch()),
      stat);
  EXPECT_EQ(ReturnCode::Ok, rc);
  EXPECT_EQ(stat.getczxid(), stat.getmzxid());
  EXPECT_EQ(0, stat.getversion());
  EXPECT_EQ(0, stat.getcversion());
  EXPECT_EQ(0, stat.getaversion());
  EXPECT_EQ(0, (int)stat.getephemeralOwner());
  EXPECT_EQ(5, stat.getdataLength());
  EXPECT_EQ(0, stat.getnumChildren());
}

TEST(CppClient, testSequential) {
  ZooKeeper zk;
  ReturnCode::type rc;
  data::Stat stat;
  std::string pathCreated;
  std::vector<data::ACL> acl;
  data::ACL temp;
  temp.getid().getscheme() = "world";
  temp.getid().getid() = "anyone";
  temp.setperms(Permission::All);
  acl.push_back(temp);

  rc = zk.init(ZkServer::HOST_PORT, 30000, shared_ptr<Watch>());
  EXPECT_EQ(ReturnCode::Ok, rc);

  // Non-sequential path cannot end with a '/'.
  rc = zk.create("/testSequential", "data",  acl, CreateMode::Persistent,
                 pathCreated);
  EXPECT_EQ(ReturnCode::Ok, rc);
  rc = zk.create("/testSequential/", "data",  acl, CreateMode::Persistent,
                 pathCreated);
  EXPECT_EQ(ReturnCode::BadArguments, rc);

  // Create /testSequential/000000000 ... /testSequential/000000009
  for (int i = 0; i < 10; i++) {
    rc = zk.create("/testSequential/", "data",  acl,
        CreateMode::PersistentSequential, pathCreated);
    EXPECT_EQ(ReturnCode::Ok, rc);
    EXPECT_EQ(str(boost::format("/testSequential/%010d") % i), pathCreated);
    rc = zk.exists(pathCreated, boost::shared_ptr<Watch>(), stat);
    EXPECT_EQ(ReturnCode::Ok, rc);
  }
}

TEST(CppClient, testBasic) {
  ZooKeeper zk;
  data::Stat stat;
  std::string znodeName = "/testBasic";
  std::string dataInput = "hello";
  std::string dataInput2 = "goodbye";
  std::string dataOutput;
  std::string pathCreated;
  std::vector<std::string> children;
  std::vector<data::ACL> acl;
  data::ACL temp;
  temp.getid().getscheme() = "world";
  temp.getid().getid() = "anyone";
  temp.setperms(Permission::All);
  acl.push_back(temp);

  shared_ptr<TestInitWatch> watch(new TestInitWatch());
  EXPECT_EQ(ReturnCode::Ok, zk.init(ZkServer::HOST_PORT, 30000, watch));

  // exists() on nonexistent znode.
  ReturnCode::type rc = zk.exists(znodeName, boost::shared_ptr<Watch>(),
      stat);
  EXPECT_EQ(ReturnCode::NoNode, rc);

  // get() on onexistent znode.
  rc = zk.get(znodeName, boost::shared_ptr<Watch>(), dataOutput, stat);
  EXPECT_EQ(ReturnCode::NoNode, rc);

  // getChildren() on onexistent znode.
  rc = zk.getChildren(znodeName, boost::shared_ptr<Watch>(),
      children, stat);
  EXPECT_EQ(ReturnCode::NoNode, rc);

  // set() on onexistent znode.
  rc = zk.set(znodeName, dataInput, -1, stat);
  EXPECT_EQ(ReturnCode::NoNode, rc);

  // create()
  rc = zk.create(znodeName, dataInput,  acl, CreateMode::Persistent,
      pathCreated);
  EXPECT_EQ(ReturnCode::Ok, rc);
  EXPECT_EQ(znodeName, pathCreated);

  // create() on existing znode.
  rc = zk.create(znodeName, dataInput,  acl, CreateMode::Persistent,
      pathCreated);
  EXPECT_EQ(ReturnCode::NodeExists, rc);

  // exists()
  rc = zk.exists(znodeName, boost::shared_ptr<Watch>(), stat);
  EXPECT_EQ(ReturnCode::Ok, rc);
  EXPECT_EQ(stat.getczxid(), stat.getmzxid());
  EXPECT_EQ(0, stat.getversion());
  EXPECT_EQ(0, stat.getcversion());
  EXPECT_EQ(0, stat.getaversion());
  EXPECT_EQ(0, (int)stat.getephemeralOwner());
  EXPECT_EQ(5, stat.getdataLength());
  EXPECT_EQ(0, stat.getnumChildren());

  // get()
  rc = zk.get(znodeName, boost::shared_ptr<Watch>(), dataOutput, stat);
  EXPECT_EQ(ReturnCode::Ok, rc);
  EXPECT_EQ(dataInput, dataOutput);
  EXPECT_EQ(stat.getczxid(), stat.getmzxid());
  EXPECT_EQ(0, stat.getversion());
  EXPECT_EQ(0, stat.getcversion());
  EXPECT_EQ(0, stat.getaversion());
  EXPECT_EQ(0, (int)stat.getephemeralOwner());
  EXPECT_EQ(5, stat.getdataLength());
  EXPECT_EQ(0, stat.getnumChildren());

  // set() with bad version
  rc = zk.set(znodeName, dataInput2, 10, stat);
  EXPECT_EQ(ReturnCode::BadVersion, rc);

  // set()
  rc = zk.set(znodeName, dataInput2, 0, stat);
  EXPECT_EQ(ReturnCode::Ok, rc);
  EXPECT_EQ(dataInput, dataOutput);
  EXPECT_TRUE(stat.getczxid() < stat.getmzxid());
  EXPECT_EQ(1, stat.getversion());
  EXPECT_EQ(0, stat.getcversion());
  EXPECT_EQ(0, stat.getaversion());
  EXPECT_EQ(0, (int)stat.getephemeralOwner());
  EXPECT_EQ(7, stat.getdataLength());
  EXPECT_EQ(0, stat.getnumChildren());

  // add some children
  int numChildren = 10;
  for (int i = 0; i < numChildren; i++) {
    std::string child = str(boost::format("%s/child%d") % znodeName % i);
    rc = zk.create(child, dataInput,  acl, CreateMode::Persistent,
        pathCreated);
    EXPECT_EQ(ReturnCode::Ok, rc);
    EXPECT_EQ(child, pathCreated);
  }

  // getChildren()
  rc = zk.getChildren(znodeName, boost::shared_ptr<Watch>(),
      children, stat);
  EXPECT_EQ(ReturnCode::Ok, rc);
  EXPECT_EQ(10, (int)children.size());
  for (int i = 0; i < numChildren; i++) {
    std::string child = str(boost::format("child%d") % i);
    std::vector<std::string>::iterator itr;
    itr = find(children.begin(), children.end(), child);
    EXPECT_TRUE(itr != children.end());
  }

  EXPECT_TRUE(stat.getczxid() < stat.getmzxid());
  EXPECT_EQ(1, stat.getversion());
  EXPECT_EQ(numChildren, stat.getcversion());
  EXPECT_EQ(0, stat.getaversion());
  EXPECT_EQ(0, (int)stat.getephemeralOwner());
  EXPECT_EQ(7, stat.getdataLength());
  EXPECT_EQ(numChildren, stat.getnumChildren());

  // remove() with children
  rc = zk.remove(znodeName, 1);
  EXPECT_EQ(ReturnCode::NotEmpty, rc);

  // remove all the children
  for (int i = 0; i < numChildren; i++) {
    std::string child = str(boost::format("%s/child%d") % znodeName % i);
    rc = zk.remove(child, -1);
    EXPECT_EQ(ReturnCode::Ok, rc);
    rc = zk.exists(znodeName, boost::shared_ptr<Watch>(), stat);
    EXPECT_EQ(ReturnCode::Ok, rc);
    EXPECT_TRUE(stat.getczxid() < stat.getmzxid());
    EXPECT_EQ(1, stat.getversion());
    EXPECT_EQ(numChildren + i + 1, stat.getcversion());
    EXPECT_EQ(0, stat.getaversion());
    EXPECT_EQ(0, (int)stat.getephemeralOwner());
    EXPECT_EQ(7, stat.getdataLength());
    EXPECT_EQ(numChildren - i - 1, stat.getnumChildren());
  }

  // remove() with bad version
  rc = zk.remove(znodeName, 10);
  EXPECT_EQ(ReturnCode::BadVersion, rc);

  // remove()
  rc = zk.remove(znodeName, 1);
  EXPECT_EQ(ReturnCode::Ok, rc);

  // remove() nonexistent znode.
  rc = zk.remove(znodeName, 1);
  EXPECT_EQ(ReturnCode::NoNode, rc);
}

TEST(CppClient, testAcl) {
  ZooKeeper zk;
  data::Stat stat;
  std::vector<data::ACL> acl, aclOut;

  shared_ptr<TestInitWatch> watch(new TestInitWatch());
  EXPECT_EQ(ReturnCode::Ok, zk.init(ZkServer::HOST_PORT, 30000, watch));
  EXPECT_TRUE(watch->waitForConnected(1000));

  // get acl for root ("/")
  EXPECT_EQ(ReturnCode::Ok, zk.getAcl("/", acl, stat));
  EXPECT_EQ(1, (int)acl.size());
  EXPECT_EQ(std::string("world"), acl[0].getid().getscheme());
  EXPECT_EQ(std::string("anyone"), acl[0].getid().getid());
  zk.set("/", "test", -1, stat);

  acl.clear();
  // echo -n user1:password1 |openssl dgst -sha1 -binary | base64
  data::ACL temp;
  temp.getid().getscheme() = "digest";
  temp.getid().getid() = "user1:XDkd2dsEuhc9ImU3q8pa8UOdtpI=";
  temp.setperms(Permission::All);
  acl.push_back(temp);

  // echo -n user2:password2 |openssl dgst -sha1 -binary | base64
  temp.getid().getscheme() = "digest";
  temp.getid().getid() = "user2:lo/iTtNMP+gEZlpUNaCqLYO3i5U=";
  temp.setperms(Permission::All);
  acl.push_back(temp);

  // echo -n user3:password3 |openssl dgst -sha1 -binary | base64
  temp.getid().getscheme() = "digest";
  temp.getid().getid() = "user3:wr5Y0kEs9nFX3bKrTMKxrlcFeWo=";
  temp.setperms(Permission::All);
  acl.push_back(temp);

  // setAcl() with bad version
  EXPECT_EQ(ReturnCode::BadVersion, zk.setAcl("/", 10, acl));

  // setAcl()
  EXPECT_EQ(ReturnCode::Ok, zk.setAcl("/", -1, acl));
  EXPECT_EQ(ReturnCode::Ok, zk.getAcl("/", aclOut, stat));

  // ACL of "/" has been modified.
  EXPECT_EQ(acl.size(), aclOut.size());
  for (size_t i = 0; i < acl.size(); i++) {
    EXPECT_TRUE(std::find(aclOut.begin(), aclOut.end(), acl[i]) !=
        aclOut.end());
  }

  // Reset root acl to world anyone.
  std::string scheme = "digest";
  std::string cert = "user1:password1";
  EXPECT_EQ(ReturnCode::Ok, zk.addAuth(scheme, cert));
  acl.clear();
  temp.getid().getscheme() = "world";
  temp.getid().getid() = "anyone";
  temp.setperms(Permission::All);
  acl.push_back(temp);
  EXPECT_EQ(ReturnCode::Ok, zk.setAcl("/", -1, acl));
}

TEST(CppClient, testAddAuth) {
  ZooKeeper zk, zk2;
  std::string pathCreated;
  data::Stat stat;
  ReturnCode::type rc;
  std::vector<data::ACL> acl;

  shared_ptr<TestInitWatch> watch(new TestInitWatch());
  shared_ptr<TestInitWatch> watch2(new TestInitWatch());
  EXPECT_EQ(ReturnCode::Ok, zk.init(ZkServer::HOST_PORT, 30000, watch));
  EXPECT_EQ(ReturnCode::Ok, zk2.init(ZkServer::HOST_PORT, 30000, watch2));
  EXPECT_TRUE(watch->waitForConnected(1000));
  EXPECT_TRUE(watch2->waitForConnected(1000));

  // Test authentication.
  std::string scheme = "digest";
  std::string cert = "user1:password1";
  EXPECT_EQ(ReturnCode::Ok, zk.addAuth(scheme, cert));
  EXPECT_EQ(SessionState::Connected, zk.getState());

  // A ssession can have multiple identities.
  cert = "user2:password2";
  EXPECT_EQ(ReturnCode::Ok, zk2.addAuth(scheme, cert));
  EXPECT_EQ(SessionState::Connected, zk2.getState());
  cert = "user3:password3";
  EXPECT_EQ(ReturnCode::Ok, zk2.addAuth(scheme, cert));
  EXPECT_EQ(SessionState::Connected, zk2.getState());

  // echo -n user1:password1 |openssl dgst -sha1 -binary | base64
  acl.clear();
  data::ACL temp;
  temp.getid().getscheme() = "digest";
  temp.getid().getid() = "user1:XDkd2dsEuhc9ImU3q8pa8UOdtpI=";
  temp.setperms(Permission::All);
  acl.push_back(temp);
  rc = zk.create("/user1", "hello",  acl,
      CreateMode::Persistent, pathCreated);
  EXPECT_EQ(ReturnCode::Ok, rc);

  // echo -n user2:password2 |openssl dgst -sha1 -binary | base64
  acl.clear();
  temp.getid().getscheme() = "digest";
  temp.getid().getid() = "user2:lo/iTtNMP+gEZlpUNaCqLYO3i5U=";
  temp.setperms(Permission::All);
  acl.push_back(temp);
  rc = zk.create("/user2", "hello",  acl,
      CreateMode::Persistent, pathCreated);
  EXPECT_EQ(ReturnCode::Ok, rc);

  // echo -n user3:password3 |openssl dgst -sha1 -binary | base64
  acl.clear();
  temp.getid().getscheme() = "digest";
  temp.getid().getid() = "user3:wr5Y0kEs9nFX3bKrTMKxrlcFeWo=";
  temp.setperms(Permission::All);
  acl.push_back(temp);
  rc = zk.create("/user3", "hello",  acl,
      CreateMode::Persistent, pathCreated);
  EXPECT_EQ(ReturnCode::Ok, rc);

  acl.clear();
  temp.getid().getscheme() = "auth";
  temp.getid().getid() = "";
  temp.setperms(Permission::All);
  acl.push_back(temp);
  rc = zk2.create("/auth", "hello",  acl,
      CreateMode::Persistent, pathCreated);
  EXPECT_EQ(ReturnCode::Ok, rc);
  rc = zk2.set("/auth", "new data", -1, stat);
  EXPECT_EQ(ReturnCode::Ok, rc);

  std::vector<data::ACL> aclVector;
  zk2.getAcl("/auth", aclVector, stat);

  rc = zk.set("/user1", "new data", -1, stat);
  EXPECT_EQ(ReturnCode::Ok, rc);
  rc = zk.set("/user2", "new data", -1, stat);
  EXPECT_EQ(ReturnCode::NoAuth, rc);
  rc = zk.set("/user3", "new data", -1, stat);
  EXPECT_EQ(ReturnCode::NoAuth, rc);

  rc = zk2.set("/user1", "new data", -1, stat);
  EXPECT_EQ(ReturnCode::NoAuth, rc);
  rc = zk2.set("/user2", "new data", -1, stat);
  EXPECT_EQ(ReturnCode::Ok, rc);
  rc = zk2.set("/user3", "new data", -1, stat);
  EXPECT_EQ(ReturnCode::Ok, rc);

  acl.clear();
  temp.getid().getscheme() = "ip";
  temp.getid().getid() = "127.0.0.1";
  temp.setperms(Permission::All);
  acl.push_back(temp);
  rc = zk2.create("/ip", "hello",  acl,
      CreateMode::Persistent, pathCreated);

  EXPECT_EQ(ReturnCode::Ok, rc);
  zk2.getAcl("/ip", aclVector, stat);
  rc = zk2.set("/ip", "new data", -1, stat);
  EXPECT_EQ(ReturnCode::Ok, rc);
}

TEST(CppClient, testPing) {
  ZooKeeper zk, zkIdle;
  std::string pathCreated;
  data::Stat stat;
  ReturnCode::type rc;
  std::vector<data::ACL> acl;
  data::ACL temp;
  temp.getid().getscheme() = "world";
  temp.getid().getid() = "anyone";
  temp.setperms(Permission::All);
  acl.push_back(temp);

  shared_ptr<TestInitWatch> watch(new TestInitWatch());
  shared_ptr<TestInitWatch> watch2(new TestInitWatch());
  EXPECT_EQ(ReturnCode::Ok, zk.init(ZkServer::HOST_PORT, 5000, watch));
  EXPECT_EQ(ReturnCode::Ok, zkIdle.init(ZkServer::HOST_PORT, 5000, watch2));
  EXPECT_TRUE(watch->waitForConnected(1000));
  EXPECT_TRUE(watch2->waitForConnected(1000));

  for(int i = 0; i < 10; i++) {
    std::string path = str(boost::format("/testping_%d") % i);
    rc = zk.create(path, "",  acl, CreateMode::Persistent, pathCreated);
    EXPECT_EQ(ReturnCode::Ok, rc);
  }

  for(int i = 0; i < 10; i++) {
    std::string path = str(boost::format("/testping_%d") % i);
    rc = zkIdle.exists(path, boost::shared_ptr<Watch>(), stat);
    EXPECT_EQ(ReturnCode::Ok, rc);
  }

  // This loop takes more than 5 seconds.
  for(int i = 0; i < 10; i++) {
    std::string path = str(boost::format("/testping_%d") % i);
    usleep(500000);
    rc = zk.remove(path, -1);
    EXPECT_EQ(ReturnCode::Ok, rc);
  }

  // Make sure the session for the idle connection is stil alive.
  for(int i = 0; i < 10; i++) {
    std::string path = str(boost::format("/testping_%d") % i);
    rc = zkIdle.exists(path, boost::shared_ptr<Watch>(), stat);
    EXPECT_EQ(ReturnCode::NoNode, rc);
  }
}

class BadRemoveCallback : public RemoveCallback {
  void process(ReturnCode::type rc, const std::string& path) {
    LOG_DEBUG("callback called: " << ReturnCode::toString(rc) <<
        " " << path);
  }
};

TEST(CppClient, testBadHost) {
  ZooKeeper zk;
  shared_ptr<TestInitWatch> watch(new TestInitWatch());
  EXPECT_EQ(ReturnCode::Ok,
      zk.init("localhost:12345", 5000, watch));
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(ReturnCode::Ok,
        zk.remove("/test", -1, boost::shared_ptr<RemoveCallback>()));
  }
}

class ChrootWatch : public Watch {
  public:
    ChrootWatch(const std::string& pathExpected) :
      pathExpected_(pathExpected), watchTriggered_(false) {
      }
    void process(WatchEvent::type event, SessionState::type state,
        const std::string& path) {
      EXPECT_EQ(pathExpected_, path);
      watchTriggered_ = true;
    }
    std::string pathExpected_;
    bool watchTriggered_;
};

TEST(CppClient, testChroot) {
  ZooKeeper zk, zkChroot;
  std::string path, pathOut, dataOut;
  zk.init("127.0.0.1:22181", 10000, boost::shared_ptr<Watch>());
  zkChroot.init("127.0.0.1:22181/test/chroot", 10000,
      boost::shared_ptr<Watch>());
  data::Stat statOut;
  std::string data = "hello";
  std::vector<data::ACL> openAcl;
  data::ACL allPerm;
  allPerm.getid().getscheme() = "world";
  allPerm.getid().getid() = "anyone";
  allPerm.setperms(Permission::All);
  openAcl.push_back(allPerm);


  EXPECT_EQ(ReturnCode::Ok,
      zk.create("/test", "", openAcl, CreateMode::Persistent, pathOut));
  EXPECT_EQ(ReturnCode::Ok,
      zk.create("/test/chroot", data, openAcl, CreateMode::Persistent,
        pathOut));

  // get
  EXPECT_EQ(ReturnCode::Ok,
      zkChroot.get("/", boost::shared_ptr<Watch>(), dataOut, statOut));
  EXPECT_EQ(data, dataOut);

  //check for watches
  path = "/hello";
  zkChroot.exists(path, boost::shared_ptr<Watch>(new ChrootWatch(path)),
      statOut);

  //check create
  EXPECT_EQ(ReturnCode::Ok,
      zkChroot.create(path, "", openAcl, CreateMode::Persistent, pathOut));
  EXPECT_EQ(path, pathOut);

  EXPECT_EQ(ReturnCode::Ok,
      zkChroot.create("/hello/child", "", openAcl, CreateMode::Persistent,
        pathOut));
  EXPECT_EQ(ReturnCode::Ok,
      zkChroot.exists("/hello/child", boost::shared_ptr<Watch>(), statOut));
  EXPECT_EQ(ReturnCode::Ok,
      zk.exists("/test/chroot/hello/child", boost::shared_ptr<Watch>(),
        statOut));

  EXPECT_EQ(ReturnCode::Ok, zkChroot.remove("/hello/child", -1));
  EXPECT_EQ(ReturnCode::NoNode,
      zkChroot.exists("/hello/child", boost::shared_ptr<Watch>(), statOut));
  EXPECT_EQ(ReturnCode::NoNode,
      zk.exists("/test/chroot/hello/child", boost::shared_ptr<Watch>(),
        statOut));

  EXPECT_EQ(ReturnCode::Ok,
      zkChroot.get("/hello", shared_ptr<Watch>(new ChrootWatch("/hello")),
        dataOut, statOut));
  EXPECT_EQ(ReturnCode::Ok,
      zkChroot.set("/hello", "new data", -1, statOut));

  // check for getchildren
  std::vector<std::string> children;
  EXPECT_EQ(ReturnCode::Ok,
      zkChroot.getChildren("/", boost::shared_ptr<Watch>(), children,
        statOut));
  EXPECT_EQ(1, (int)children.size());
  EXPECT_EQ(std::string("hello"), children[0]);

  // check for get/set acl
  std::vector<data::ACL> acl;
  EXPECT_EQ(ReturnCode::Ok, zkChroot.getAcl("/", acl, statOut));
  EXPECT_EQ(1, (int)acl.size());
  EXPECT_EQ((int)Permission::All, (int)acl[0].getperms());

  // set acl
  acl.clear();
  data::ACL readPerm;
  readPerm.getid().getscheme() = "world";
  readPerm.getid().getid() = "anyone";
  readPerm.setperms(Permission::Read);
  acl.push_back(readPerm);
  EXPECT_EQ(ReturnCode::Ok, zkChroot.setAcl("/hello", -1, acl));
  // see if you add children
  EXPECT_EQ(ReturnCode::NoAuth,
      zkChroot.create("/hello/child1", "", openAcl, CreateMode::Persistent,
        pathOut));

  //add wget children test
  EXPECT_EQ(ReturnCode::Ok,
      zkChroot.getChildren("/", shared_ptr<Watch>(new ChrootWatch("/")),
        children, statOut));
  EXPECT_EQ(1, (int)children.size());
  EXPECT_EQ(std::string("hello"), children[0]);

  //now create a node
  EXPECT_EQ(ReturnCode::Ok,
      zkChroot.create("/child2", "", openAcl, CreateMode::Persistent,
        pathOut));

  //ZOOKEEPER-1027 correctly return path_buffer without prefixed chroot
  path = "/zookeeper1027";
  EXPECT_EQ(ReturnCode::Ok,
      zkChroot.create(path, "", openAcl, CreateMode::Persistent, pathOut));
  EXPECT_EQ(path, pathOut);
}

#if 0
TEST(CppClient, testIPV6) {
  ZooKeeper zk;
  std::string pathCreated;
  zk.init("::1:22181", 10000, boost::shared_ptr<Watch>());
  EXPECT_EQ(ReturnCode::Ok,
      zk.create("/ipv6", "", openAcl, CreateMode::Persistent, pathCreated));
}

void testAcl() {
  ZooKeeper zk;
  std::string pathCreated;
  zk.init(hostPorts, 10000, boost::shared_ptr<Watch>());
  data::Stat stat;
  CPPUNIT_ASSERT_EQUAL(ReturnCode::Ok,
      zk.create("/acl", "", openAcl, CreateMode::Persistent, pathCreated));
  std::vector<data::ACL> acl, aclOut;

  CPPUNIT_ASSERT_EQUAL(ReturnCode::Ok, zk.getAcl("/acl", aclOut, stat));
  CPPUNIT_ASSERT_EQUAL(1, (int)aclOut.size());
  CPPUNIT_ASSERT_EQUAL(std::string("world"), aclOut[0].getid().getscheme());
  CPPUNIT_ASSERT_EQUAL(std::string("anyone"), aclOut[0].getid().getid());

  data::ACL readPerm;
  readPerm.getid().getscheme() = "world";
  readPerm.getid().getid() = "anyone";
  readPerm.setperms(Permission::Read);
  acl.push_back(readPerm);
  CPPUNIT_ASSERT_EQUAL(ReturnCode::Ok, zk.setAcl("/acl", -1, acl));

  CPPUNIT_ASSERT_EQUAL(ReturnCode::Ok, zk.getAcl("/acl", aclOut, stat));
  CPPUNIT_ASSERT_EQUAL(1, (int)aclOut.size());
  CPPUNIT_ASSERT_EQUAL(std::string("world"), readPerm.getid().getscheme());
  CPPUNIT_ASSERT_EQUAL(std::string("anyone"), readPerm.getid().getid());
  CPPUNIT_ASSERT_EQUAL((int)Permission::Read, readPerm.getperms());
}

#endif
