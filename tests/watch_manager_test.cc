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
#include <zookeeper/logging.hh>
ENABLE_LOGGING;

#include <algorithm>
#include "watch_manager.hh"
#include "zk_server.hh"

using namespace boost;
using namespace org::apache::zookeeper;

TEST(WatchManager, negative) {
  std::vector<std::string> paths;
  shared_ptr<WatchManager> manager(new WatchManager());
  ExistsWatchRegistration exists(manager, "/path", shared_ptr<Watch>());
  GetDataWatchRegistration data(manager, "/path", shared_ptr<Watch>());
  GetChildrenWatchRegistration children(manager, "/path", shared_ptr<Watch>());

  // Make sure activate fails on invalid return codes.
  EXPECT_FALSE(exists.activate(ReturnCode::InvalidState));
  EXPECT_FALSE(exists.activate(ReturnCode::BadArguments));
  EXPECT_FALSE(exists.activate(ReturnCode::ConnectionLoss));
  EXPECT_FALSE(exists.activate(ReturnCode::NodeExists));

  EXPECT_FALSE(data.activate(ReturnCode::NoNode));
  EXPECT_FALSE(data.activate(ReturnCode::InvalidState));
  EXPECT_FALSE(data.activate(ReturnCode::BadArguments));
  EXPECT_FALSE(data.activate(ReturnCode::ConnectionLoss));
  EXPECT_FALSE(data.activate(ReturnCode::NodeExists));

  EXPECT_FALSE(children.activate(ReturnCode::NoNode));
  EXPECT_FALSE(children.activate(ReturnCode::InvalidState));
  EXPECT_FALSE(children.activate(ReturnCode::BadArguments));
  EXPECT_FALSE(children.activate(ReturnCode::ConnectionLoss));
  EXPECT_FALSE(children.activate(ReturnCode::NodeExists));

  // No watches should have been activated
  manager->getExistsPaths(paths);
  EXPECT_TRUE(paths.empty());
  manager->getGetDataPaths(paths);
  EXPECT_TRUE(paths.empty());
  manager->getGetChildrenPaths(paths);
  EXPECT_TRUE(paths.empty());
}

class EmptyWatch : public Watch {
  void process(WatchEvent::type event, SessionState::type state,
               const std::string& path) {};
};

