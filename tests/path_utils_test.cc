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
#include "path_utils.hh"

using namespace org::apache::zookeeper;

TEST(PathUtilsTest, prependChroot) {
  // Basic test.
  EXPECT_EQ("/chroot/hello", PathUtils::prependChroot("/hello", "/chroot"));

  // It should return the original path if chroot is empty.
  EXPECT_EQ("/hello", PathUtils::prependChroot("/hello", ""));

  // It should return the chroot itself if the path is "/".
  EXPECT_EQ("/chroot", PathUtils::prependChroot("/", "/chroot"));
}

TEST(PathUtilsTest, stripChroot) {
  // Basic test.
  EXPECT_EQ("/hello", PathUtils::stripChroot("/chroot/hello", "/chroot"));

  // It should return "/" if the chroot is same as the path.
  EXPECT_EQ("/", PathUtils::stripChroot("/chroot", "/chroot"));

  // It should return the path itself if the path doesn't start with chroot.
  EXPECT_EQ("/hello/world", PathUtils::stripChroot("/hello/world", "/chroot"));
}
