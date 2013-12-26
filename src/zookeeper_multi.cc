/*
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
#include "zookeeper/zookeeper_multi.hh"

namespace org {
namespace apache {

/** ZooKeeper namespace. */
namespace zookeeper {

Op::
Op(OpCode::type type, const std::string& path) : type_(type), path_(path) {
}

Op::
~Op() {
}

OpCode::type Op::
getType() const {
  return type_;
}

std::string Op::
getPath() const {
  return path_;
}

Op::Create::
Create(const std::string& path, const std::string& data,
       const std::vector<data::ACL>& acl, CreateMode::type mode) :
  Op(OpCode::Create, path), data_(data), acl_(acl), mode_(mode) {
}

Op::Create::
~Create() {
}


const std::string& Op::Create::
getData() const {
  return data_;
}

const std::vector<data::ACL>& Op::Create::
getAcl() const {
  return acl_;
}

CreateMode::type Op::Create::
getMode() const {
  return mode_;
}

Op::Remove::
Remove(const std::string& path, int32_t version) :
  Op(OpCode::Remove, path), version_(version) {
}

Op::Remove::
~Remove() {
}

int32_t Op::Remove::
getVersion() const {
  return version_;
}


Op::SetData::
SetData(const std::string& path, const std::string& data, int32_t version) :
  Op(OpCode::SetData, path), data_(data), version_(version) {
}

Op::SetData::
~SetData() {
}

const std::string& Op::SetData::
getData() const {
  return data_;
}

int32_t Op::SetData::
getVersion() const {
  return version_;
}

void Op::SetData::
setVersion(int32_t version) {
  version_ = version;
}


Op::Check::
Check(const std::string& path, int32_t version) :
  Op(OpCode::Check, path), version_(version) {
}

Op::Check::
~Check() {
}

int32_t Op::Check::
getVersion() const {
  return version_;
}

OpResult::
OpResult(OpCode::type type, ReturnCode::type rc) : type_(type), rc_(rc) {
}

OpResult::
~OpResult() {
}

OpCode::type OpResult::
getType() const {
  return type_;
}

ReturnCode::type OpResult::
getReturnCode() const {
  return rc_;
}

void OpResult::
setReturnCode(ReturnCode::type rc) {
  rc_ = rc;
}


OpResult::Create::
Create() : OpResult(OpCode::Create, ReturnCode::Ok) {
}

OpResult::Create::
~Create() {
}

const std::string OpResult::Create::
getPathCreated() const {
  return pathCreated_;
}

void OpResult::Create::
setPathCreated(const std::string& pathCreated) {
  pathCreated_ = pathCreated;
}

OpResult::Remove::
Remove() : OpResult(OpCode::Remove, ReturnCode::Ok) {
}

OpResult::Remove::
~Remove() {
}

OpResult::SetData::
SetData() : OpResult(OpCode::SetData, ReturnCode::Ok) {
}

OpResult::SetData::
~SetData() {
}

const data::Stat OpResult::SetData::
getStat() const {
  return stat_;
}

void OpResult::SetData::
setStat(const data::Stat& stat) {
  stat_ = stat;
}

OpResult::Check::
Check() : OpResult(OpCode::Check, ReturnCode::Ok) {
}

OpResult::Check::
~Check() {
}

OpResult::Error::
Error() : OpResult(OpCode::Error, ReturnCode::Ok) {
}

OpResult::Error::
~Error() {
}

}}}  // namespace org::apache::zookeeper
