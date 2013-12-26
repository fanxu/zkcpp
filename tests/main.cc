#include <gtest/gtest.h>
#include "zk_server.hh"

class ZkEnvironment : public ::testing::Environment {
  virtual void SetUp() {
    ZkServer::start();
  }

  virtual void TearDown() {
    ZkServer::stop();
  }
};

int main(int argc, char **argv) {
  ::testing::AddGlobalTestEnvironment(new ZkEnvironment());
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
