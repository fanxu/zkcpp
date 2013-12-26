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
#include <string>
#include <boost/ptr_container/ptr_vector.hpp>
#include <zookeeper/zookeeper_multi.hh>

using namespace org::apache::zookeeper;

TEST(TestOp, testOp) {
  boost::ptr_vector<Op> ops;

  ops.push_back(new Op::Create("/path", "data", std::vector<data::ACL>(),
                               CreateMode::Persistent));
  ops.push_back(new Op::Remove("/path", -1));
  ops.push_back(new Op::SetData("/path", "data", -1));
  ops.push_back(new Op::Check("/path", -1));

  boost::ptr_vector<OpResult> results;
  results.push_back(new OpResult::Create());
  results.push_back(new OpResult::Remove());
  results.push_back(new OpResult::SetData());
  results.push_back(new OpResult::Check());
  results.push_back(new OpResult::Error());
}
