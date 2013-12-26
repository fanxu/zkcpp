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

#ifndef __ZOOKEEPER_JUTE_HH__
#define __ZOOKEEPER_JUTE_HH__
#include "recordio.hh"
namespace org {
namespace apache {
namespace zookeeper {
namespace data {
class Id : public ::hadoop::Record {
private:
   ::std::string mscheme;
   ::std::string mid;
  mutable std::bitset<2> bs_;
public:
  virtual void serialize(::hadoop::OArchive& a_, const char* tag) const;
  virtual void deserialize(::hadoop::IArchive& a_, const char* tag);
  virtual const ::std::string& type() const;
  virtual const ::std::string& signature() const;
  virtual bool validate() const;
  virtual bool operator<(const Id& peer_) const;
  virtual bool operator==(const Id& peer_) const;
  virtual ~Id() {};
  virtual const  ::std::string& getscheme() const {
    return mscheme;
  }
  virtual  ::std::string& getscheme() {
    bs_.set(0);return mscheme;
  }
  virtual const  ::std::string& getid() const {
    return mid;
  }
  virtual  ::std::string& getid() {
    bs_.set(1);return mid;
  }
}; // end record Id
} // end namespace data
} // end namespace zookeeper
} // end namespace apache
} // end namespace org
namespace org {
namespace apache {
namespace zookeeper {
namespace data {
class ACL : public ::hadoop::Record {
private:
  int32_t mperms;
  org::apache::zookeeper::data::Id mid;
  mutable std::bitset<2> bs_;
public:
  virtual void serialize(::hadoop::OArchive& a_, const char* tag) const;
  virtual void deserialize(::hadoop::IArchive& a_, const char* tag);
  virtual const ::std::string& type() const;
  virtual const ::std::string& signature() const;
  virtual bool validate() const;
  virtual bool operator<(const ACL& peer_) const;
  virtual bool operator==(const ACL& peer_) const;
  virtual ~ACL() {};
  virtual int32_t getperms() const {
    return mperms;
  }
  virtual void setperms(int32_t m_) {
    mperms=m_; bs_.set(0);
  }
  virtual const org::apache::zookeeper::data::Id& getid() const {
    return mid;
  }
  virtual org::apache::zookeeper::data::Id& getid() {
    bs_.set(1);return mid;
  }
}; // end record ACL
} // end namespace data
} // end namespace zookeeper
} // end namespace apache
} // end namespace org
namespace org {
namespace apache {
namespace zookeeper {
namespace data {
class Stat : public ::hadoop::Record {
private:
  int64_t mczxid;
  int64_t mmzxid;
  int64_t mctime;
  int64_t mmtime;
  int32_t mversion;
  int32_t mcversion;
  int32_t maversion;
  int64_t mephemeralOwner;
  int32_t mdataLength;
  int32_t mnumChildren;
  int64_t mpzxid;
  mutable std::bitset<11> bs_;
public:
  virtual void serialize(::hadoop::OArchive& a_, const char* tag) const;
  virtual void deserialize(::hadoop::IArchive& a_, const char* tag);
  virtual const ::std::string& type() const;
  virtual const ::std::string& signature() const;
  virtual bool validate() const;
  virtual bool operator<(const Stat& peer_) const;
  virtual bool operator==(const Stat& peer_) const;
  virtual ~Stat() {};
  virtual int64_t getczxid() const {
    return mczxid;
  }
  virtual void setczxid(int64_t m_) {
    mczxid=m_; bs_.set(0);
  }
  virtual int64_t getmzxid() const {
    return mmzxid;
  }
  virtual void setmzxid(int64_t m_) {
    mmzxid=m_; bs_.set(1);
  }
  virtual int64_t getctime() const {
    return mctime;
  }
  virtual void setctime(int64_t m_) {
    mctime=m_; bs_.set(2);
  }
  virtual int64_t getmtime() const {
    return mmtime;
  }
  virtual void setmtime(int64_t m_) {
    mmtime=m_; bs_.set(3);
  }
  virtual int32_t getversion() const {
    return mversion;
  }
  virtual void setversion(int32_t m_) {
    mversion=m_; bs_.set(4);
  }
  virtual int32_t getcversion() const {
    return mcversion;
  }
  virtual void setcversion(int32_t m_) {
    mcversion=m_; bs_.set(5);
  }
  virtual int32_t getaversion() const {
    return maversion;
  }
  virtual void setaversion(int32_t m_) {
    maversion=m_; bs_.set(6);
  }
  virtual int64_t getephemeralOwner() const {
    return mephemeralOwner;
  }
  virtual void setephemeralOwner(int64_t m_) {
    mephemeralOwner=m_; bs_.set(7);
  }
  virtual int32_t getdataLength() const {
    return mdataLength;
  }
  virtual void setdataLength(int32_t m_) {
    mdataLength=m_; bs_.set(8);
  }
  virtual int32_t getnumChildren() const {
    return mnumChildren;
  }
  virtual void setnumChildren(int32_t m_) {
    mnumChildren=m_; bs_.set(9);
  }
  virtual int64_t getpzxid() const {
    return mpzxid;
  }
  virtual void setpzxid(int64_t m_) {
    mpzxid=m_; bs_.set(10);
  }
}; // end record Stat
} // end namespace data
} // end namespace zookeeper
} // end namespace apache
} // end namespace org
namespace org {
namespace apache {
namespace zookeeper {
namespace data {
class StatPersisted : public ::hadoop::Record {
private:
  int64_t mczxid;
  int64_t mmzxid;
  int64_t mctime;
  int64_t mmtime;
  int32_t mversion;
  int32_t mcversion;
  int32_t maversion;
  int64_t mephemeralOwner;
  int64_t mpzxid;
  mutable std::bitset<9> bs_;
public:
  virtual void serialize(::hadoop::OArchive& a_, const char* tag) const;
  virtual void deserialize(::hadoop::IArchive& a_, const char* tag);
  virtual const ::std::string& type() const;
  virtual const ::std::string& signature() const;
  virtual bool validate() const;
  virtual bool operator<(const StatPersisted& peer_) const;
  virtual bool operator==(const StatPersisted& peer_) const;
  virtual ~StatPersisted() {};
  virtual int64_t getczxid() const {
    return mczxid;
  }
  virtual void setczxid(int64_t m_) {
    mczxid=m_; bs_.set(0);
  }
  virtual int64_t getmzxid() const {
    return mmzxid;
  }
  virtual void setmzxid(int64_t m_) {
    mmzxid=m_; bs_.set(1);
  }
  virtual int64_t getctime() const {
    return mctime;
  }
  virtual void setctime(int64_t m_) {
    mctime=m_; bs_.set(2);
  }
  virtual int64_t getmtime() const {
    return mmtime;
  }
  virtual void setmtime(int64_t m_) {
    mmtime=m_; bs_.set(3);
  }
  virtual int32_t getversion() const {
    return mversion;
  }
  virtual void setversion(int32_t m_) {
    mversion=m_; bs_.set(4);
  }
  virtual int32_t getcversion() const {
    return mcversion;
  }
  virtual void setcversion(int32_t m_) {
    mcversion=m_; bs_.set(5);
  }
  virtual int32_t getaversion() const {
    return maversion;
  }
  virtual void setaversion(int32_t m_) {
    maversion=m_; bs_.set(6);
  }
  virtual int64_t getephemeralOwner() const {
    return mephemeralOwner;
  }
  virtual void setephemeralOwner(int64_t m_) {
    mephemeralOwner=m_; bs_.set(7);
  }
  virtual int64_t getpzxid() const {
    return mpzxid;
  }
  virtual void setpzxid(int64_t m_) {
    mpzxid=m_; bs_.set(8);
  }
}; // end record StatPersisted
} // end namespace data
} // end namespace zookeeper
} // end namespace apache
} // end namespace org
namespace org {
namespace apache {
namespace zookeeper {
namespace proto {
class ConnectRequest : public ::hadoop::Record {
private:
  int32_t mprotocolVersion;
  int64_t mlastZxidSeen;
  int32_t mtimeOut;
  int64_t msessionId;
   ::std::string mpasswd;
  mutable std::bitset<5> bs_;
public:
  virtual void serialize(::hadoop::OArchive& a_, const char* tag) const;
  virtual void deserialize(::hadoop::IArchive& a_, const char* tag);
  virtual const ::std::string& type() const;
  virtual const ::std::string& signature() const;
  virtual bool validate() const;
  virtual bool operator<(const ConnectRequest& peer_) const;
  virtual bool operator==(const ConnectRequest& peer_) const;
  virtual ~ConnectRequest() {};
  virtual int32_t getprotocolVersion() const {
    return mprotocolVersion;
  }
  virtual void setprotocolVersion(int32_t m_) {
    mprotocolVersion=m_; bs_.set(0);
  }
  virtual int64_t getlastZxidSeen() const {
    return mlastZxidSeen;
  }
  virtual void setlastZxidSeen(int64_t m_) {
    mlastZxidSeen=m_; bs_.set(1);
  }
  virtual int32_t gettimeOut() const {
    return mtimeOut;
  }
  virtual void settimeOut(int32_t m_) {
    mtimeOut=m_; bs_.set(2);
  }
  virtual int64_t getsessionId() const {
    return msessionId;
  }
  virtual void setsessionId(int64_t m_) {
    msessionId=m_; bs_.set(3);
  }
  virtual const  ::std::string& getpasswd() const {
    return mpasswd;
  }
  virtual  ::std::string& getpasswd() {
    bs_.set(4);return mpasswd;
  }
}; // end record ConnectRequest
} // end namespace proto
} // end namespace zookeeper
} // end namespace apache
} // end namespace org
namespace org {
namespace apache {
namespace zookeeper {
namespace proto {
class ConnectResponse : public ::hadoop::Record {
private:
  int32_t mprotocolVersion;
  int32_t mtimeOut;
  int64_t msessionId;
   ::std::string mpasswd;
  mutable std::bitset<4> bs_;
public:
  virtual void serialize(::hadoop::OArchive& a_, const char* tag) const;
  virtual void deserialize(::hadoop::IArchive& a_, const char* tag);
  virtual const ::std::string& type() const;
  virtual const ::std::string& signature() const;
  virtual bool validate() const;
  virtual bool operator<(const ConnectResponse& peer_) const;
  virtual bool operator==(const ConnectResponse& peer_) const;
  virtual ~ConnectResponse() {};
  virtual int32_t getprotocolVersion() const {
    return mprotocolVersion;
  }
  virtual void setprotocolVersion(int32_t m_) {
    mprotocolVersion=m_; bs_.set(0);
  }
  virtual int32_t gettimeOut() const {
    return mtimeOut;
  }
  virtual void settimeOut(int32_t m_) {
    mtimeOut=m_; bs_.set(1);
  }
  virtual int64_t getsessionId() const {
    return msessionId;
  }
  virtual void setsessionId(int64_t m_) {
    msessionId=m_; bs_.set(2);
  }
  virtual const  ::std::string& getpasswd() const {
    return mpasswd;
  }
  virtual  ::std::string& getpasswd() {
    bs_.set(3);return mpasswd;
  }
}; // end record ConnectResponse
} // end namespace proto
} // end namespace zookeeper
} // end namespace apache
} // end namespace org
namespace org {
namespace apache {
namespace zookeeper {
namespace proto {
class SetWatches : public ::hadoop::Record {
private:
  int64_t mrelativeZxid;
   ::std::vector< ::std::string> mdataWatches;
   ::std::vector< ::std::string> mexistWatches;
   ::std::vector< ::std::string> mchildWatches;
  mutable std::bitset<4> bs_;
public:
  virtual void serialize(::hadoop::OArchive& a_, const char* tag) const;
  virtual void deserialize(::hadoop::IArchive& a_, const char* tag);
  virtual const ::std::string& type() const;
  virtual const ::std::string& signature() const;
  virtual bool validate() const;
  virtual bool operator<(const SetWatches& peer_) const;
  virtual bool operator==(const SetWatches& peer_) const;
  virtual ~SetWatches() {};
  virtual int64_t getrelativeZxid() const {
    return mrelativeZxid;
  }
  virtual void setrelativeZxid(int64_t m_) {
    mrelativeZxid=m_; bs_.set(0);
  }
  virtual const  ::std::vector< ::std::string>& getdataWatches() const {
    return mdataWatches;
  }
  virtual  ::std::vector< ::std::string>& getdataWatches() {
    bs_.set(1);return mdataWatches;
  }
  virtual const  ::std::vector< ::std::string>& getexistWatches() const {
    return mexistWatches;
  }
  virtual  ::std::vector< ::std::string>& getexistWatches() {
    bs_.set(2);return mexistWatches;
  }
  virtual const  ::std::vector< ::std::string>& getchildWatches() const {
    return mchildWatches;
  }
  virtual  ::std::vector< ::std::string>& getchildWatches() {
    bs_.set(3);return mchildWatches;
  }
}; // end record SetWatches
} // end namespace proto
} // end namespace zookeeper
} // end namespace apache
} // end namespace org
namespace org {
namespace apache {
namespace zookeeper {
namespace proto {
class RequestHeader : public ::hadoop::Record {
private:
  int32_t mxid;
  int32_t mtype;
  mutable std::bitset<2> bs_;
public:
  virtual void serialize(::hadoop::OArchive& a_, const char* tag) const;
  virtual void deserialize(::hadoop::IArchive& a_, const char* tag);
  virtual const ::std::string& type() const;
  virtual const ::std::string& signature() const;
  virtual bool validate() const;
  virtual bool operator<(const RequestHeader& peer_) const;
  virtual bool operator==(const RequestHeader& peer_) const;
  virtual ~RequestHeader() {};
  virtual int32_t getxid() const {
    return mxid;
  }
  virtual void setxid(int32_t m_) {
    mxid=m_; bs_.set(0);
  }
  virtual int32_t gettype() const {
    return mtype;
  }
  virtual void settype(int32_t m_) {
    mtype=m_; bs_.set(1);
  }
}; // end record RequestHeader
} // end namespace proto
} // end namespace zookeeper
} // end namespace apache
} // end namespace org
namespace org {
namespace apache {
namespace zookeeper {
namespace proto {
class MultiHeader : public ::hadoop::Record {
private:
  int32_t mtype;
  bool mdone;
  int32_t merr;
  mutable std::bitset<3> bs_;
public:
  virtual void serialize(::hadoop::OArchive& a_, const char* tag) const;
  virtual void deserialize(::hadoop::IArchive& a_, const char* tag);
  virtual const ::std::string& type() const;
  virtual const ::std::string& signature() const;
  virtual bool validate() const;
  virtual bool operator<(const MultiHeader& peer_) const;
  virtual bool operator==(const MultiHeader& peer_) const;
  virtual ~MultiHeader() {};
  virtual int32_t gettype() const {
    return mtype;
  }
  virtual void settype(int32_t m_) {
    mtype=m_; bs_.set(0);
  }
  virtual bool getdone() const {
    return mdone;
  }
  virtual void setdone(bool m_) {
    mdone=m_; bs_.set(1);
  }
  virtual int32_t geterr() const {
    return merr;
  }
  virtual void seterr(int32_t m_) {
    merr=m_; bs_.set(2);
  }
}; // end record MultiHeader
} // end namespace proto
} // end namespace zookeeper
} // end namespace apache
} // end namespace org
namespace org {
namespace apache {
namespace zookeeper {
namespace proto {
class AuthPacket : public ::hadoop::Record {
private:
  int32_t mtype;
   ::std::string mscheme;
   ::std::string mauth;
  mutable std::bitset<3> bs_;
public:
  virtual void serialize(::hadoop::OArchive& a_, const char* tag) const;
  virtual void deserialize(::hadoop::IArchive& a_, const char* tag);
  virtual const ::std::string& type() const;
  virtual const ::std::string& signature() const;
  virtual bool validate() const;
  virtual bool operator<(const AuthPacket& peer_) const;
  virtual bool operator==(const AuthPacket& peer_) const;
  virtual ~AuthPacket() {};
  virtual int32_t gettype() const {
    return mtype;
  }
  virtual void settype(int32_t m_) {
    mtype=m_; bs_.set(0);
  }
  virtual const  ::std::string& getscheme() const {
    return mscheme;
  }
  virtual  ::std::string& getscheme() {
    bs_.set(1);return mscheme;
  }
  virtual const  ::std::string& getauth() const {
    return mauth;
  }
  virtual  ::std::string& getauth() {
    bs_.set(2);return mauth;
  }
}; // end record AuthPacket
} // end namespace proto
} // end namespace zookeeper
} // end namespace apache
} // end namespace org
namespace org {
namespace apache {
namespace zookeeper {
namespace proto {
class ReplyHeader : public ::hadoop::Record {
private:
  int32_t mxid;
  int64_t mzxid;
  int32_t merr;
  mutable std::bitset<3> bs_;
public:
  virtual void serialize(::hadoop::OArchive& a_, const char* tag) const;
  virtual void deserialize(::hadoop::IArchive& a_, const char* tag);
  virtual const ::std::string& type() const;
  virtual const ::std::string& signature() const;
  virtual bool validate() const;
  virtual bool operator<(const ReplyHeader& peer_) const;
  virtual bool operator==(const ReplyHeader& peer_) const;
  virtual ~ReplyHeader() {};
  virtual int32_t getxid() const {
    return mxid;
  }
  virtual void setxid(int32_t m_) {
    mxid=m_; bs_.set(0);
  }
  virtual int64_t getzxid() const {
    return mzxid;
  }
  virtual void setzxid(int64_t m_) {
    mzxid=m_; bs_.set(1);
  }
  virtual int32_t geterr() const {
    return merr;
  }
  virtual void seterr(int32_t m_) {
    merr=m_; bs_.set(2);
  }
}; // end record ReplyHeader
} // end namespace proto
} // end namespace zookeeper
} // end namespace apache
} // end namespace org
namespace org {
namespace apache {
namespace zookeeper {
namespace proto {
class GetDataRequest : public ::hadoop::Record {
private:
   ::std::string mpath;
  bool mwatch;
  mutable std::bitset<2> bs_;
public:
  virtual void serialize(::hadoop::OArchive& a_, const char* tag) const;
  virtual void deserialize(::hadoop::IArchive& a_, const char* tag);
  virtual const ::std::string& type() const;
  virtual const ::std::string& signature() const;
  virtual bool validate() const;
  virtual bool operator<(const GetDataRequest& peer_) const;
  virtual bool operator==(const GetDataRequest& peer_) const;
  virtual ~GetDataRequest() {};
  virtual const  ::std::string& getpath() const {
    return mpath;
  }
  virtual  ::std::string& getpath() {
    bs_.set(0);return mpath;
  }
  virtual bool getwatch() const {
    return mwatch;
  }
  virtual void setwatch(bool m_) {
    mwatch=m_; bs_.set(1);
  }
}; // end record GetDataRequest
} // end namespace proto
} // end namespace zookeeper
} // end namespace apache
} // end namespace org
namespace org {
namespace apache {
namespace zookeeper {
namespace proto {
class SetDataRequest : public ::hadoop::Record {
private:
   ::std::string mpath;
   ::std::string mdata;
  int32_t mversion;
  mutable std::bitset<3> bs_;
public:
  virtual void serialize(::hadoop::OArchive& a_, const char* tag) const;
  virtual void deserialize(::hadoop::IArchive& a_, const char* tag);
  virtual const ::std::string& type() const;
  virtual const ::std::string& signature() const;
  virtual bool validate() const;
  virtual bool operator<(const SetDataRequest& peer_) const;
  virtual bool operator==(const SetDataRequest& peer_) const;
  virtual ~SetDataRequest() {};
  virtual const  ::std::string& getpath() const {
    return mpath;
  }
  virtual  ::std::string& getpath() {
    bs_.set(0);return mpath;
  }
  virtual const  ::std::string& getdata() const {
    return mdata;
  }
  virtual  ::std::string& getdata() {
    bs_.set(1);return mdata;
  }
  virtual int32_t getversion() const {
    return mversion;
  }
  virtual void setversion(int32_t m_) {
    mversion=m_; bs_.set(2);
  }
}; // end record SetDataRequest
} // end namespace proto
} // end namespace zookeeper
} // end namespace apache
} // end namespace org
namespace org {
namespace apache {
namespace zookeeper {
namespace proto {
class SetDataResponse : public ::hadoop::Record {
private:
  org::apache::zookeeper::data::Stat mstat;
  mutable std::bitset<1> bs_;
public:
  virtual void serialize(::hadoop::OArchive& a_, const char* tag) const;
  virtual void deserialize(::hadoop::IArchive& a_, const char* tag);
  virtual const ::std::string& type() const;
  virtual const ::std::string& signature() const;
  virtual bool validate() const;
  virtual bool operator<(const SetDataResponse& peer_) const;
  virtual bool operator==(const SetDataResponse& peer_) const;
  virtual ~SetDataResponse() {};
  virtual const org::apache::zookeeper::data::Stat& getstat() const {
    return mstat;
  }
  virtual org::apache::zookeeper::data::Stat& getstat() {
    bs_.set(0);return mstat;
  }
}; // end record SetDataResponse
} // end namespace proto
} // end namespace zookeeper
} // end namespace apache
} // end namespace org
namespace org {
namespace apache {
namespace zookeeper {
namespace proto {
class GetSASLRequest : public ::hadoop::Record {
private:
   ::std::string mtoken;
  mutable std::bitset<1> bs_;
public:
  virtual void serialize(::hadoop::OArchive& a_, const char* tag) const;
  virtual void deserialize(::hadoop::IArchive& a_, const char* tag);
  virtual const ::std::string& type() const;
  virtual const ::std::string& signature() const;
  virtual bool validate() const;
  virtual bool operator<(const GetSASLRequest& peer_) const;
  virtual bool operator==(const GetSASLRequest& peer_) const;
  virtual ~GetSASLRequest() {};
  virtual const  ::std::string& gettoken() const {
    return mtoken;
  }
  virtual  ::std::string& gettoken() {
    bs_.set(0);return mtoken;
  }
}; // end record GetSASLRequest
} // end namespace proto
} // end namespace zookeeper
} // end namespace apache
} // end namespace org
namespace org {
namespace apache {
namespace zookeeper {
namespace proto {
class SetSASLRequest : public ::hadoop::Record {
private:
   ::std::string mtoken;
  mutable std::bitset<1> bs_;
public:
  virtual void serialize(::hadoop::OArchive& a_, const char* tag) const;
  virtual void deserialize(::hadoop::IArchive& a_, const char* tag);
  virtual const ::std::string& type() const;
  virtual const ::std::string& signature() const;
  virtual bool validate() const;
  virtual bool operator<(const SetSASLRequest& peer_) const;
  virtual bool operator==(const SetSASLRequest& peer_) const;
  virtual ~SetSASLRequest() {};
  virtual const  ::std::string& gettoken() const {
    return mtoken;
  }
  virtual  ::std::string& gettoken() {
    bs_.set(0);return mtoken;
  }
}; // end record SetSASLRequest
} // end namespace proto
} // end namespace zookeeper
} // end namespace apache
} // end namespace org
namespace org {
namespace apache {
namespace zookeeper {
namespace proto {
class SetSASLResponse : public ::hadoop::Record {
private:
   ::std::string mtoken;
  mutable std::bitset<1> bs_;
public:
  virtual void serialize(::hadoop::OArchive& a_, const char* tag) const;
  virtual void deserialize(::hadoop::IArchive& a_, const char* tag);
  virtual const ::std::string& type() const;
  virtual const ::std::string& signature() const;
  virtual bool validate() const;
  virtual bool operator<(const SetSASLResponse& peer_) const;
  virtual bool operator==(const SetSASLResponse& peer_) const;
  virtual ~SetSASLResponse() {};
  virtual const  ::std::string& gettoken() const {
    return mtoken;
  }
  virtual  ::std::string& gettoken() {
    bs_.set(0);return mtoken;
  }
}; // end record SetSASLResponse
} // end namespace proto
} // end namespace zookeeper
} // end namespace apache
} // end namespace org
namespace org {
namespace apache {
namespace zookeeper {
namespace proto {
class CreateRequest : public ::hadoop::Record {
private:
   ::std::string mpath;
   ::std::string mdata;
   ::std::vector<org::apache::zookeeper::data::ACL> macl;
  int32_t mflags;
  mutable std::bitset<4> bs_;
public:
  virtual void serialize(::hadoop::OArchive& a_, const char* tag) const;
  virtual void deserialize(::hadoop::IArchive& a_, const char* tag);
  virtual const ::std::string& type() const;
  virtual const ::std::string& signature() const;
  virtual bool validate() const;
  virtual bool operator<(const CreateRequest& peer_) const;
  virtual bool operator==(const CreateRequest& peer_) const;
  virtual ~CreateRequest() {};
  virtual const  ::std::string& getpath() const {
    return mpath;
  }
  virtual  ::std::string& getpath() {
    bs_.set(0);return mpath;
  }
  virtual const  ::std::string& getdata() const {
    return mdata;
  }
  virtual  ::std::string& getdata() {
    bs_.set(1);return mdata;
  }
  virtual const  ::std::vector<org::apache::zookeeper::data::ACL>& getacl() const {
    return macl;
  }
  virtual  ::std::vector<org::apache::zookeeper::data::ACL>& getacl() {
    bs_.set(2);return macl;
  }
  virtual int32_t getflags() const {
    return mflags;
  }
  virtual void setflags(int32_t m_) {
    mflags=m_; bs_.set(3);
  }
}; // end record CreateRequest
} // end namespace proto
} // end namespace zookeeper
} // end namespace apache
} // end namespace org
namespace org {
namespace apache {
namespace zookeeper {
namespace proto {
class DeleteRequest : public ::hadoop::Record {
private:
   ::std::string mpath;
  int32_t mversion;
  mutable std::bitset<2> bs_;
public:
  virtual void serialize(::hadoop::OArchive& a_, const char* tag) const;
  virtual void deserialize(::hadoop::IArchive& a_, const char* tag);
  virtual const ::std::string& type() const;
  virtual const ::std::string& signature() const;
  virtual bool validate() const;
  virtual bool operator<(const DeleteRequest& peer_) const;
  virtual bool operator==(const DeleteRequest& peer_) const;
  virtual ~DeleteRequest() {};
  virtual const  ::std::string& getpath() const {
    return mpath;
  }
  virtual  ::std::string& getpath() {
    bs_.set(0);return mpath;
  }
  virtual int32_t getversion() const {
    return mversion;
  }
  virtual void setversion(int32_t m_) {
    mversion=m_; bs_.set(1);
  }
}; // end record DeleteRequest
} // end namespace proto
} // end namespace zookeeper
} // end namespace apache
} // end namespace org
namespace org {
namespace apache {
namespace zookeeper {
namespace proto {
class GetChildrenRequest : public ::hadoop::Record {
private:
   ::std::string mpath;
  bool mwatch;
  mutable std::bitset<2> bs_;
public:
  virtual void serialize(::hadoop::OArchive& a_, const char* tag) const;
  virtual void deserialize(::hadoop::IArchive& a_, const char* tag);
  virtual const ::std::string& type() const;
  virtual const ::std::string& signature() const;
  virtual bool validate() const;
  virtual bool operator<(const GetChildrenRequest& peer_) const;
  virtual bool operator==(const GetChildrenRequest& peer_) const;
  virtual ~GetChildrenRequest() {};
  virtual const  ::std::string& getpath() const {
    return mpath;
  }
  virtual  ::std::string& getpath() {
    bs_.set(0);return mpath;
  }
  virtual bool getwatch() const {
    return mwatch;
  }
  virtual void setwatch(bool m_) {
    mwatch=m_; bs_.set(1);
  }
}; // end record GetChildrenRequest
} // end namespace proto
} // end namespace zookeeper
} // end namespace apache
} // end namespace org
namespace org {
namespace apache {
namespace zookeeper {
namespace proto {
class GetChildren2Request : public ::hadoop::Record {
private:
   ::std::string mpath;
  bool mwatch;
  mutable std::bitset<2> bs_;
public:
  virtual void serialize(::hadoop::OArchive& a_, const char* tag) const;
  virtual void deserialize(::hadoop::IArchive& a_, const char* tag);
  virtual const ::std::string& type() const;
  virtual const ::std::string& signature() const;
  virtual bool validate() const;
  virtual bool operator<(const GetChildren2Request& peer_) const;
  virtual bool operator==(const GetChildren2Request& peer_) const;
  virtual ~GetChildren2Request() {};
  virtual const  ::std::string& getpath() const {
    return mpath;
  }
  virtual  ::std::string& getpath() {
    bs_.set(0);return mpath;
  }
  virtual bool getwatch() const {
    return mwatch;
  }
  virtual void setwatch(bool m_) {
    mwatch=m_; bs_.set(1);
  }
}; // end record GetChildren2Request
} // end namespace proto
} // end namespace zookeeper
} // end namespace apache
} // end namespace org
namespace org {
namespace apache {
namespace zookeeper {
namespace proto {
class CheckVersionRequest : public ::hadoop::Record {
private:
   ::std::string mpath;
  int32_t mversion;
  mutable std::bitset<2> bs_;
public:
  virtual void serialize(::hadoop::OArchive& a_, const char* tag) const;
  virtual void deserialize(::hadoop::IArchive& a_, const char* tag);
  virtual const ::std::string& type() const;
  virtual const ::std::string& signature() const;
  virtual bool validate() const;
  virtual bool operator<(const CheckVersionRequest& peer_) const;
  virtual bool operator==(const CheckVersionRequest& peer_) const;
  virtual ~CheckVersionRequest() {};
  virtual const  ::std::string& getpath() const {
    return mpath;
  }
  virtual  ::std::string& getpath() {
    bs_.set(0);return mpath;
  }
  virtual int32_t getversion() const {
    return mversion;
  }
  virtual void setversion(int32_t m_) {
    mversion=m_; bs_.set(1);
  }
}; // end record CheckVersionRequest
} // end namespace proto
} // end namespace zookeeper
} // end namespace apache
} // end namespace org
namespace org {
namespace apache {
namespace zookeeper {
namespace proto {
class GetMaxChildrenRequest : public ::hadoop::Record {
private:
   ::std::string mpath;
  mutable std::bitset<1> bs_;
public:
  virtual void serialize(::hadoop::OArchive& a_, const char* tag) const;
  virtual void deserialize(::hadoop::IArchive& a_, const char* tag);
  virtual const ::std::string& type() const;
  virtual const ::std::string& signature() const;
  virtual bool validate() const;
  virtual bool operator<(const GetMaxChildrenRequest& peer_) const;
  virtual bool operator==(const GetMaxChildrenRequest& peer_) const;
  virtual ~GetMaxChildrenRequest() {};
  virtual const  ::std::string& getpath() const {
    return mpath;
  }
  virtual  ::std::string& getpath() {
    bs_.set(0);return mpath;
  }
}; // end record GetMaxChildrenRequest
} // end namespace proto
} // end namespace zookeeper
} // end namespace apache
} // end namespace org
namespace org {
namespace apache {
namespace zookeeper {
namespace proto {
class GetMaxChildrenResponse : public ::hadoop::Record {
private:
  int32_t mmax;
  mutable std::bitset<1> bs_;
public:
  virtual void serialize(::hadoop::OArchive& a_, const char* tag) const;
  virtual void deserialize(::hadoop::IArchive& a_, const char* tag);
  virtual const ::std::string& type() const;
  virtual const ::std::string& signature() const;
  virtual bool validate() const;
  virtual bool operator<(const GetMaxChildrenResponse& peer_) const;
  virtual bool operator==(const GetMaxChildrenResponse& peer_) const;
  virtual ~GetMaxChildrenResponse() {};
  virtual int32_t getmax() const {
    return mmax;
  }
  virtual void setmax(int32_t m_) {
    mmax=m_; bs_.set(0);
  }
}; // end record GetMaxChildrenResponse
} // end namespace proto
} // end namespace zookeeper
} // end namespace apache
} // end namespace org
namespace org {
namespace apache {
namespace zookeeper {
namespace proto {
class SetMaxChildrenRequest : public ::hadoop::Record {
private:
   ::std::string mpath;
  int32_t mmax;
  mutable std::bitset<2> bs_;
public:
  virtual void serialize(::hadoop::OArchive& a_, const char* tag) const;
  virtual void deserialize(::hadoop::IArchive& a_, const char* tag);
  virtual const ::std::string& type() const;
  virtual const ::std::string& signature() const;
  virtual bool validate() const;
  virtual bool operator<(const SetMaxChildrenRequest& peer_) const;
  virtual bool operator==(const SetMaxChildrenRequest& peer_) const;
  virtual ~SetMaxChildrenRequest() {};
  virtual const  ::std::string& getpath() const {
    return mpath;
  }
  virtual  ::std::string& getpath() {
    bs_.set(0);return mpath;
  }
  virtual int32_t getmax() const {
    return mmax;
  }
  virtual void setmax(int32_t m_) {
    mmax=m_; bs_.set(1);
  }
}; // end record SetMaxChildrenRequest
} // end namespace proto
} // end namespace zookeeper
} // end namespace apache
} // end namespace org
namespace org {
namespace apache {
namespace zookeeper {
namespace proto {
class SyncRequest : public ::hadoop::Record {
private:
   ::std::string mpath;
  mutable std::bitset<1> bs_;
public:
  virtual void serialize(::hadoop::OArchive& a_, const char* tag) const;
  virtual void deserialize(::hadoop::IArchive& a_, const char* tag);
  virtual const ::std::string& type() const;
  virtual const ::std::string& signature() const;
  virtual bool validate() const;
  virtual bool operator<(const SyncRequest& peer_) const;
  virtual bool operator==(const SyncRequest& peer_) const;
  virtual ~SyncRequest() {};
  virtual const  ::std::string& getpath() const {
    return mpath;
  }
  virtual  ::std::string& getpath() {
    bs_.set(0);return mpath;
  }
}; // end record SyncRequest
} // end namespace proto
} // end namespace zookeeper
} // end namespace apache
} // end namespace org
namespace org {
namespace apache {
namespace zookeeper {
namespace proto {
class SyncResponse : public ::hadoop::Record {
private:
   ::std::string mpath;
  mutable std::bitset<1> bs_;
public:
  virtual void serialize(::hadoop::OArchive& a_, const char* tag) const;
  virtual void deserialize(::hadoop::IArchive& a_, const char* tag);
  virtual const ::std::string& type() const;
  virtual const ::std::string& signature() const;
  virtual bool validate() const;
  virtual bool operator<(const SyncResponse& peer_) const;
  virtual bool operator==(const SyncResponse& peer_) const;
  virtual ~SyncResponse() {};
  virtual const  ::std::string& getpath() const {
    return mpath;
  }
  virtual  ::std::string& getpath() {
    bs_.set(0);return mpath;
  }
}; // end record SyncResponse
} // end namespace proto
} // end namespace zookeeper
} // end namespace apache
} // end namespace org
namespace org {
namespace apache {
namespace zookeeper {
namespace proto {
class GetACLRequest : public ::hadoop::Record {
private:
   ::std::string mpath;
  mutable std::bitset<1> bs_;
public:
  virtual void serialize(::hadoop::OArchive& a_, const char* tag) const;
  virtual void deserialize(::hadoop::IArchive& a_, const char* tag);
  virtual const ::std::string& type() const;
  virtual const ::std::string& signature() const;
  virtual bool validate() const;
  virtual bool operator<(const GetACLRequest& peer_) const;
  virtual bool operator==(const GetACLRequest& peer_) const;
  virtual ~GetACLRequest() {};
  virtual const  ::std::string& getpath() const {
    return mpath;
  }
  virtual  ::std::string& getpath() {
    bs_.set(0);return mpath;
  }
}; // end record GetACLRequest
} // end namespace proto
} // end namespace zookeeper
} // end namespace apache
} // end namespace org
namespace org {
namespace apache {
namespace zookeeper {
namespace proto {
class SetACLRequest : public ::hadoop::Record {
private:
   ::std::string mpath;
   ::std::vector<org::apache::zookeeper::data::ACL> macl;
  int32_t mversion;
  mutable std::bitset<3> bs_;
public:
  virtual void serialize(::hadoop::OArchive& a_, const char* tag) const;
  virtual void deserialize(::hadoop::IArchive& a_, const char* tag);
  virtual const ::std::string& type() const;
  virtual const ::std::string& signature() const;
  virtual bool validate() const;
  virtual bool operator<(const SetACLRequest& peer_) const;
  virtual bool operator==(const SetACLRequest& peer_) const;
  virtual ~SetACLRequest() {};
  virtual const  ::std::string& getpath() const {
    return mpath;
  }
  virtual  ::std::string& getpath() {
    bs_.set(0);return mpath;
  }
  virtual const  ::std::vector<org::apache::zookeeper::data::ACL>& getacl() const {
    return macl;
  }
  virtual  ::std::vector<org::apache::zookeeper::data::ACL>& getacl() {
    bs_.set(1);return macl;
  }
  virtual int32_t getversion() const {
    return mversion;
  }
  virtual void setversion(int32_t m_) {
    mversion=m_; bs_.set(2);
  }
}; // end record SetACLRequest
} // end namespace proto
} // end namespace zookeeper
} // end namespace apache
} // end namespace org
namespace org {
namespace apache {
namespace zookeeper {
namespace proto {
class SetACLResponse : public ::hadoop::Record {
private:
  org::apache::zookeeper::data::Stat mstat;
  mutable std::bitset<1> bs_;
public:
  virtual void serialize(::hadoop::OArchive& a_, const char* tag) const;
  virtual void deserialize(::hadoop::IArchive& a_, const char* tag);
  virtual const ::std::string& type() const;
  virtual const ::std::string& signature() const;
  virtual bool validate() const;
  virtual bool operator<(const SetACLResponse& peer_) const;
  virtual bool operator==(const SetACLResponse& peer_) const;
  virtual ~SetACLResponse() {};
  virtual const org::apache::zookeeper::data::Stat& getstat() const {
    return mstat;
  }
  virtual org::apache::zookeeper::data::Stat& getstat() {
    bs_.set(0);return mstat;
  }
}; // end record SetACLResponse
} // end namespace proto
} // end namespace zookeeper
} // end namespace apache
} // end namespace org
namespace org {
namespace apache {
namespace zookeeper {
namespace proto {
class WatcherEvent : public ::hadoop::Record {
private:
  int32_t mtype;
  int32_t mstate;
   ::std::string mpath;
  mutable std::bitset<3> bs_;
public:
  virtual void serialize(::hadoop::OArchive& a_, const char* tag) const;
  virtual void deserialize(::hadoop::IArchive& a_, const char* tag);
  virtual const ::std::string& type() const;
  virtual const ::std::string& signature() const;
  virtual bool validate() const;
  virtual bool operator<(const WatcherEvent& peer_) const;
  virtual bool operator==(const WatcherEvent& peer_) const;
  virtual ~WatcherEvent() {};
  virtual int32_t gettype() const {
    return mtype;
  }
  virtual void settype(int32_t m_) {
    mtype=m_; bs_.set(0);
  }
  virtual int32_t getstate() const {
    return mstate;
  }
  virtual void setstate(int32_t m_) {
    mstate=m_; bs_.set(1);
  }
  virtual const  ::std::string& getpath() const {
    return mpath;
  }
  virtual  ::std::string& getpath() {
    bs_.set(2);return mpath;
  }
}; // end record WatcherEvent
} // end namespace proto
} // end namespace zookeeper
} // end namespace apache
} // end namespace org
namespace org {
namespace apache {
namespace zookeeper {
namespace proto {
class ErrorResponse : public ::hadoop::Record {
private:
  int32_t merr;
  mutable std::bitset<1> bs_;
public:
  virtual void serialize(::hadoop::OArchive& a_, const char* tag) const;
  virtual void deserialize(::hadoop::IArchive& a_, const char* tag);
  virtual const ::std::string& type() const;
  virtual const ::std::string& signature() const;
  virtual bool validate() const;
  virtual bool operator<(const ErrorResponse& peer_) const;
  virtual bool operator==(const ErrorResponse& peer_) const;
  virtual ~ErrorResponse() {};
  virtual int32_t geterr() const {
    return merr;
  }
  virtual void seterr(int32_t m_) {
    merr=m_; bs_.set(0);
  }
}; // end record ErrorResponse
} // end namespace proto
} // end namespace zookeeper
} // end namespace apache
} // end namespace org
namespace org {
namespace apache {
namespace zookeeper {
namespace proto {
class CreateResponse : public ::hadoop::Record {
private:
   ::std::string mpath;
  mutable std::bitset<1> bs_;
public:
  virtual void serialize(::hadoop::OArchive& a_, const char* tag) const;
  virtual void deserialize(::hadoop::IArchive& a_, const char* tag);
  virtual const ::std::string& type() const;
  virtual const ::std::string& signature() const;
  virtual bool validate() const;
  virtual bool operator<(const CreateResponse& peer_) const;
  virtual bool operator==(const CreateResponse& peer_) const;
  virtual ~CreateResponse() {};
  virtual const  ::std::string& getpath() const {
    return mpath;
  }
  virtual  ::std::string& getpath() {
    bs_.set(0);return mpath;
  }
}; // end record CreateResponse
} // end namespace proto
} // end namespace zookeeper
} // end namespace apache
} // end namespace org
namespace org {
namespace apache {
namespace zookeeper {
namespace proto {
class ExistsRequest : public ::hadoop::Record {
private:
   ::std::string mpath;
  bool mwatch;
  mutable std::bitset<2> bs_;
public:
  virtual void serialize(::hadoop::OArchive& a_, const char* tag) const;
  virtual void deserialize(::hadoop::IArchive& a_, const char* tag);
  virtual const ::std::string& type() const;
  virtual const ::std::string& signature() const;
  virtual bool validate() const;
  virtual bool operator<(const ExistsRequest& peer_) const;
  virtual bool operator==(const ExistsRequest& peer_) const;
  virtual ~ExistsRequest() {};
  virtual const  ::std::string& getpath() const {
    return mpath;
  }
  virtual  ::std::string& getpath() {
    bs_.set(0);return mpath;
  }
  virtual bool getwatch() const {
    return mwatch;
  }
  virtual void setwatch(bool m_) {
    mwatch=m_; bs_.set(1);
  }
}; // end record ExistsRequest
} // end namespace proto
} // end namespace zookeeper
} // end namespace apache
} // end namespace org
namespace org {
namespace apache {
namespace zookeeper {
namespace proto {
class ExistsResponse : public ::hadoop::Record {
private:
  org::apache::zookeeper::data::Stat mstat;
  mutable std::bitset<1> bs_;
public:
  virtual void serialize(::hadoop::OArchive& a_, const char* tag) const;
  virtual void deserialize(::hadoop::IArchive& a_, const char* tag);
  virtual const ::std::string& type() const;
  virtual const ::std::string& signature() const;
  virtual bool validate() const;
  virtual bool operator<(const ExistsResponse& peer_) const;
  virtual bool operator==(const ExistsResponse& peer_) const;
  virtual ~ExistsResponse() {};
  virtual const org::apache::zookeeper::data::Stat& getstat() const {
    return mstat;
  }
  virtual org::apache::zookeeper::data::Stat& getstat() {
    bs_.set(0);return mstat;
  }
}; // end record ExistsResponse
} // end namespace proto
} // end namespace zookeeper
} // end namespace apache
} // end namespace org
namespace org {
namespace apache {
namespace zookeeper {
namespace proto {
class GetDataResponse : public ::hadoop::Record {
private:
   ::std::string mdata;
  org::apache::zookeeper::data::Stat mstat;
  mutable std::bitset<2> bs_;
public:
  virtual void serialize(::hadoop::OArchive& a_, const char* tag) const;
  virtual void deserialize(::hadoop::IArchive& a_, const char* tag);
  virtual const ::std::string& type() const;
  virtual const ::std::string& signature() const;
  virtual bool validate() const;
  virtual bool operator<(const GetDataResponse& peer_) const;
  virtual bool operator==(const GetDataResponse& peer_) const;
  virtual ~GetDataResponse() {};
  virtual const  ::std::string& getdata() const {
    return mdata;
  }
  virtual  ::std::string& getdata() {
    bs_.set(0);return mdata;
  }
  virtual const org::apache::zookeeper::data::Stat& getstat() const {
    return mstat;
  }
  virtual org::apache::zookeeper::data::Stat& getstat() {
    bs_.set(1);return mstat;
  }
}; // end record GetDataResponse
} // end namespace proto
} // end namespace zookeeper
} // end namespace apache
} // end namespace org
namespace org {
namespace apache {
namespace zookeeper {
namespace proto {
class GetChildrenResponse : public ::hadoop::Record {
private:
   ::std::vector< ::std::string> mchildren;
  mutable std::bitset<1> bs_;
public:
  virtual void serialize(::hadoop::OArchive& a_, const char* tag) const;
  virtual void deserialize(::hadoop::IArchive& a_, const char* tag);
  virtual const ::std::string& type() const;
  virtual const ::std::string& signature() const;
  virtual bool validate() const;
  virtual bool operator<(const GetChildrenResponse& peer_) const;
  virtual bool operator==(const GetChildrenResponse& peer_) const;
  virtual ~GetChildrenResponse() {};
  virtual const  ::std::vector< ::std::string>& getchildren() const {
    return mchildren;
  }
  virtual  ::std::vector< ::std::string>& getchildren() {
    bs_.set(0);return mchildren;
  }
}; // end record GetChildrenResponse
} // end namespace proto
} // end namespace zookeeper
} // end namespace apache
} // end namespace org
namespace org {
namespace apache {
namespace zookeeper {
namespace proto {
class GetChildren2Response : public ::hadoop::Record {
private:
   ::std::vector< ::std::string> mchildren;
  org::apache::zookeeper::data::Stat mstat;
  mutable std::bitset<2> bs_;
public:
  virtual void serialize(::hadoop::OArchive& a_, const char* tag) const;
  virtual void deserialize(::hadoop::IArchive& a_, const char* tag);
  virtual const ::std::string& type() const;
  virtual const ::std::string& signature() const;
  virtual bool validate() const;
  virtual bool operator<(const GetChildren2Response& peer_) const;
  virtual bool operator==(const GetChildren2Response& peer_) const;
  virtual ~GetChildren2Response() {};
  virtual const  ::std::vector< ::std::string>& getchildren() const {
    return mchildren;
  }
  virtual  ::std::vector< ::std::string>& getchildren() {
    bs_.set(0);return mchildren;
  }
  virtual const org::apache::zookeeper::data::Stat& getstat() const {
    return mstat;
  }
  virtual org::apache::zookeeper::data::Stat& getstat() {
    bs_.set(1);return mstat;
  }
}; // end record GetChildren2Response
} // end namespace proto
} // end namespace zookeeper
} // end namespace apache
} // end namespace org
namespace org {
namespace apache {
namespace zookeeper {
namespace proto {
class GetACLResponse : public ::hadoop::Record {
private:
   ::std::vector<org::apache::zookeeper::data::ACL> macl;
  org::apache::zookeeper::data::Stat mstat;
  mutable std::bitset<2> bs_;
public:
  virtual void serialize(::hadoop::OArchive& a_, const char* tag) const;
  virtual void deserialize(::hadoop::IArchive& a_, const char* tag);
  virtual const ::std::string& type() const;
  virtual const ::std::string& signature() const;
  virtual bool validate() const;
  virtual bool operator<(const GetACLResponse& peer_) const;
  virtual bool operator==(const GetACLResponse& peer_) const;
  virtual ~GetACLResponse() {};
  virtual const  ::std::vector<org::apache::zookeeper::data::ACL>& getacl() const {
    return macl;
  }
  virtual  ::std::vector<org::apache::zookeeper::data::ACL>& getacl() {
    bs_.set(0);return macl;
  }
  virtual const org::apache::zookeeper::data::Stat& getstat() const {
    return mstat;
  }
  virtual org::apache::zookeeper::data::Stat& getstat() {
    bs_.set(1);return mstat;
  }
}; // end record GetACLResponse
} // end namespace proto
} // end namespace zookeeper
} // end namespace apache
} // end namespace org
namespace org {
namespace apache {
namespace zookeeper {
namespace server {
namespace quorum {
class LearnerInfo : public ::hadoop::Record {
private:
  int64_t mserverid;
  int32_t mprotocolVersion;
  mutable std::bitset<2> bs_;
public:
  virtual void serialize(::hadoop::OArchive& a_, const char* tag) const;
  virtual void deserialize(::hadoop::IArchive& a_, const char* tag);
  virtual const ::std::string& type() const;
  virtual const ::std::string& signature() const;
  virtual bool validate() const;
  virtual bool operator<(const LearnerInfo& peer_) const;
  virtual bool operator==(const LearnerInfo& peer_) const;
  virtual ~LearnerInfo() {};
  virtual int64_t getserverid() const {
    return mserverid;
  }
  virtual void setserverid(int64_t m_) {
    mserverid=m_; bs_.set(0);
  }
  virtual int32_t getprotocolVersion() const {
    return mprotocolVersion;
  }
  virtual void setprotocolVersion(int32_t m_) {
    mprotocolVersion=m_; bs_.set(1);
  }
}; // end record LearnerInfo
} // end namespace quorum
} // end namespace server
} // end namespace zookeeper
} // end namespace apache
} // end namespace org
namespace org {
namespace apache {
namespace zookeeper {
namespace server {
namespace quorum {
class QuorumPacket : public ::hadoop::Record {
private:
  int32_t mtype;
  int64_t mzxid;
   ::std::string mdata;
   ::std::vector<org::apache::zookeeper::data::Id> mauthinfo;
  mutable std::bitset<4> bs_;
public:
  virtual void serialize(::hadoop::OArchive& a_, const char* tag) const;
  virtual void deserialize(::hadoop::IArchive& a_, const char* tag);
  virtual const ::std::string& type() const;
  virtual const ::std::string& signature() const;
  virtual bool validate() const;
  virtual bool operator<(const QuorumPacket& peer_) const;
  virtual bool operator==(const QuorumPacket& peer_) const;
  virtual ~QuorumPacket() {};
  virtual int32_t gettype() const {
    return mtype;
  }
  virtual void settype(int32_t m_) {
    mtype=m_; bs_.set(0);
  }
  virtual int64_t getzxid() const {
    return mzxid;
  }
  virtual void setzxid(int64_t m_) {
    mzxid=m_; bs_.set(1);
  }
  virtual const  ::std::string& getdata() const {
    return mdata;
  }
  virtual  ::std::string& getdata() {
    bs_.set(2);return mdata;
  }
  virtual const  ::std::vector<org::apache::zookeeper::data::Id>& getauthinfo() const {
    return mauthinfo;
  }
  virtual  ::std::vector<org::apache::zookeeper::data::Id>& getauthinfo() {
    bs_.set(3);return mauthinfo;
  }
}; // end record QuorumPacket
} // end namespace quorum
} // end namespace server
} // end namespace zookeeper
} // end namespace apache
} // end namespace org
namespace org {
namespace apache {
namespace zookeeper {
namespace server {
namespace persistence {
class FileHeader : public ::hadoop::Record {
private:
  int32_t mmagic;
  int32_t mversion;
  int64_t mdbid;
  mutable std::bitset<3> bs_;
public:
  virtual void serialize(::hadoop::OArchive& a_, const char* tag) const;
  virtual void deserialize(::hadoop::IArchive& a_, const char* tag);
  virtual const ::std::string& type() const;
  virtual const ::std::string& signature() const;
  virtual bool validate() const;
  virtual bool operator<(const FileHeader& peer_) const;
  virtual bool operator==(const FileHeader& peer_) const;
  virtual ~FileHeader() {};
  virtual int32_t getmagic() const {
    return mmagic;
  }
  virtual void setmagic(int32_t m_) {
    mmagic=m_; bs_.set(0);
  }
  virtual int32_t getversion() const {
    return mversion;
  }
  virtual void setversion(int32_t m_) {
    mversion=m_; bs_.set(1);
  }
  virtual int64_t getdbid() const {
    return mdbid;
  }
  virtual void setdbid(int64_t m_) {
    mdbid=m_; bs_.set(2);
  }
}; // end record FileHeader
} // end namespace persistence
} // end namespace server
} // end namespace zookeeper
} // end namespace apache
} // end namespace org
namespace org {
namespace apache {
namespace zookeeper {
namespace txn {
class TxnHeader : public ::hadoop::Record {
private:
  int64_t mclientId;
  int32_t mcxid;
  int64_t mzxid;
  int64_t mtime;
  int32_t mtype;
  mutable std::bitset<5> bs_;
public:
  virtual void serialize(::hadoop::OArchive& a_, const char* tag) const;
  virtual void deserialize(::hadoop::IArchive& a_, const char* tag);
  virtual const ::std::string& type() const;
  virtual const ::std::string& signature() const;
  virtual bool validate() const;
  virtual bool operator<(const TxnHeader& peer_) const;
  virtual bool operator==(const TxnHeader& peer_) const;
  virtual ~TxnHeader() {};
  virtual int64_t getclientId() const {
    return mclientId;
  }
  virtual void setclientId(int64_t m_) {
    mclientId=m_; bs_.set(0);
  }
  virtual int32_t getcxid() const {
    return mcxid;
  }
  virtual void setcxid(int32_t m_) {
    mcxid=m_; bs_.set(1);
  }
  virtual int64_t getzxid() const {
    return mzxid;
  }
  virtual void setzxid(int64_t m_) {
    mzxid=m_; bs_.set(2);
  }
  virtual int64_t gettime() const {
    return mtime;
  }
  virtual void settime(int64_t m_) {
    mtime=m_; bs_.set(3);
  }
  virtual int32_t gettype() const {
    return mtype;
  }
  virtual void settype(int32_t m_) {
    mtype=m_; bs_.set(4);
  }
}; // end record TxnHeader
} // end namespace txn
} // end namespace zookeeper
} // end namespace apache
} // end namespace org
namespace org {
namespace apache {
namespace zookeeper {
namespace txn {
class CreateTxnV0 : public ::hadoop::Record {
private:
   ::std::string mpath;
   ::std::string mdata;
   ::std::vector<org::apache::zookeeper::data::ACL> macl;
  bool mephemeral;
  mutable std::bitset<4> bs_;
public:
  virtual void serialize(::hadoop::OArchive& a_, const char* tag) const;
  virtual void deserialize(::hadoop::IArchive& a_, const char* tag);
  virtual const ::std::string& type() const;
  virtual const ::std::string& signature() const;
  virtual bool validate() const;
  virtual bool operator<(const CreateTxnV0& peer_) const;
  virtual bool operator==(const CreateTxnV0& peer_) const;
  virtual ~CreateTxnV0() {};
  virtual const  ::std::string& getpath() const {
    return mpath;
  }
  virtual  ::std::string& getpath() {
    bs_.set(0);return mpath;
  }
  virtual const  ::std::string& getdata() const {
    return mdata;
  }
  virtual  ::std::string& getdata() {
    bs_.set(1);return mdata;
  }
  virtual const  ::std::vector<org::apache::zookeeper::data::ACL>& getacl() const {
    return macl;
  }
  virtual  ::std::vector<org::apache::zookeeper::data::ACL>& getacl() {
    bs_.set(2);return macl;
  }
  virtual bool getephemeral() const {
    return mephemeral;
  }
  virtual void setephemeral(bool m_) {
    mephemeral=m_; bs_.set(3);
  }
}; // end record CreateTxnV0
} // end namespace txn
} // end namespace zookeeper
} // end namespace apache
} // end namespace org
namespace org {
namespace apache {
namespace zookeeper {
namespace txn {
class CreateTxn : public ::hadoop::Record {
private:
   ::std::string mpath;
   ::std::string mdata;
   ::std::vector<org::apache::zookeeper::data::ACL> macl;
  bool mephemeral;
  int32_t mparentCVersion;
  mutable std::bitset<5> bs_;
public:
  virtual void serialize(::hadoop::OArchive& a_, const char* tag) const;
  virtual void deserialize(::hadoop::IArchive& a_, const char* tag);
  virtual const ::std::string& type() const;
  virtual const ::std::string& signature() const;
  virtual bool validate() const;
  virtual bool operator<(const CreateTxn& peer_) const;
  virtual bool operator==(const CreateTxn& peer_) const;
  virtual ~CreateTxn() {};
  virtual const  ::std::string& getpath() const {
    return mpath;
  }
  virtual  ::std::string& getpath() {
    bs_.set(0);return mpath;
  }
  virtual const  ::std::string& getdata() const {
    return mdata;
  }
  virtual  ::std::string& getdata() {
    bs_.set(1);return mdata;
  }
  virtual const  ::std::vector<org::apache::zookeeper::data::ACL>& getacl() const {
    return macl;
  }
  virtual  ::std::vector<org::apache::zookeeper::data::ACL>& getacl() {
    bs_.set(2);return macl;
  }
  virtual bool getephemeral() const {
    return mephemeral;
  }
  virtual void setephemeral(bool m_) {
    mephemeral=m_; bs_.set(3);
  }
  virtual int32_t getparentCVersion() const {
    return mparentCVersion;
  }
  virtual void setparentCVersion(int32_t m_) {
    mparentCVersion=m_; bs_.set(4);
  }
}; // end record CreateTxn
} // end namespace txn
} // end namespace zookeeper
} // end namespace apache
} // end namespace org
namespace org {
namespace apache {
namespace zookeeper {
namespace txn {
class DeleteTxn : public ::hadoop::Record {
private:
   ::std::string mpath;
  mutable std::bitset<1> bs_;
public:
  virtual void serialize(::hadoop::OArchive& a_, const char* tag) const;
  virtual void deserialize(::hadoop::IArchive& a_, const char* tag);
  virtual const ::std::string& type() const;
  virtual const ::std::string& signature() const;
  virtual bool validate() const;
  virtual bool operator<(const DeleteTxn& peer_) const;
  virtual bool operator==(const DeleteTxn& peer_) const;
  virtual ~DeleteTxn() {};
  virtual const  ::std::string& getpath() const {
    return mpath;
  }
  virtual  ::std::string& getpath() {
    bs_.set(0);return mpath;
  }
}; // end record DeleteTxn
} // end namespace txn
} // end namespace zookeeper
} // end namespace apache
} // end namespace org
namespace org {
namespace apache {
namespace zookeeper {
namespace txn {
class SetDataTxn : public ::hadoop::Record {
private:
   ::std::string mpath;
   ::std::string mdata;
  int32_t mversion;
  mutable std::bitset<3> bs_;
public:
  virtual void serialize(::hadoop::OArchive& a_, const char* tag) const;
  virtual void deserialize(::hadoop::IArchive& a_, const char* tag);
  virtual const ::std::string& type() const;
  virtual const ::std::string& signature() const;
  virtual bool validate() const;
  virtual bool operator<(const SetDataTxn& peer_) const;
  virtual bool operator==(const SetDataTxn& peer_) const;
  virtual ~SetDataTxn() {};
  virtual const  ::std::string& getpath() const {
    return mpath;
  }
  virtual  ::std::string& getpath() {
    bs_.set(0);return mpath;
  }
  virtual const  ::std::string& getdata() const {
    return mdata;
  }
  virtual  ::std::string& getdata() {
    bs_.set(1);return mdata;
  }
  virtual int32_t getversion() const {
    return mversion;
  }
  virtual void setversion(int32_t m_) {
    mversion=m_; bs_.set(2);
  }
}; // end record SetDataTxn
} // end namespace txn
} // end namespace zookeeper
} // end namespace apache
} // end namespace org
namespace org {
namespace apache {
namespace zookeeper {
namespace txn {
class CheckVersionTxn : public ::hadoop::Record {
private:
   ::std::string mpath;
  int32_t mversion;
  mutable std::bitset<2> bs_;
public:
  virtual void serialize(::hadoop::OArchive& a_, const char* tag) const;
  virtual void deserialize(::hadoop::IArchive& a_, const char* tag);
  virtual const ::std::string& type() const;
  virtual const ::std::string& signature() const;
  virtual bool validate() const;
  virtual bool operator<(const CheckVersionTxn& peer_) const;
  virtual bool operator==(const CheckVersionTxn& peer_) const;
  virtual ~CheckVersionTxn() {};
  virtual const  ::std::string& getpath() const {
    return mpath;
  }
  virtual  ::std::string& getpath() {
    bs_.set(0);return mpath;
  }
  virtual int32_t getversion() const {
    return mversion;
  }
  virtual void setversion(int32_t m_) {
    mversion=m_; bs_.set(1);
  }
}; // end record CheckVersionTxn
} // end namespace txn
} // end namespace zookeeper
} // end namespace apache
} // end namespace org
namespace org {
namespace apache {
namespace zookeeper {
namespace txn {
class SetACLTxn : public ::hadoop::Record {
private:
   ::std::string mpath;
   ::std::vector<org::apache::zookeeper::data::ACL> macl;
  int32_t mversion;
  mutable std::bitset<3> bs_;
public:
  virtual void serialize(::hadoop::OArchive& a_, const char* tag) const;
  virtual void deserialize(::hadoop::IArchive& a_, const char* tag);
  virtual const ::std::string& type() const;
  virtual const ::std::string& signature() const;
  virtual bool validate() const;
  virtual bool operator<(const SetACLTxn& peer_) const;
  virtual bool operator==(const SetACLTxn& peer_) const;
  virtual ~SetACLTxn() {};
  virtual const  ::std::string& getpath() const {
    return mpath;
  }
  virtual  ::std::string& getpath() {
    bs_.set(0);return mpath;
  }
  virtual const  ::std::vector<org::apache::zookeeper::data::ACL>& getacl() const {
    return macl;
  }
  virtual  ::std::vector<org::apache::zookeeper::data::ACL>& getacl() {
    bs_.set(1);return macl;
  }
  virtual int32_t getversion() const {
    return mversion;
  }
  virtual void setversion(int32_t m_) {
    mversion=m_; bs_.set(2);
  }
}; // end record SetACLTxn
} // end namespace txn
} // end namespace zookeeper
} // end namespace apache
} // end namespace org
namespace org {
namespace apache {
namespace zookeeper {
namespace txn {
class SetMaxChildrenTxn : public ::hadoop::Record {
private:
   ::std::string mpath;
  int32_t mmax;
  mutable std::bitset<2> bs_;
public:
  virtual void serialize(::hadoop::OArchive& a_, const char* tag) const;
  virtual void deserialize(::hadoop::IArchive& a_, const char* tag);
  virtual const ::std::string& type() const;
  virtual const ::std::string& signature() const;
  virtual bool validate() const;
  virtual bool operator<(const SetMaxChildrenTxn& peer_) const;
  virtual bool operator==(const SetMaxChildrenTxn& peer_) const;
  virtual ~SetMaxChildrenTxn() {};
  virtual const  ::std::string& getpath() const {
    return mpath;
  }
  virtual  ::std::string& getpath() {
    bs_.set(0);return mpath;
  }
  virtual int32_t getmax() const {
    return mmax;
  }
  virtual void setmax(int32_t m_) {
    mmax=m_; bs_.set(1);
  }
}; // end record SetMaxChildrenTxn
} // end namespace txn
} // end namespace zookeeper
} // end namespace apache
} // end namespace org
namespace org {
namespace apache {
namespace zookeeper {
namespace txn {
class CreateSessionTxn : public ::hadoop::Record {
private:
  int32_t mtimeOut;
  mutable std::bitset<1> bs_;
public:
  virtual void serialize(::hadoop::OArchive& a_, const char* tag) const;
  virtual void deserialize(::hadoop::IArchive& a_, const char* tag);
  virtual const ::std::string& type() const;
  virtual const ::std::string& signature() const;
  virtual bool validate() const;
  virtual bool operator<(const CreateSessionTxn& peer_) const;
  virtual bool operator==(const CreateSessionTxn& peer_) const;
  virtual ~CreateSessionTxn() {};
  virtual int32_t gettimeOut() const {
    return mtimeOut;
  }
  virtual void settimeOut(int32_t m_) {
    mtimeOut=m_; bs_.set(0);
  }
}; // end record CreateSessionTxn
} // end namespace txn
} // end namespace zookeeper
} // end namespace apache
} // end namespace org
namespace org {
namespace apache {
namespace zookeeper {
namespace txn {
class ErrorTxn : public ::hadoop::Record {
private:
  int32_t merr;
  mutable std::bitset<1> bs_;
public:
  virtual void serialize(::hadoop::OArchive& a_, const char* tag) const;
  virtual void deserialize(::hadoop::IArchive& a_, const char* tag);
  virtual const ::std::string& type() const;
  virtual const ::std::string& signature() const;
  virtual bool validate() const;
  virtual bool operator<(const ErrorTxn& peer_) const;
  virtual bool operator==(const ErrorTxn& peer_) const;
  virtual ~ErrorTxn() {};
  virtual int32_t geterr() const {
    return merr;
  }
  virtual void seterr(int32_t m_) {
    merr=m_; bs_.set(0);
  }
}; // end record ErrorTxn
} // end namespace txn
} // end namespace zookeeper
} // end namespace apache
} // end namespace org
namespace org {
namespace apache {
namespace zookeeper {
namespace txn {
class Txn : public ::hadoop::Record {
private:
  int32_t mtype;
   ::std::string mdata;
  mutable std::bitset<2> bs_;
public:
  virtual void serialize(::hadoop::OArchive& a_, const char* tag) const;
  virtual void deserialize(::hadoop::IArchive& a_, const char* tag);
  virtual const ::std::string& type() const;
  virtual const ::std::string& signature() const;
  virtual bool validate() const;
  virtual bool operator<(const Txn& peer_) const;
  virtual bool operator==(const Txn& peer_) const;
  virtual ~Txn() {};
  virtual int32_t gettype() const {
    return mtype;
  }
  virtual void settype(int32_t m_) {
    mtype=m_; bs_.set(0);
  }
  virtual const  ::std::string& getdata() const {
    return mdata;
  }
  virtual  ::std::string& getdata() {
    bs_.set(1);return mdata;
  }
}; // end record Txn
} // end namespace txn
} // end namespace zookeeper
} // end namespace apache
} // end namespace org
namespace org {
namespace apache {
namespace zookeeper {
namespace txn {
class MultiTxn : public ::hadoop::Record {
private:
   ::std::vector<org::apache::zookeeper::txn::Txn> mtxns;
  mutable std::bitset<1> bs_;
public:
  virtual void serialize(::hadoop::OArchive& a_, const char* tag) const;
  virtual void deserialize(::hadoop::IArchive& a_, const char* tag);
  virtual const ::std::string& type() const;
  virtual const ::std::string& signature() const;
  virtual bool validate() const;
  virtual bool operator<(const MultiTxn& peer_) const;
  virtual bool operator==(const MultiTxn& peer_) const;
  virtual ~MultiTxn() {};
  virtual const  ::std::vector<org::apache::zookeeper::txn::Txn>& gettxns() const {
    return mtxns;
  }
  virtual  ::std::vector<org::apache::zookeeper::txn::Txn>& gettxns() {
    bs_.set(0);return mtxns;
  }
}; // end record MultiTxn
} // end namespace txn
} // end namespace zookeeper
} // end namespace apache
} // end namespace org
#endif //ZOOKEEPER_JUTE_HH__