TEST(WatchManager, activate) {
  std::vector<std::string> paths;
  shared_ptr<WatchManager> manager(new WatchManager());
  manager->setDefaultWatch(boost::shared_ptr<Watch>(new EmptyWatch()));

  // GetWatches should return the default watch for session events.
  std::list<boost::shared_ptr<Watch> > watches;
  manager->getWatches(WatchEvent::SessionStateChanged, SessionState::Connected,
                      "", watches);
  EXPECT_EQ(1, watches.size());

  for (int i = 0; i < 10; i++) {
    // 10 watches for /exists{i}
    std::string path = str(boost::format("/exists%d") % i);
    for (int j = 0; j < 10; j++) {
      ExistsWatchRegistration exists(manager, path, shared_ptr<Watch>());
      EXPECT_TRUE(exists.activate(ReturnCode::NoNode));
    }

    // 10 watches for /data{i}
    path = str(boost::format("/data%d") % i);
    for (int j = 0; j < 10; j++) {
      GetDataWatchRegistration data(manager, path, shared_ptr<Watch>());
      EXPECT_TRUE(data.activate(ReturnCode::Ok));
    }

    // 10 watches for /children{i}
    path = str(boost::format("/children%d") % i);
    for (int j = 0; j < 10; j++) {
      GetChildrenWatchRegistration children(manager, path, shared_ptr<Watch>());
      EXPECT_TRUE(children.activate(ReturnCode::Ok));
    }
  }

  // existsPaths should contain 10 paths.
  manager->getExistsPaths(paths);
  EXPECT_EQ(10, paths.size());
  for (int i = 0; i < 10; i++) {
    std::string path = str(boost::format("/exists%d") % i);
    EXPECT_NE(paths.end(), find(paths.begin(), paths.end(), path));
  }

  // getDataPaths should contain 10 paths.
  manager->getGetDataPaths(paths);
  EXPECT_EQ(10, paths.size());
  for (int i = 0; i < 10; i++) {
    std::string path = str(boost::format("/data%d") % i);
    EXPECT_NE(paths.end(), find(paths.begin(), paths.end(), path));
  }

  // getChildrenPaths should contain 10 paths
  manager->getGetChildrenPaths(paths);
  EXPECT_EQ(10, paths.size());
  for (int i = 0; i < 10; i++) {
    std::string path = str(boost::format("/children%d") % i);
    EXPECT_NE(paths.end(), find(paths.begin(), paths.end(), path));
  }

  // GetWatches should return all the watches for session events, including the
  // default watch.
  manager->getWatches(WatchEvent::SessionStateChanged, SessionState::Connected,
                      "", watches);
  EXPECT_EQ(301, watches.size());

  // Non-existent path.
  manager->getWatches(WatchEvent::ZnodeCreated, SessionState::Connected,
                      "/nonexistent", watches);
  EXPECT_EQ(0, watches.size());
  manager->getWatches(WatchEvent::ZnodeRemoved, SessionState::Connected,
                      "/nonexistent", watches);
  EXPECT_EQ(0, watches.size());
  manager->getWatches(WatchEvent::ZnodeDataChanged, SessionState::Connected,
                      "/nonexistent", watches);
  EXPECT_EQ(0, watches.size());
  manager->getWatches(WatchEvent::ZnodeChildrenChanged, SessionState::Connected,
                      "/nonexistent", watches);
  EXPECT_EQ(0, watches.size());

  // exists watches can be triggered by ZnodeCreated.
  for (int i = 0; i < 10; i++) {
    std::string path = str(boost::format("/exists%d") % i);
    manager->getWatches(WatchEvent::ZnodeCreated,
                        SessionState::Connected, path, watches);
    EXPECT_EQ(10, watches.size());
  }

  // data watches can be triggered by ZnodeDataChanged or ZnodeRemoved.
  for (int i = 0; i < 5; i++) {
    std::string path = str(boost::format("/data%d") % i);
    manager->getWatches(WatchEvent::ZnodeDataChanged,
                        SessionState::Connected, path, watches);
    EXPECT_EQ(10, watches.size());
  }
  for (int i = 5; i < 10; i++) {
    std::string path = str(boost::format("/data%d") % i);
    manager->getWatches(WatchEvent::ZnodeRemoved,
                        SessionState::Connected, path, watches);
    EXPECT_EQ(10, watches.size());
  }

  // children watches can be triggered by ZnodeChildrenChanged or ZnodeRemoved.
  for (int i = 0; i < 5; i++) {
    std::string path = str(boost::format("/children%d") % i);
    manager->getWatches(WatchEvent::ZnodeChildrenChanged,
                        SessionState::Connected, path, watches);
    EXPECT_EQ(10, watches.size());
  }
  for (int i = 5; i < 10; i++) {
    std::string path = str(boost::format("/children%d") % i);
    manager->getWatches(WatchEvent::ZnodeRemoved,
                        SessionState::Connected, path, watches);
    EXPECT_EQ(10, watches.size());
  }

  // Now all the watches have been triggered.
  manager->getExistsPaths(paths);
  EXPECT_TRUE(paths.empty());
  manager->getGetDataPaths(paths);
  EXPECT_TRUE(paths.empty());
  manager->getGetChildrenPaths(paths);
  EXPECT_TRUE(paths.empty());

  // getWatches still returns the default watch.
  manager->getWatches(WatchEvent::SessionStateChanged, SessionState::Connected,
                      "", watches);
  EXPECT_EQ(1, watches.size());
}

