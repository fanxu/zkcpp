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

#include "binarchive.hh"
#include <boost/asio.hpp>


using namespace hadoop;

int64_t htonll(int64_t v) {
  int i = 0;
  char *s = (char *)&v;
  if (htonl(1) == 1) {
    return v;
  }
  for (i = 0; i < 4; i++) {
    int tmp = s[i];
    s[i] = s[8-i-1];
    s[8-i-1] = tmp;
  }

  return v;
}

int64_t ntohll(int64_t v) {
  return htonll(v);
}

template <typename T>
static void serialize(T t, OutStream& stream)
{
  if (sizeof(T) != stream.write((const void *) &t, sizeof(T))) {
    throw new IOException("Error serializing data.");
  }
}

template <typename T>
static void deserialize(T& t, InStream& stream)
{
  if (sizeof(T) != stream.read((void *) &t, sizeof(T))) {
    throw new IOException("Error deserializing data.");
  }
}

static void serializeLong(int64_t t, OutStream& stream)
{
  const int64_t num = htonll(t);
  assert(sizeof(num) == stream.write(&num, sizeof(num)));
}

static void deserializeLong(int64_t& t, InStream& stream)
{
  int64_t num;
  assert(sizeof(num) == stream.read(&num, sizeof(num)));
  t = ntohll(num);
}

static void serializeInt(int32_t t, OutStream& stream)
{
  const int32_t num = htonl(t);
  assert(sizeof(num) == stream.write(&num, sizeof(num)));
}

static void deserializeInt(int32_t& t, InStream& stream)
{
  int32_t num;
  assert(sizeof(num) == stream.read(&num, sizeof(num)));
  t = ntohl(num);
}

static void serializeFloat(float t, OutStream& stream)
{
  assert("!Serializing froat is not supported\n");
}

static void deserializeFloat(float& t, InStream& stream)
{
  assert("!Deserializing froat is not supported\n");
}

static void serializeDouble(double t, OutStream& stream)
{
  assert("!Serializing double is not supported\n");
}

static void deserializeDouble(double& t, InStream& stream)
{
  assert("!Deserializing double is not supported\n");
}

static void serializeString(const std::string& t, OutStream& stream)
{
  ::serializeInt(t.length(), stream);
  if (t.length() > 0) {
    stream.write(t.data(), t.length());
  }
}

static void deserializeString(std::string& t, InStream& stream)
{
  int32_t len = 0;
  t.clear();
  ::deserializeInt(len, stream);
  if (len > 0) {
    // resize the string to the right length
    t.resize(len);
    assert(len == stream.read((void *)t.data(), len));
  }
}

void hadoop::IBinArchive::deserialize(int8_t& t, const char* tag)
{
  ::deserialize(t, stream);
}

void hadoop::IBinArchive::deserialize(bool& t, const char* tag)
{
  ::deserialize(t, stream);
}

void hadoop::IBinArchive::deserialize(int32_t& t, const char* tag)
{
  ::deserializeInt(t, stream);
}

void hadoop::IBinArchive::deserialize(int64_t& t, const char* tag)
{
  ::deserializeLong(t, stream);
}

void hadoop::IBinArchive::deserialize(float& t, const char* tag)
{
  ::deserializeFloat(t, stream);
}

void hadoop::IBinArchive::deserialize(double& t, const char* tag)
{
  ::deserializeDouble(t, stream);
}

void hadoop::IBinArchive::deserialize(std::string& t, const char* tag)
{
  ::deserializeString(t, stream);
}

void hadoop::IBinArchive::deserialize(std::string& t, size_t& len, const char* tag)
{
  ::deserializeString(t, stream);
  len = t.length();
}

void hadoop::IBinArchive::startRecord(Record& s, const char* tag)
{
}

void hadoop::IBinArchive::endRecord(Record& s, const char* tag)
{
}

Index* hadoop::IBinArchive::startVector(const char* tag)
{
  int32_t len;
  ::deserializeInt(len, stream);
  BinIndex *idx = new BinIndex((size_t) len);
  return idx;
}

void hadoop::IBinArchive::endVector(Index* idx, const char* tag)
{
  delete idx;
}

Index* hadoop::IBinArchive::startMap(const char* tag)
{
  int32_t len;
  ::deserializeInt(len, stream);
  BinIndex *idx = new BinIndex((size_t) len);
  return idx;
}

void hadoop::IBinArchive::endMap(Index* idx, const char* tag)
{
  delete idx;
}

hadoop::IBinArchive::~IBinArchive()
{
}

void hadoop::OBinArchive::serialize(int8_t t, const char* tag)
{
  ::serialize(t, stream);
}

void hadoop::OBinArchive::serialize(bool t, const char* tag)
{
  ::serialize(t, stream);
}

void hadoop::OBinArchive::serialize(int32_t t, const char* tag)
{
  ::serializeInt(t, stream);
}

void hadoop::OBinArchive::serialize(int64_t t, const char* tag)
{
  ::serializeLong(t, stream);
}

void hadoop::OBinArchive::serialize(float t, const char* tag)
{
  ::serializeFloat(t, stream);
}

void hadoop::OBinArchive::serialize(double t, const char* tag)
{
  ::serializeDouble(t, stream);
}

void hadoop::OBinArchive::serialize(const std::string& t, const char* tag)
{
  ::serializeString(t, stream);
}

void hadoop::OBinArchive::serialize(const std::string& t, size_t len, const char* tag)
{
  ::serializeString(t, stream);
}

void hadoop::OBinArchive::startRecord(const Record& s, const char* tag)
{
}

void hadoop::OBinArchive::endRecord(const Record& s, const char* tag)
{
}

void hadoop::OBinArchive::startVector(size_t len, const char* tag)
{
  ::serializeInt(len, stream);
}

void hadoop::OBinArchive::endVector(size_t len, const char* tag)
{
}

void hadoop::OBinArchive::startMap(size_t len, const char* tag)
{
  ::serializeInt(len, stream);
}

void hadoop::OBinArchive::endMap(size_t len, const char* tag)
{
}

hadoop::OBinArchive::~OBinArchive()
{
}
