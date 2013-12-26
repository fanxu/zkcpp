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
#include <memory_in_stream.hh>
#include <string_out_stream.hh>
#include <zookeeper.jute.hh>
#include <recordio.hh>
#include <binarchive.hh>

using namespace boost;
using namespace org::apache::zookeeper;

TEST(TestRecordIo, testMemoryInStream) {
  const char* inbuf = "abcdefg";
  char outbuf[strlen(inbuf)];
  MemoryInStream stream(inbuf, strlen(inbuf));
  EXPECT_EQ(1, (int)stream.read(outbuf, 1));
  EXPECT_EQ('a', outbuf[0]);
  EXPECT_EQ(2, (int)stream.read(outbuf, 2));
  EXPECT_EQ('b', outbuf[0]);
  EXPECT_EQ('c', outbuf[1]);
  EXPECT_EQ(3, (int)stream.read(outbuf, 3));
  EXPECT_EQ('d', outbuf[0]);
  EXPECT_EQ('e', outbuf[1]);
  EXPECT_EQ('f', outbuf[2]);
  // only have 1 byte left to read
  EXPECT_EQ(1, (int)stream.read(outbuf, 4));
  EXPECT_EQ('g', outbuf[0]);
  // nothing left to read
  EXPECT_EQ(0, (int)stream.read(outbuf, 4));
}

TEST(TestRecordIo, testStringOutStream) {
  std::string outbuf;
  StringOutStream stream(outbuf);
  EXPECT_EQ(1, (int)stream.write("a", 1));
  EXPECT_EQ(2, (int)stream.write("bc", 2));
  EXPECT_EQ(3, (int)stream.write("def", 3));
  EXPECT_EQ(0, (int)stream.write(NULL, 10));
  EXPECT_EQ(0, (int)stream.write("g", 0));
  EXPECT_EQ(std::string("abcdef"), outbuf);
}

/**
 * I don't know what the tag parameter in serialize()/deserialize() methods
 * is supposed to do.
 */
TEST(TestRecordIo, testId) {
  data::Id id1, id2;
  id1.getscheme() = "test_scheme";
  id1.getid() = "test_id";
  EXPECT_EQ(std::string("test_scheme"), id1.getscheme());
  EXPECT_EQ(std::string("test_id"), id1.getid());

  // serialize
  std::string serialized;
  StringOutStream stream(serialized);
  hadoop::OBinArchive oarchive(stream);
  id1.serialize(oarchive, "mytag");

  // deserialize
  MemoryInStream istream(serialized.c_str(), serialized.size());
  hadoop::IBinArchive iarchive(istream);
  id2.deserialize(iarchive, "something else?");
  EXPECT_TRUE(id1 == id2);
}

TEST(TestRecordIo, testAcl) {
  data::ACL acl1, acl2;
  acl1.setperms(0xff);
  acl1.getid().getscheme() = "my_scheme";
  acl1.getid().getid() = "my_id";

  // serialize
  std::string serialized;
  StringOutStream stream(serialized);
  hadoop::OBinArchive oarchive(stream);
  acl1.serialize(oarchive, "mytag");

  // deserialize
  MemoryInStream istream(serialized.c_str(), serialized.size());
  hadoop::IBinArchive iarchive(istream);
  acl2.deserialize(iarchive, "something else?");
  EXPECT_TRUE(acl1 == acl2);
}

TEST(TestRecordIo, testConnectResponse) {
  proto::ConnectResponse res1, res2;
  res1.setprotocolVersion(10);
  res1.settimeOut(123);
  res1.setsessionId(2);
  res1.getpasswd() = "mypass";

  // serialize
  std::string serialized;
  StringOutStream stream(serialized);
  hadoop::OBinArchive oarchive(stream);
  res1.serialize(oarchive, "mytag");

  // deserialize
  MemoryInStream istream(serialized.c_str(), serialized.size());
  hadoop::IBinArchive iarchive(istream);
  res2.deserialize(iarchive, "something else?");
  EXPECT_TRUE(res1 == res2);
}
