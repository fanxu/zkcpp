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

#include "zookeeper.jute.hh"
void org::apache::zookeeper::data::Id::serialize(::hadoop::OArchive& a_, const char* tag) const {
  if (!validate()) throw new ::hadoop::IOException("All fields not set.");
  a_.startRecord(*this,tag);
  a_.serialize(mscheme,"scheme");
  bs_.reset(0);
  a_.serialize(mid,"id");
  bs_.reset(1);
  a_.endRecord(*this,tag);
  return;
}
void org::apache::zookeeper::data::Id::deserialize(::hadoop::IArchive& a_, const char* tag) {
  a_.startRecord(*this,tag);
  a_.deserialize(mscheme,"scheme");
  bs_.set(0);
  a_.deserialize(mid,"id");
  bs_.set(1);
  a_.endRecord(*this,tag);
  return;
}
bool org::apache::zookeeper::data::Id::validate() const {
  if (bs_.size() != bs_.count()) return false;
  return true;
}
bool org::apache::zookeeper::data::Id::operator< (const org::apache::zookeeper::data::Id& peer_) const {
  return (1
    && (mscheme < peer_.mscheme)
    && (mid < peer_.mid)
  );
}
bool org::apache::zookeeper::data::Id::operator== (const org::apache::zookeeper::data::Id& peer_) const {
  return (1
    && (mscheme == peer_.mscheme)
    && (mid == peer_.mid)
  );
}
const ::std::string&org::apache::zookeeper::data::Id::type() const {
  static const ::std::string type_("Id");
  return type_;
}
const ::std::string&org::apache::zookeeper::data::Id::signature() const {
  static const ::std::string sig_("LId(ss)");
  return sig_;
}
void org::apache::zookeeper::data::ACL::serialize(::hadoop::OArchive& a_, const char* tag) const {
  if (!validate()) throw new ::hadoop::IOException("All fields not set.");
  a_.startRecord(*this,tag);
  a_.serialize(mperms,"perms");
  bs_.reset(0);
  a_.serialize(mid,"id");
  bs_.reset(1);
  a_.endRecord(*this,tag);
  return;
}
void org::apache::zookeeper::data::ACL::deserialize(::hadoop::IArchive& a_, const char* tag) {
  a_.startRecord(*this,tag);
  a_.deserialize(mperms,"perms");
  bs_.set(0);
  a_.deserialize(mid,"id");
  bs_.set(1);
  a_.endRecord(*this,tag);
  return;
}
bool org::apache::zookeeper::data::ACL::validate() const {
  if (bs_.size() != bs_.count()) return false;
  if (!mid.validate()) return false;
  return true;
}
bool org::apache::zookeeper::data::ACL::operator< (const org::apache::zookeeper::data::ACL& peer_) const {
  return (1
    && (mperms < peer_.mperms)
    && (mid < peer_.mid)
  );
}
bool org::apache::zookeeper::data::ACL::operator== (const org::apache::zookeeper::data::ACL& peer_) const {
  return (1
    && (mperms == peer_.mperms)
    && (mid == peer_.mid)
  );
}
const ::std::string&org::apache::zookeeper::data::ACL::type() const {
  static const ::std::string type_("ACL");
  return type_;
}
const ::std::string&org::apache::zookeeper::data::ACL::signature() const {
  static const ::std::string sig_("LACL(iLId(ss))");
  return sig_;
}
void org::apache::zookeeper::data::Stat::serialize(::hadoop::OArchive& a_, const char* tag) const {
  if (!validate()) throw new ::hadoop::IOException("All fields not set.");
  a_.startRecord(*this,tag);
  a_.serialize(mczxid,"czxid");
  bs_.reset(0);
  a_.serialize(mmzxid,"mzxid");
  bs_.reset(1);
  a_.serialize(mctime,"ctime");
  bs_.reset(2);
  a_.serialize(mmtime,"mtime");
  bs_.reset(3);
  a_.serialize(mversion,"version");
  bs_.reset(4);
  a_.serialize(mcversion,"cversion");
  bs_.reset(5);
  a_.serialize(maversion,"aversion");
  bs_.reset(6);
  a_.serialize(mephemeralOwner,"ephemeralOwner");
  bs_.reset(7);
  a_.serialize(mdataLength,"dataLength");
  bs_.reset(8);
  a_.serialize(mnumChildren,"numChildren");
  bs_.reset(9);
  a_.serialize(mpzxid,"pzxid");
  bs_.reset(10);
  a_.endRecord(*this,tag);
  return;
}
void org::apache::zookeeper::data::Stat::deserialize(::hadoop::IArchive& a_, const char* tag) {
  a_.startRecord(*this,tag);
  a_.deserialize(mczxid,"czxid");
  bs_.set(0);
  a_.deserialize(mmzxid,"mzxid");
  bs_.set(1);
  a_.deserialize(mctime,"ctime");
  bs_.set(2);
  a_.deserialize(mmtime,"mtime");
  bs_.set(3);
  a_.deserialize(mversion,"version");
  bs_.set(4);
  a_.deserialize(mcversion,"cversion");
  bs_.set(5);
  a_.deserialize(maversion,"aversion");
  bs_.set(6);
  a_.deserialize(mephemeralOwner,"ephemeralOwner");
  bs_.set(7);
  a_.deserialize(mdataLength,"dataLength");
  bs_.set(8);
  a_.deserialize(mnumChildren,"numChildren");
  bs_.set(9);
  a_.deserialize(mpzxid,"pzxid");
  bs_.set(10);
  a_.endRecord(*this,tag);
  return;
}
bool org::apache::zookeeper::data::Stat::validate() const {
  if (bs_.size() != bs_.count()) return false;
  return true;
}
bool org::apache::zookeeper::data::Stat::operator< (const org::apache::zookeeper::data::Stat& peer_) const {
  return (1
    && (mczxid < peer_.mczxid)
    && (mmzxid < peer_.mmzxid)
    && (mctime < peer_.mctime)
    && (mmtime < peer_.mmtime)
    && (mversion < peer_.mversion)
    && (mcversion < peer_.mcversion)
    && (maversion < peer_.maversion)
    && (mephemeralOwner < peer_.mephemeralOwner)
    && (mdataLength < peer_.mdataLength)
    && (mnumChildren < peer_.mnumChildren)
    && (mpzxid < peer_.mpzxid)
  );
}
bool org::apache::zookeeper::data::Stat::operator== (const org::apache::zookeeper::data::Stat& peer_) const {
  return (1
    && (mczxid == peer_.mczxid)
    && (mmzxid == peer_.mmzxid)
    && (mctime == peer_.mctime)
    && (mmtime == peer_.mmtime)
    && (mversion == peer_.mversion)
    && (mcversion == peer_.mcversion)
    && (maversion == peer_.maversion)
    && (mephemeralOwner == peer_.mephemeralOwner)
    && (mdataLength == peer_.mdataLength)
    && (mnumChildren == peer_.mnumChildren)
    && (mpzxid == peer_.mpzxid)
  );
}
const ::std::string&org::apache::zookeeper::data::Stat::type() const {
  static const ::std::string type_("Stat");
  return type_;
}
const ::std::string&org::apache::zookeeper::data::Stat::signature() const {
  static const ::std::string sig_("LStat(lllliiiliil)");
  return sig_;
}
void org::apache::zookeeper::data::StatPersisted::serialize(::hadoop::OArchive& a_, const char* tag) const {
  if (!validate()) throw new ::hadoop::IOException("All fields not set.");
  a_.startRecord(*this,tag);
  a_.serialize(mczxid,"czxid");
  bs_.reset(0);
  a_.serialize(mmzxid,"mzxid");
  bs_.reset(1);
  a_.serialize(mctime,"ctime");
  bs_.reset(2);
  a_.serialize(mmtime,"mtime");
  bs_.reset(3);
  a_.serialize(mversion,"version");
  bs_.reset(4);
  a_.serialize(mcversion,"cversion");
  bs_.reset(5);
  a_.serialize(maversion,"aversion");
  bs_.reset(6);
  a_.serialize(mephemeralOwner,"ephemeralOwner");
  bs_.reset(7);
  a_.serialize(mpzxid,"pzxid");
  bs_.reset(8);
  a_.endRecord(*this,tag);
  return;
}
void org::apache::zookeeper::data::StatPersisted::deserialize(::hadoop::IArchive& a_, const char* tag) {
  a_.startRecord(*this,tag);
  a_.deserialize(mczxid,"czxid");
  bs_.set(0);
  a_.deserialize(mmzxid,"mzxid");
  bs_.set(1);
  a_.deserialize(mctime,"ctime");
  bs_.set(2);
  a_.deserialize(mmtime,"mtime");
  bs_.set(3);
  a_.deserialize(mversion,"version");
  bs_.set(4);
  a_.deserialize(mcversion,"cversion");
  bs_.set(5);
  a_.deserialize(maversion,"aversion");
  bs_.set(6);
  a_.deserialize(mephemeralOwner,"ephemeralOwner");
  bs_.set(7);
  a_.deserialize(mpzxid,"pzxid");
  bs_.set(8);
  a_.endRecord(*this,tag);
  return;
}
bool org::apache::zookeeper::data::StatPersisted::validate() const {
  if (bs_.size() != bs_.count()) return false;
  return true;
}
bool org::apache::zookeeper::data::StatPersisted::operator< (const org::apache::zookeeper::data::StatPersisted& peer_) const {
  return (1
    && (mczxid < peer_.mczxid)
    && (mmzxid < peer_.mmzxid)
    && (mctime < peer_.mctime)
    && (mmtime < peer_.mmtime)
    && (mversion < peer_.mversion)
    && (mcversion < peer_.mcversion)
    && (maversion < peer_.maversion)
    && (mephemeralOwner < peer_.mephemeralOwner)
    && (mpzxid < peer_.mpzxid)
  );
}
bool org::apache::zookeeper::data::StatPersisted::operator== (const org::apache::zookeeper::data::StatPersisted& peer_) const {
  return (1
    && (mczxid == peer_.mczxid)
    && (mmzxid == peer_.mmzxid)
    && (mctime == peer_.mctime)
    && (mmtime == peer_.mmtime)
    && (mversion == peer_.mversion)
    && (mcversion == peer_.mcversion)
    && (maversion == peer_.maversion)
    && (mephemeralOwner == peer_.mephemeralOwner)
    && (mpzxid == peer_.mpzxid)
  );
}
const ::std::string&org::apache::zookeeper::data::StatPersisted::type() const {
  static const ::std::string type_("StatPersisted");
  return type_;
}
const ::std::string&org::apache::zookeeper::data::StatPersisted::signature() const {
  static const ::std::string sig_("LStatPersisted(lllliiill)");
  return sig_;
}
void org::apache::zookeeper::proto::ConnectRequest::serialize(::hadoop::OArchive& a_, const char* tag) const {
  if (!validate()) throw new ::hadoop::IOException("All fields not set.");
  a_.startRecord(*this,tag);
  a_.serialize(mprotocolVersion,"protocolVersion");
  bs_.reset(0);
  a_.serialize(mlastZxidSeen,"lastZxidSeen");
  bs_.reset(1);
  a_.serialize(mtimeOut,"timeOut");
  bs_.reset(2);
  a_.serialize(msessionId,"sessionId");
  bs_.reset(3);
  a_.serialize(mpasswd,mpasswd.length(),"passwd");
  bs_.reset(4);
  a_.endRecord(*this,tag);
  return;
}
void org::apache::zookeeper::proto::ConnectRequest::deserialize(::hadoop::IArchive& a_, const char* tag) {
  a_.startRecord(*this,tag);
  a_.deserialize(mprotocolVersion,"protocolVersion");
  bs_.set(0);
  a_.deserialize(mlastZxidSeen,"lastZxidSeen");
  bs_.set(1);
  a_.deserialize(mtimeOut,"timeOut");
  bs_.set(2);
  a_.deserialize(msessionId,"sessionId");
  bs_.set(3);
  { size_t len=0; a_.deserialize(mpasswd,len,"passwd");}
  bs_.set(4);
  a_.endRecord(*this,tag);
  return;
}
bool org::apache::zookeeper::proto::ConnectRequest::validate() const {
  if (bs_.size() != bs_.count()) return false;
  return true;
}
bool org::apache::zookeeper::proto::ConnectRequest::operator< (const org::apache::zookeeper::proto::ConnectRequest& peer_) const {
  return (1
    && (mprotocolVersion < peer_.mprotocolVersion)
    && (mlastZxidSeen < peer_.mlastZxidSeen)
    && (mtimeOut < peer_.mtimeOut)
    && (msessionId < peer_.msessionId)
    && (mpasswd < peer_.mpasswd)
  );
}
bool org::apache::zookeeper::proto::ConnectRequest::operator== (const org::apache::zookeeper::proto::ConnectRequest& peer_) const {
  return (1
    && (mprotocolVersion == peer_.mprotocolVersion)
    && (mlastZxidSeen == peer_.mlastZxidSeen)
    && (mtimeOut == peer_.mtimeOut)
    && (msessionId == peer_.msessionId)
    && (mpasswd == peer_.mpasswd)
  );
}
const ::std::string&org::apache::zookeeper::proto::ConnectRequest::type() const {
  static const ::std::string type_("ConnectRequest");
  return type_;
}
const ::std::string&org::apache::zookeeper::proto::ConnectRequest::signature() const {
  static const ::std::string sig_("LConnectRequest(ililB)");
  return sig_;
}
void org::apache::zookeeper::proto::ConnectResponse::serialize(::hadoop::OArchive& a_, const char* tag) const {
  if (!validate()) throw new ::hadoop::IOException("All fields not set.");
  a_.startRecord(*this,tag);
  a_.serialize(mprotocolVersion,"protocolVersion");
  bs_.reset(0);
  a_.serialize(mtimeOut,"timeOut");
  bs_.reset(1);
  a_.serialize(msessionId,"sessionId");
  bs_.reset(2);
  a_.serialize(mpasswd,mpasswd.length(),"passwd");
  bs_.reset(3);
  a_.endRecord(*this,tag);
  return;
}
void org::apache::zookeeper::proto::ConnectResponse::deserialize(::hadoop::IArchive& a_, const char* tag) {
  a_.startRecord(*this,tag);
  a_.deserialize(mprotocolVersion,"protocolVersion");
  bs_.set(0);
  a_.deserialize(mtimeOut,"timeOut");
  bs_.set(1);
  a_.deserialize(msessionId,"sessionId");
  bs_.set(2);
  { size_t len=0; a_.deserialize(mpasswd,len,"passwd");}
  bs_.set(3);
  a_.endRecord(*this,tag);
  return;
}
bool org::apache::zookeeper::proto::ConnectResponse::validate() const {
  if (bs_.size() != bs_.count()) return false;
  return true;
}
bool org::apache::zookeeper::proto::ConnectResponse::operator< (const org::apache::zookeeper::proto::ConnectResponse& peer_) const {
  return (1
    && (mprotocolVersion < peer_.mprotocolVersion)
    && (mtimeOut < peer_.mtimeOut)
    && (msessionId < peer_.msessionId)
    && (mpasswd < peer_.mpasswd)
  );
}
bool org::apache::zookeeper::proto::ConnectResponse::operator== (const org::apache::zookeeper::proto::ConnectResponse& peer_) const {
  return (1
    && (mprotocolVersion == peer_.mprotocolVersion)
    && (mtimeOut == peer_.mtimeOut)
    && (msessionId == peer_.msessionId)
    && (mpasswd == peer_.mpasswd)
  );
}
const ::std::string&org::apache::zookeeper::proto::ConnectResponse::type() const {
  static const ::std::string type_("ConnectResponse");
  return type_;
}
const ::std::string&org::apache::zookeeper::proto::ConnectResponse::signature() const {
  static const ::std::string sig_("LConnectResponse(iilB)");
  return sig_;
}
void org::apache::zookeeper::proto::SetWatches::serialize(::hadoop::OArchive& a_, const char* tag) const {
  if (!validate()) throw new ::hadoop::IOException("All fields not set.");
  a_.startRecord(*this,tag);
  a_.serialize(mrelativeZxid,"relativeZxid");
  bs_.reset(0);
  a_.serialize(mdataWatches,"dataWatches");
  bs_.reset(1);
  a_.serialize(mexistWatches,"existWatches");
  bs_.reset(2);
  a_.serialize(mchildWatches,"childWatches");
  bs_.reset(3);
  a_.endRecord(*this,tag);
  return;
}
void org::apache::zookeeper::proto::SetWatches::deserialize(::hadoop::IArchive& a_, const char* tag) {
  a_.startRecord(*this,tag);
  a_.deserialize(mrelativeZxid,"relativeZxid");
  bs_.set(0);
  a_.deserialize(mdataWatches,"dataWatches");
  bs_.set(1);
  a_.deserialize(mexistWatches,"existWatches");
  bs_.set(2);
  a_.deserialize(mchildWatches,"childWatches");
  bs_.set(3);
  a_.endRecord(*this,tag);
  return;
}
bool org::apache::zookeeper::proto::SetWatches::validate() const {
  if (bs_.size() != bs_.count()) return false;
  return true;
}
bool org::apache::zookeeper::proto::SetWatches::operator< (const org::apache::zookeeper::proto::SetWatches& peer_) const {
  return (1
    && (mrelativeZxid < peer_.mrelativeZxid)
    && (mdataWatches < peer_.mdataWatches)
    && (mexistWatches < peer_.mexistWatches)
    && (mchildWatches < peer_.mchildWatches)
  );
}
bool org::apache::zookeeper::proto::SetWatches::operator== (const org::apache::zookeeper::proto::SetWatches& peer_) const {
  return (1
    && (mrelativeZxid == peer_.mrelativeZxid)
    && (mdataWatches == peer_.mdataWatches)
    && (mexistWatches == peer_.mexistWatches)
    && (mchildWatches == peer_.mchildWatches)
  );
}
const ::std::string&org::apache::zookeeper::proto::SetWatches::type() const {
  static const ::std::string type_("SetWatches");
  return type_;
}
const ::std::string&org::apache::zookeeper::proto::SetWatches::signature() const {
  static const ::std::string sig_("LSetWatches(l[s][s][s])");
  return sig_;
}
void org::apache::zookeeper::proto::RequestHeader::serialize(::hadoop::OArchive& a_, const char* tag) const {
  if (!validate()) throw new ::hadoop::IOException("All fields not set.");
  a_.startRecord(*this,tag);
  a_.serialize(mxid,"xid");
  bs_.reset(0);
  a_.serialize(mtype,"type");
  bs_.reset(1);
  a_.endRecord(*this,tag);
  return;
}
void org::apache::zookeeper::proto::RequestHeader::deserialize(::hadoop::IArchive& a_, const char* tag) {
  a_.startRecord(*this,tag);
  a_.deserialize(mxid,"xid");
  bs_.set(0);
  a_.deserialize(mtype,"type");
  bs_.set(1);
  a_.endRecord(*this,tag);
  return;
}
bool org::apache::zookeeper::proto::RequestHeader::validate() const {
  if (bs_.size() != bs_.count()) return false;
  return true;
}
bool org::apache::zookeeper::proto::RequestHeader::operator< (const org::apache::zookeeper::proto::RequestHeader& peer_) const {
  return (1
    && (mxid < peer_.mxid)
    && (mtype < peer_.mtype)
  );
}
bool org::apache::zookeeper::proto::RequestHeader::operator== (const org::apache::zookeeper::proto::RequestHeader& peer_) const {
  return (1
    && (mxid == peer_.mxid)
    && (mtype == peer_.mtype)
  );
}
const ::std::string&org::apache::zookeeper::proto::RequestHeader::type() const {
  static const ::std::string type_("RequestHeader");
  return type_;
}
const ::std::string&org::apache::zookeeper::proto::RequestHeader::signature() const {
  static const ::std::string sig_("LRequestHeader(ii)");
  return sig_;
}
void org::apache::zookeeper::proto::MultiHeader::serialize(::hadoop::OArchive& a_, const char* tag) const {
  if (!validate()) throw new ::hadoop::IOException("All fields not set.");
  a_.startRecord(*this,tag);
  a_.serialize(mtype,"type");
  bs_.reset(0);
  a_.serialize(mdone,"done");
  bs_.reset(1);
  a_.serialize(merr,"err");
  bs_.reset(2);
  a_.endRecord(*this,tag);
  return;
}
void org::apache::zookeeper::proto::MultiHeader::deserialize(::hadoop::IArchive& a_, const char* tag) {
  a_.startRecord(*this,tag);
  a_.deserialize(mtype,"type");
  bs_.set(0);
  a_.deserialize(mdone,"done");
  bs_.set(1);
  a_.deserialize(merr,"err");
  bs_.set(2);
  a_.endRecord(*this,tag);
  return;
}
bool org::apache::zookeeper::proto::MultiHeader::validate() const {
  if (bs_.size() != bs_.count()) return false;
  return true;
}
bool org::apache::zookeeper::proto::MultiHeader::operator< (const org::apache::zookeeper::proto::MultiHeader& peer_) const {
  return (1
    && (mtype < peer_.mtype)
    && (mdone < peer_.mdone)
    && (merr < peer_.merr)
  );
}
bool org::apache::zookeeper::proto::MultiHeader::operator== (const org::apache::zookeeper::proto::MultiHeader& peer_) const {
  return (1
    && (mtype == peer_.mtype)
    && (mdone == peer_.mdone)
    && (merr == peer_.merr)
  );
}
const ::std::string&org::apache::zookeeper::proto::MultiHeader::type() const {
  static const ::std::string type_("MultiHeader");
  return type_;
}
const ::std::string&org::apache::zookeeper::proto::MultiHeader::signature() const {
  static const ::std::string sig_("LMultiHeader(izi)");
  return sig_;
}
void org::apache::zookeeper::proto::AuthPacket::serialize(::hadoop::OArchive& a_, const char* tag) const {
  if (!validate()) throw new ::hadoop::IOException("All fields not set.");
  a_.startRecord(*this,tag);
  a_.serialize(mtype,"type");
  bs_.reset(0);
  a_.serialize(mscheme,"scheme");
  bs_.reset(1);
  a_.serialize(mauth,mauth.length(),"auth");
  bs_.reset(2);
  a_.endRecord(*this,tag);
  return;
}
void org::apache::zookeeper::proto::AuthPacket::deserialize(::hadoop::IArchive& a_, const char* tag) {
  a_.startRecord(*this,tag);
  a_.deserialize(mtype,"type");
  bs_.set(0);
  a_.deserialize(mscheme,"scheme");
  bs_.set(1);
  { size_t len=0; a_.deserialize(mauth,len,"auth");}
  bs_.set(2);
  a_.endRecord(*this,tag);
  return;
}
bool org::apache::zookeeper::proto::AuthPacket::validate() const {
  if (bs_.size() != bs_.count()) return false;
  return true;
}
bool org::apache::zookeeper::proto::AuthPacket::operator< (const org::apache::zookeeper::proto::AuthPacket& peer_) const {
  return (1
    && (mtype < peer_.mtype)
    && (mscheme < peer_.mscheme)
    && (mauth < peer_.mauth)
  );
}
bool org::apache::zookeeper::proto::AuthPacket::operator== (const org::apache::zookeeper::proto::AuthPacket& peer_) const {
  return (1
    && (mtype == peer_.mtype)
    && (mscheme == peer_.mscheme)
    && (mauth == peer_.mauth)
  );
}
const ::std::string&org::apache::zookeeper::proto::AuthPacket::type() const {
  static const ::std::string type_("AuthPacket");
  return type_;
}
const ::std::string&org::apache::zookeeper::proto::AuthPacket::signature() const {
  static const ::std::string sig_("LAuthPacket(isB)");
  return sig_;
}
void org::apache::zookeeper::proto::ReplyHeader::serialize(::hadoop::OArchive& a_, const char* tag) const {
  if (!validate()) throw new ::hadoop::IOException("All fields not set.");
  a_.startRecord(*this,tag);
  a_.serialize(mxid,"xid");
  bs_.reset(0);
  a_.serialize(mzxid,"zxid");
  bs_.reset(1);
  a_.serialize(merr,"err");
  bs_.reset(2);
  a_.endRecord(*this,tag);
  return;
}
void org::apache::zookeeper::proto::ReplyHeader::deserialize(::hadoop::IArchive& a_, const char* tag) {
  a_.startRecord(*this,tag);
  a_.deserialize(mxid,"xid");
  bs_.set(0);
  a_.deserialize(mzxid,"zxid");
  bs_.set(1);
  a_.deserialize(merr,"err");
  bs_.set(2);
  a_.endRecord(*this,tag);
  return;
}
bool org::apache::zookeeper::proto::ReplyHeader::validate() const {
  if (bs_.size() != bs_.count()) return false;
  return true;
}
bool org::apache::zookeeper::proto::ReplyHeader::operator< (const org::apache::zookeeper::proto::ReplyHeader& peer_) const {
  return (1
    && (mxid < peer_.mxid)
    && (mzxid < peer_.mzxid)
    && (merr < peer_.merr)
  );
}
bool org::apache::zookeeper::proto::ReplyHeader::operator== (const org::apache::zookeeper::proto::ReplyHeader& peer_) const {
  return (1
    && (mxid == peer_.mxid)
    && (mzxid == peer_.mzxid)
    && (merr == peer_.merr)
  );
}
const ::std::string&org::apache::zookeeper::proto::ReplyHeader::type() const {
  static const ::std::string type_("ReplyHeader");
  return type_;
}
const ::std::string&org::apache::zookeeper::proto::ReplyHeader::signature() const {
  static const ::std::string sig_("LReplyHeader(ili)");
  return sig_;
}
void org::apache::zookeeper::proto::GetDataRequest::serialize(::hadoop::OArchive& a_, const char* tag) const {
  if (!validate()) throw new ::hadoop::IOException("All fields not set.");
  a_.startRecord(*this,tag);
  a_.serialize(mpath,"path");
  bs_.reset(0);
  a_.serialize(mwatch,"watch");
  bs_.reset(1);
  a_.endRecord(*this,tag);
  return;
}
void org::apache::zookeeper::proto::GetDataRequest::deserialize(::hadoop::IArchive& a_, const char* tag) {
  a_.startRecord(*this,tag);
  a_.deserialize(mpath,"path");
  bs_.set(0);
  a_.deserialize(mwatch,"watch");
  bs_.set(1);
  a_.endRecord(*this,tag);
  return;
}
bool org::apache::zookeeper::proto::GetDataRequest::validate() const {
  if (bs_.size() != bs_.count()) return false;
  return true;
}
bool org::apache::zookeeper::proto::GetDataRequest::operator< (const org::apache::zookeeper::proto::GetDataRequest& peer_) const {
  return (1
    && (mpath < peer_.mpath)
    && (mwatch < peer_.mwatch)
  );
}
bool org::apache::zookeeper::proto::GetDataRequest::operator== (const org::apache::zookeeper::proto::GetDataRequest& peer_) const {
  return (1
    && (mpath == peer_.mpath)
    && (mwatch == peer_.mwatch)
  );
}
const ::std::string&org::apache::zookeeper::proto::GetDataRequest::type() const {
  static const ::std::string type_("GetDataRequest");
  return type_;
}
const ::std::string&org::apache::zookeeper::proto::GetDataRequest::signature() const {
  static const ::std::string sig_("LGetDataRequest(sz)");
  return sig_;
}
void org::apache::zookeeper::proto::SetDataRequest::serialize(::hadoop::OArchive& a_, const char* tag) const {
  if (!validate()) throw new ::hadoop::IOException("All fields not set.");
  a_.startRecord(*this,tag);
  a_.serialize(mpath,"path");
  bs_.reset(0);
  a_.serialize(mdata,mdata.length(),"data");
  bs_.reset(1);
  a_.serialize(mversion,"version");
  bs_.reset(2);
  a_.endRecord(*this,tag);
  return;
}
void org::apache::zookeeper::proto::SetDataRequest::deserialize(::hadoop::IArchive& a_, const char* tag) {
  a_.startRecord(*this,tag);
  a_.deserialize(mpath,"path");
  bs_.set(0);
  { size_t len=0; a_.deserialize(mdata,len,"data");}
  bs_.set(1);
  a_.deserialize(mversion,"version");
  bs_.set(2);
  a_.endRecord(*this,tag);
  return;
}
bool org::apache::zookeeper::proto::SetDataRequest::validate() const {
  if (bs_.size() != bs_.count()) return false;
  return true;
}
bool org::apache::zookeeper::proto::SetDataRequest::operator< (const org::apache::zookeeper::proto::SetDataRequest& peer_) const {
  return (1
    && (mpath < peer_.mpath)
    && (mdata < peer_.mdata)
    && (mversion < peer_.mversion)
  );
}
bool org::apache::zookeeper::proto::SetDataRequest::operator== (const org::apache::zookeeper::proto::SetDataRequest& peer_) const {
  return (1
    && (mpath == peer_.mpath)
    && (mdata == peer_.mdata)
    && (mversion == peer_.mversion)
  );
}
const ::std::string&org::apache::zookeeper::proto::SetDataRequest::type() const {
  static const ::std::string type_("SetDataRequest");
  return type_;
}
const ::std::string&org::apache::zookeeper::proto::SetDataRequest::signature() const {
  static const ::std::string sig_("LSetDataRequest(sBi)");
  return sig_;
}
void org::apache::zookeeper::proto::SetDataResponse::serialize(::hadoop::OArchive& a_, const char* tag) const {
  if (!validate()) throw new ::hadoop::IOException("All fields not set.");
  a_.startRecord(*this,tag);
  a_.serialize(mstat,"stat");
  bs_.reset(0);
  a_.endRecord(*this,tag);
  return;
}
void org::apache::zookeeper::proto::SetDataResponse::deserialize(::hadoop::IArchive& a_, const char* tag) {
  a_.startRecord(*this,tag);
  a_.deserialize(mstat,"stat");
  bs_.set(0);
  a_.endRecord(*this,tag);
  return;
}
bool org::apache::zookeeper::proto::SetDataResponse::validate() const {
  if (bs_.size() != bs_.count()) return false;
  if (!mstat.validate()) return false;
  return true;
}
bool org::apache::zookeeper::proto::SetDataResponse::operator< (const org::apache::zookeeper::proto::SetDataResponse& peer_) const {
  return (1
    && (mstat < peer_.mstat)
  );
}
bool org::apache::zookeeper::proto::SetDataResponse::operator== (const org::apache::zookeeper::proto::SetDataResponse& peer_) const {
  return (1
    && (mstat == peer_.mstat)
  );
}
const ::std::string&org::apache::zookeeper::proto::SetDataResponse::type() const {
  static const ::std::string type_("SetDataResponse");
  return type_;
}
const ::std::string&org::apache::zookeeper::proto::SetDataResponse::signature() const {
  static const ::std::string sig_("LSetDataResponse(LStat(lllliiiliil))");
  return sig_;
}
void org::apache::zookeeper::proto::GetSASLRequest::serialize(::hadoop::OArchive& a_, const char* tag) const {
  if (!validate()) throw new ::hadoop::IOException("All fields not set.");
  a_.startRecord(*this,tag);
  a_.serialize(mtoken,mtoken.length(),"token");
  bs_.reset(0);
  a_.endRecord(*this,tag);
  return;
}
void org::apache::zookeeper::proto::GetSASLRequest::deserialize(::hadoop::IArchive& a_, const char* tag) {
  a_.startRecord(*this,tag);
  { size_t len=0; a_.deserialize(mtoken,len,"token");}
  bs_.set(0);
  a_.endRecord(*this,tag);
  return;
}
bool org::apache::zookeeper::proto::GetSASLRequest::validate() const {
  if (bs_.size() != bs_.count()) return false;
  return true;
}
bool org::apache::zookeeper::proto::GetSASLRequest::operator< (const org::apache::zookeeper::proto::GetSASLRequest& peer_) const {
  return (1
    && (mtoken < peer_.mtoken)
  );
}
bool org::apache::zookeeper::proto::GetSASLRequest::operator== (const org::apache::zookeeper::proto::GetSASLRequest& peer_) const {
  return (1
    && (mtoken == peer_.mtoken)
  );
}
const ::std::string&org::apache::zookeeper::proto::GetSASLRequest::type() const {
  static const ::std::string type_("GetSASLRequest");
  return type_;
}
const ::std::string&org::apache::zookeeper::proto::GetSASLRequest::signature() const {
  static const ::std::string sig_("LGetSASLRequest(B)");
  return sig_;
}
void org::apache::zookeeper::proto::SetSASLRequest::serialize(::hadoop::OArchive& a_, const char* tag) const {
  if (!validate()) throw new ::hadoop::IOException("All fields not set.");
  a_.startRecord(*this,tag);
  a_.serialize(mtoken,mtoken.length(),"token");
  bs_.reset(0);
  a_.endRecord(*this,tag);
  return;
}
void org::apache::zookeeper::proto::SetSASLRequest::deserialize(::hadoop::IArchive& a_, const char* tag) {
  a_.startRecord(*this,tag);
  { size_t len=0; a_.deserialize(mtoken,len,"token");}
  bs_.set(0);
  a_.endRecord(*this,tag);
  return;
}
bool org::apache::zookeeper::proto::SetSASLRequest::validate() const {
  if (bs_.size() != bs_.count()) return false;
  return true;
}
bool org::apache::zookeeper::proto::SetSASLRequest::operator< (const org::apache::zookeeper::proto::SetSASLRequest& peer_) const {
  return (1
    && (mtoken < peer_.mtoken)
  );
}
bool org::apache::zookeeper::proto::SetSASLRequest::operator== (const org::apache::zookeeper::proto::SetSASLRequest& peer_) const {
  return (1
    && (mtoken == peer_.mtoken)
  );
}
const ::std::string&org::apache::zookeeper::proto::SetSASLRequest::type() const {
  static const ::std::string type_("SetSASLRequest");
  return type_;
}
const ::std::string&org::apache::zookeeper::proto::SetSASLRequest::signature() const {
  static const ::std::string sig_("LSetSASLRequest(B)");
  return sig_;
}
void org::apache::zookeeper::proto::SetSASLResponse::serialize(::hadoop::OArchive& a_, const char* tag) const {
  if (!validate()) throw new ::hadoop::IOException("All fields not set.");
  a_.startRecord(*this,tag);
  a_.serialize(mtoken,mtoken.length(),"token");
  bs_.reset(0);
  a_.endRecord(*this,tag);
  return;
}
void org::apache::zookeeper::proto::SetSASLResponse::deserialize(::hadoop::IArchive& a_, const char* tag) {
  a_.startRecord(*this,tag);
  { size_t len=0; a_.deserialize(mtoken,len,"token");}
  bs_.set(0);
  a_.endRecord(*this,tag);
  return;
}
bool org::apache::zookeeper::proto::SetSASLResponse::validate() const {
  if (bs_.size() != bs_.count()) return false;
  return true;
}
bool org::apache::zookeeper::proto::SetSASLResponse::operator< (const org::apache::zookeeper::proto::SetSASLResponse& peer_) const {
  return (1
    && (mtoken < peer_.mtoken)
  );
}
bool org::apache::zookeeper::proto::SetSASLResponse::operator== (const org::apache::zookeeper::proto::SetSASLResponse& peer_) const {
  return (1
    && (mtoken == peer_.mtoken)
  );
}
const ::std::string&org::apache::zookeeper::proto::SetSASLResponse::type() const {
  static const ::std::string type_("SetSASLResponse");
  return type_;
}
const ::std::string&org::apache::zookeeper::proto::SetSASLResponse::signature() const {
  static const ::std::string sig_("LSetSASLResponse(B)");
  return sig_;
}
void org::apache::zookeeper::proto::CreateRequest::serialize(::hadoop::OArchive& a_, const char* tag) const {
  if (!validate()) throw new ::hadoop::IOException("All fields not set.");
  a_.startRecord(*this,tag);
  a_.serialize(mpath,"path");
  bs_.reset(0);
  a_.serialize(mdata,mdata.length(),"data");
  bs_.reset(1);
  a_.serialize(macl,"acl");
  bs_.reset(2);
  a_.serialize(mflags,"flags");
  bs_.reset(3);
  a_.endRecord(*this,tag);
  return;
}
void org::apache::zookeeper::proto::CreateRequest::deserialize(::hadoop::IArchive& a_, const char* tag) {
  a_.startRecord(*this,tag);
  a_.deserialize(mpath,"path");
  bs_.set(0);
  { size_t len=0; a_.deserialize(mdata,len,"data");}
  bs_.set(1);
  a_.deserialize(macl,"acl");
  bs_.set(2);
  a_.deserialize(mflags,"flags");
  bs_.set(3);
  a_.endRecord(*this,tag);
  return;
}
bool org::apache::zookeeper::proto::CreateRequest::validate() const {
  if (bs_.size() != bs_.count()) return false;
  return true;
}
bool org::apache::zookeeper::proto::CreateRequest::operator< (const org::apache::zookeeper::proto::CreateRequest& peer_) const {
  return (1
    && (mpath < peer_.mpath)
    && (mdata < peer_.mdata)
    && (macl < peer_.macl)
    && (mflags < peer_.mflags)
  );
}
bool org::apache::zookeeper::proto::CreateRequest::operator== (const org::apache::zookeeper::proto::CreateRequest& peer_) const {
  return (1
    && (mpath == peer_.mpath)
    && (mdata == peer_.mdata)
    && (macl == peer_.macl)
    && (mflags == peer_.mflags)
  );
}
const ::std::string&org::apache::zookeeper::proto::CreateRequest::type() const {
  static const ::std::string type_("CreateRequest");
  return type_;
}
const ::std::string&org::apache::zookeeper::proto::CreateRequest::signature() const {
  static const ::std::string sig_("LCreateRequest(sB[LACL(iLId(ss))]i)");
  return sig_;
}
void org::apache::zookeeper::proto::DeleteRequest::serialize(::hadoop::OArchive& a_, const char* tag) const {
  if (!validate()) throw new ::hadoop::IOException("All fields not set.");
  a_.startRecord(*this,tag);
  a_.serialize(mpath,"path");
  bs_.reset(0);
  a_.serialize(mversion,"version");
  bs_.reset(1);
  a_.endRecord(*this,tag);
  return;
}
void org::apache::zookeeper::proto::DeleteRequest::deserialize(::hadoop::IArchive& a_, const char* tag) {
  a_.startRecord(*this,tag);
  a_.deserialize(mpath,"path");
  bs_.set(0);
  a_.deserialize(mversion,"version");
  bs_.set(1);
  a_.endRecord(*this,tag);
  return;
}
bool org::apache::zookeeper::proto::DeleteRequest::validate() const {
  if (bs_.size() != bs_.count()) return false;
  return true;
}
bool org::apache::zookeeper::proto::DeleteRequest::operator< (const org::apache::zookeeper::proto::DeleteRequest& peer_) const {
  return (1
    && (mpath < peer_.mpath)
    && (mversion < peer_.mversion)
  );
}
bool org::apache::zookeeper::proto::DeleteRequest::operator== (const org::apache::zookeeper::proto::DeleteRequest& peer_) const {
  return (1
    && (mpath == peer_.mpath)
    && (mversion == peer_.mversion)
  );
}
const ::std::string&org::apache::zookeeper::proto::DeleteRequest::type() const {
  static const ::std::string type_("DeleteRequest");
  return type_;
}
const ::std::string&org::apache::zookeeper::proto::DeleteRequest::signature() const {
  static const ::std::string sig_("LDeleteRequest(si)");
  return sig_;
}
void org::apache::zookeeper::proto::GetChildrenRequest::serialize(::hadoop::OArchive& a_, const char* tag) const {
  if (!validate()) throw new ::hadoop::IOException("All fields not set.");
  a_.startRecord(*this,tag);
  a_.serialize(mpath,"path");
  bs_.reset(0);
  a_.serialize(mwatch,"watch");
  bs_.reset(1);
  a_.endRecord(*this,tag);
  return;
}
void org::apache::zookeeper::proto::GetChildrenRequest::deserialize(::hadoop::IArchive& a_, const char* tag) {
  a_.startRecord(*this,tag);
  a_.deserialize(mpath,"path");
  bs_.set(0);
  a_.deserialize(mwatch,"watch");
  bs_.set(1);
  a_.endRecord(*this,tag);
  return;
}
bool org::apache::zookeeper::proto::GetChildrenRequest::validate() const {
  if (bs_.size() != bs_.count()) return false;
  return true;
}
bool org::apache::zookeeper::proto::GetChildrenRequest::operator< (const org::apache::zookeeper::proto::GetChildrenRequest& peer_) const {
  return (1
    && (mpath < peer_.mpath)
    && (mwatch < peer_.mwatch)
  );
}
bool org::apache::zookeeper::proto::GetChildrenRequest::operator== (const org::apache::zookeeper::proto::GetChildrenRequest& peer_) const {
  return (1
    && (mpath == peer_.mpath)
    && (mwatch == peer_.mwatch)
  );
}
const ::std::string&org::apache::zookeeper::proto::GetChildrenRequest::type() const {
  static const ::std::string type_("GetChildrenRequest");
  return type_;
}
const ::std::string&org::apache::zookeeper::proto::GetChildrenRequest::signature() const {
  static const ::std::string sig_("LGetChildrenRequest(sz)");
  return sig_;
}
void org::apache::zookeeper::proto::GetChildren2Request::serialize(::hadoop::OArchive& a_, const char* tag) const {
  if (!validate()) throw new ::hadoop::IOException("All fields not set.");
  a_.startRecord(*this,tag);
  a_.serialize(mpath,"path");
  bs_.reset(0);
  a_.serialize(mwatch,"watch");
  bs_.reset(1);
  a_.endRecord(*this,tag);
  return;
}
void org::apache::zookeeper::proto::GetChildren2Request::deserialize(::hadoop::IArchive& a_, const char* tag) {
  a_.startRecord(*this,tag);
  a_.deserialize(mpath,"path");
  bs_.set(0);
  a_.deserialize(mwatch,"watch");
  bs_.set(1);
  a_.endRecord(*this,tag);
  return;
}
bool org::apache::zookeeper::proto::GetChildren2Request::validate() const {
  if (bs_.size() != bs_.count()) return false;
  return true;
}
bool org::apache::zookeeper::proto::GetChildren2Request::operator< (const org::apache::zookeeper::proto::GetChildren2Request& peer_) const {
  return (1
    && (mpath < peer_.mpath)
    && (mwatch < peer_.mwatch)
  );
}
bool org::apache::zookeeper::proto::GetChildren2Request::operator== (const org::apache::zookeeper::proto::GetChildren2Request& peer_) const {
  return (1
    && (mpath == peer_.mpath)
    && (mwatch == peer_.mwatch)
  );
}
const ::std::string&org::apache::zookeeper::proto::GetChildren2Request::type() const {
  static const ::std::string type_("GetChildren2Request");
  return type_;
}
const ::std::string&org::apache::zookeeper::proto::GetChildren2Request::signature() const {
  static const ::std::string sig_("LGetChildren2Request(sz)");
  return sig_;
}
void org::apache::zookeeper::proto::CheckVersionRequest::serialize(::hadoop::OArchive& a_, const char* tag) const {
  if (!validate()) throw new ::hadoop::IOException("All fields not set.");
  a_.startRecord(*this,tag);
  a_.serialize(mpath,"path");
  bs_.reset(0);
  a_.serialize(mversion,"version");
  bs_.reset(1);
  a_.endRecord(*this,tag);
  return;
}
void org::apache::zookeeper::proto::CheckVersionRequest::deserialize(::hadoop::IArchive& a_, const char* tag) {
  a_.startRecord(*this,tag);
  a_.deserialize(mpath,"path");
  bs_.set(0);
  a_.deserialize(mversion,"version");
  bs_.set(1);
  a_.endRecord(*this,tag);
  return;
}
bool org::apache::zookeeper::proto::CheckVersionRequest::validate() const {
  if (bs_.size() != bs_.count()) return false;
  return true;
}
bool org::apache::zookeeper::proto::CheckVersionRequest::operator< (const org::apache::zookeeper::proto::CheckVersionRequest& peer_) const {
  return (1
    && (mpath < peer_.mpath)
    && (mversion < peer_.mversion)
  );
}
bool org::apache::zookeeper::proto::CheckVersionRequest::operator== (const org::apache::zookeeper::proto::CheckVersionRequest& peer_) const {
  return (1
    && (mpath == peer_.mpath)
    && (mversion == peer_.mversion)
  );
}
const ::std::string&org::apache::zookeeper::proto::CheckVersionRequest::type() const {
  static const ::std::string type_("CheckVersionRequest");
  return type_;
}
const ::std::string&org::apache::zookeeper::proto::CheckVersionRequest::signature() const {
  static const ::std::string sig_("LCheckVersionRequest(si)");
  return sig_;
}
void org::apache::zookeeper::proto::GetMaxChildrenRequest::serialize(::hadoop::OArchive& a_, const char* tag) const {
  if (!validate()) throw new ::hadoop::IOException("All fields not set.");
  a_.startRecord(*this,tag);
  a_.serialize(mpath,"path");
  bs_.reset(0);
  a_.endRecord(*this,tag);
  return;
}
void org::apache::zookeeper::proto::GetMaxChildrenRequest::deserialize(::hadoop::IArchive& a_, const char* tag) {
  a_.startRecord(*this,tag);
  a_.deserialize(mpath,"path");
  bs_.set(0);
  a_.endRecord(*this,tag);
  return;
}
bool org::apache::zookeeper::proto::GetMaxChildrenRequest::validate() const {
  if (bs_.size() != bs_.count()) return false;
  return true;
}
bool org::apache::zookeeper::proto::GetMaxChildrenRequest::operator< (const org::apache::zookeeper::proto::GetMaxChildrenRequest& peer_) const {
  return (1
    && (mpath < peer_.mpath)
  );
}
bool org::apache::zookeeper::proto::GetMaxChildrenRequest::operator== (const org::apache::zookeeper::proto::GetMaxChildrenRequest& peer_) const {
  return (1
    && (mpath == peer_.mpath)
  );
}
const ::std::string&org::apache::zookeeper::proto::GetMaxChildrenRequest::type() const {
  static const ::std::string type_("GetMaxChildrenRequest");
  return type_;
}
const ::std::string&org::apache::zookeeper::proto::GetMaxChildrenRequest::signature() const {
  static const ::std::string sig_("LGetMaxChildrenRequest(s)");
  return sig_;
}
void org::apache::zookeeper::proto::GetMaxChildrenResponse::serialize(::hadoop::OArchive& a_, const char* tag) const {
  if (!validate()) throw new ::hadoop::IOException("All fields not set.");
  a_.startRecord(*this,tag);
  a_.serialize(mmax,"max");
  bs_.reset(0);
  a_.endRecord(*this,tag);
  return;
}
void org::apache::zookeeper::proto::GetMaxChildrenResponse::deserialize(::hadoop::IArchive& a_, const char* tag) {
  a_.startRecord(*this,tag);
  a_.deserialize(mmax,"max");
  bs_.set(0);
  a_.endRecord(*this,tag);
  return;
}
bool org::apache::zookeeper::proto::GetMaxChildrenResponse::validate() const {
  if (bs_.size() != bs_.count()) return false;
  return true;
}
bool org::apache::zookeeper::proto::GetMaxChildrenResponse::operator< (const org::apache::zookeeper::proto::GetMaxChildrenResponse& peer_) const {
  return (1
    && (mmax < peer_.mmax)
  );
}
bool org::apache::zookeeper::proto::GetMaxChildrenResponse::operator== (const org::apache::zookeeper::proto::GetMaxChildrenResponse& peer_) const {
  return (1
    && (mmax == peer_.mmax)
  );
}
const ::std::string&org::apache::zookeeper::proto::GetMaxChildrenResponse::type() const {
  static const ::std::string type_("GetMaxChildrenResponse");
  return type_;
}
const ::std::string&org::apache::zookeeper::proto::GetMaxChildrenResponse::signature() const {
  static const ::std::string sig_("LGetMaxChildrenResponse(i)");
  return sig_;
}
void org::apache::zookeeper::proto::SetMaxChildrenRequest::serialize(::hadoop::OArchive& a_, const char* tag) const {
  if (!validate()) throw new ::hadoop::IOException("All fields not set.");
  a_.startRecord(*this,tag);
  a_.serialize(mpath,"path");
  bs_.reset(0);
  a_.serialize(mmax,"max");
  bs_.reset(1);
  a_.endRecord(*this,tag);
  return;
}
void org::apache::zookeeper::proto::SetMaxChildrenRequest::deserialize(::hadoop::IArchive& a_, const char* tag) {
  a_.startRecord(*this,tag);
  a_.deserialize(mpath,"path");
  bs_.set(0);
  a_.deserialize(mmax,"max");
  bs_.set(1);
  a_.endRecord(*this,tag);
  return;
}
bool org::apache::zookeeper::proto::SetMaxChildrenRequest::validate() const {
  if (bs_.size() != bs_.count()) return false;
  return true;
}
bool org::apache::zookeeper::proto::SetMaxChildrenRequest::operator< (const org::apache::zookeeper::proto::SetMaxChildrenRequest& peer_) const {
  return (1
    && (mpath < peer_.mpath)
    && (mmax < peer_.mmax)
  );
}
bool org::apache::zookeeper::proto::SetMaxChildrenRequest::operator== (const org::apache::zookeeper::proto::SetMaxChildrenRequest& peer_) const {
  return (1
    && (mpath == peer_.mpath)
    && (mmax == peer_.mmax)
  );
}
const ::std::string&org::apache::zookeeper::proto::SetMaxChildrenRequest::type() const {
  static const ::std::string type_("SetMaxChildrenRequest");
  return type_;
}
const ::std::string&org::apache::zookeeper::proto::SetMaxChildrenRequest::signature() const {
  static const ::std::string sig_("LSetMaxChildrenRequest(si)");
  return sig_;
}
void org::apache::zookeeper::proto::SyncRequest::serialize(::hadoop::OArchive& a_, const char* tag) const {
  if (!validate()) throw new ::hadoop::IOException("All fields not set.");
  a_.startRecord(*this,tag);
  a_.serialize(mpath,"path");
  bs_.reset(0);
  a_.endRecord(*this,tag);
  return;
}
void org::apache::zookeeper::proto::SyncRequest::deserialize(::hadoop::IArchive& a_, const char* tag) {
  a_.startRecord(*this,tag);
  a_.deserialize(mpath,"path");
  bs_.set(0);
  a_.endRecord(*this,tag);
  return;
}
bool org::apache::zookeeper::proto::SyncRequest::validate() const {
  if (bs_.size() != bs_.count()) return false;
  return true;
}
bool org::apache::zookeeper::proto::SyncRequest::operator< (const org::apache::zookeeper::proto::SyncRequest& peer_) const {
  return (1
    && (mpath < peer_.mpath)
  );
}
bool org::apache::zookeeper::proto::SyncRequest::operator== (const org::apache::zookeeper::proto::SyncRequest& peer_) const {
  return (1
    && (mpath == peer_.mpath)
  );
}
const ::std::string&org::apache::zookeeper::proto::SyncRequest::type() const {
  static const ::std::string type_("SyncRequest");
  return type_;
}
const ::std::string&org::apache::zookeeper::proto::SyncRequest::signature() const {
  static const ::std::string sig_("LSyncRequest(s)");
  return sig_;
}
void org::apache::zookeeper::proto::SyncResponse::serialize(::hadoop::OArchive& a_, const char* tag) const {
  if (!validate()) throw new ::hadoop::IOException("All fields not set.");
  a_.startRecord(*this,tag);
  a_.serialize(mpath,"path");
  bs_.reset(0);
  a_.endRecord(*this,tag);
  return;
}
void org::apache::zookeeper::proto::SyncResponse::deserialize(::hadoop::IArchive& a_, const char* tag) {
  a_.startRecord(*this,tag);
  a_.deserialize(mpath,"path");
  bs_.set(0);
  a_.endRecord(*this,tag);
  return;
}
bool org::apache::zookeeper::proto::SyncResponse::validate() const {
  if (bs_.size() != bs_.count()) return false;
  return true;
}
bool org::apache::zookeeper::proto::SyncResponse::operator< (const org::apache::zookeeper::proto::SyncResponse& peer_) const {
  return (1
    && (mpath < peer_.mpath)
  );
}
bool org::apache::zookeeper::proto::SyncResponse::operator== (const org::apache::zookeeper::proto::SyncResponse& peer_) const {
  return (1
    && (mpath == peer_.mpath)
  );
}
const ::std::string&org::apache::zookeeper::proto::SyncResponse::type() const {
  static const ::std::string type_("SyncResponse");
  return type_;
}
const ::std::string&org::apache::zookeeper::proto::SyncResponse::signature() const {
  static const ::std::string sig_("LSyncResponse(s)");
  return sig_;
}
void org::apache::zookeeper::proto::GetACLRequest::serialize(::hadoop::OArchive& a_, const char* tag) const {
  if (!validate()) throw new ::hadoop::IOException("All fields not set.");
  a_.startRecord(*this,tag);
  a_.serialize(mpath,"path");
  bs_.reset(0);
  a_.endRecord(*this,tag);
  return;
}
void org::apache::zookeeper::proto::GetACLRequest::deserialize(::hadoop::IArchive& a_, const char* tag) {
  a_.startRecord(*this,tag);
  a_.deserialize(mpath,"path");
  bs_.set(0);
  a_.endRecord(*this,tag);
  return;
}
bool org::apache::zookeeper::proto::GetACLRequest::validate() const {
  if (bs_.size() != bs_.count()) return false;
  return true;
}
bool org::apache::zookeeper::proto::GetACLRequest::operator< (const org::apache::zookeeper::proto::GetACLRequest& peer_) const {
  return (1
    && (mpath < peer_.mpath)
  );
}
bool org::apache::zookeeper::proto::GetACLRequest::operator== (const org::apache::zookeeper::proto::GetACLRequest& peer_) const {
  return (1
    && (mpath == peer_.mpath)
  );
}
const ::std::string&org::apache::zookeeper::proto::GetACLRequest::type() const {
  static const ::std::string type_("GetACLRequest");
  return type_;
}
const ::std::string&org::apache::zookeeper::proto::GetACLRequest::signature() const {
  static const ::std::string sig_("LGetACLRequest(s)");
  return sig_;
}
void org::apache::zookeeper::proto::SetACLRequest::serialize(::hadoop::OArchive& a_, const char* tag) const {
  if (!validate()) throw new ::hadoop::IOException("All fields not set.");
  a_.startRecord(*this,tag);
  a_.serialize(mpath,"path");
  bs_.reset(0);
  a_.serialize(macl,"acl");
  bs_.reset(1);
  a_.serialize(mversion,"version");
  bs_.reset(2);
  a_.endRecord(*this,tag);
  return;
}
void org::apache::zookeeper::proto::SetACLRequest::deserialize(::hadoop::IArchive& a_, const char* tag) {
  a_.startRecord(*this,tag);
  a_.deserialize(mpath,"path");
  bs_.set(0);
  a_.deserialize(macl,"acl");
  bs_.set(1);
  a_.deserialize(mversion,"version");
  bs_.set(2);
  a_.endRecord(*this,tag);
  return;
}
bool org::apache::zookeeper::proto::SetACLRequest::validate() const {
  if (bs_.size() != bs_.count()) return false;
  return true;
}
bool org::apache::zookeeper::proto::SetACLRequest::operator< (const org::apache::zookeeper::proto::SetACLRequest& peer_) const {
  return (1
    && (mpath < peer_.mpath)
    && (macl < peer_.macl)
    && (mversion < peer_.mversion)
  );
}
bool org::apache::zookeeper::proto::SetACLRequest::operator== (const org::apache::zookeeper::proto::SetACLRequest& peer_) const {
  return (1
    && (mpath == peer_.mpath)
    && (macl == peer_.macl)
    && (mversion == peer_.mversion)
  );
}
const ::std::string&org::apache::zookeeper::proto::SetACLRequest::type() const {
  static const ::std::string type_("SetACLRequest");
  return type_;
}
const ::std::string&org::apache::zookeeper::proto::SetACLRequest::signature() const {
  static const ::std::string sig_("LSetACLRequest(s[LACL(iLId(ss))]i)");
  return sig_;
}
void org::apache::zookeeper::proto::SetACLResponse::serialize(::hadoop::OArchive& a_, const char* tag) const {
  if (!validate()) throw new ::hadoop::IOException("All fields not set.");
  a_.startRecord(*this,tag);
  a_.serialize(mstat,"stat");
  bs_.reset(0);
  a_.endRecord(*this,tag);
  return;
}
void org::apache::zookeeper::proto::SetACLResponse::deserialize(::hadoop::IArchive& a_, const char* tag) {
  a_.startRecord(*this,tag);
  a_.deserialize(mstat,"stat");
  bs_.set(0);
  a_.endRecord(*this,tag);
  return;
}
bool org::apache::zookeeper::proto::SetACLResponse::validate() const {
  if (bs_.size() != bs_.count()) return false;
  if (!mstat.validate()) return false;
  return true;
}
bool org::apache::zookeeper::proto::SetACLResponse::operator< (const org::apache::zookeeper::proto::SetACLResponse& peer_) const {
  return (1
    && (mstat < peer_.mstat)
  );
}
bool org::apache::zookeeper::proto::SetACLResponse::operator== (const org::apache::zookeeper::proto::SetACLResponse& peer_) const {
  return (1
    && (mstat == peer_.mstat)
  );
}
const ::std::string&org::apache::zookeeper::proto::SetACLResponse::type() const {
  static const ::std::string type_("SetACLResponse");
  return type_;
}
const ::std::string&org::apache::zookeeper::proto::SetACLResponse::signature() const {
  static const ::std::string sig_("LSetACLResponse(LStat(lllliiiliil))");
  return sig_;
}
void org::apache::zookeeper::proto::WatcherEvent::serialize(::hadoop::OArchive& a_, const char* tag) const {
  if (!validate()) throw new ::hadoop::IOException("All fields not set.");
  a_.startRecord(*this,tag);
  a_.serialize(mtype,"type");
  bs_.reset(0);
  a_.serialize(mstate,"state");
  bs_.reset(1);
  a_.serialize(mpath,"path");
  bs_.reset(2);
  a_.endRecord(*this,tag);
  return;
}
void org::apache::zookeeper::proto::WatcherEvent::deserialize(::hadoop::IArchive& a_, const char* tag) {
  a_.startRecord(*this,tag);
  a_.deserialize(mtype,"type");
  bs_.set(0);
  a_.deserialize(mstate,"state");
  bs_.set(1);
  a_.deserialize(mpath,"path");
  bs_.set(2);
  a_.endRecord(*this,tag);
  return;
}
bool org::apache::zookeeper::proto::WatcherEvent::validate() const {
  if (bs_.size() != bs_.count()) return false;
  return true;
}
bool org::apache::zookeeper::proto::WatcherEvent::operator< (const org::apache::zookeeper::proto::WatcherEvent& peer_) const {
  return (1
    && (mtype < peer_.mtype)
    && (mstate < peer_.mstate)
    && (mpath < peer_.mpath)
  );
}
bool org::apache::zookeeper::proto::WatcherEvent::operator== (const org::apache::zookeeper::proto::WatcherEvent& peer_) const {
  return (1
    && (mtype == peer_.mtype)
    && (mstate == peer_.mstate)
    && (mpath == peer_.mpath)
  );
}
const ::std::string&org::apache::zookeeper::proto::WatcherEvent::type() const {
  static const ::std::string type_("WatcherEvent");
  return type_;
}
const ::std::string&org::apache::zookeeper::proto::WatcherEvent::signature() const {
  static const ::std::string sig_("LWatcherEvent(iis)");
  return sig_;
}
void org::apache::zookeeper::proto::ErrorResponse::serialize(::hadoop::OArchive& a_, const char* tag) const {
  if (!validate()) throw new ::hadoop::IOException("All fields not set.");
  a_.startRecord(*this,tag);
  a_.serialize(merr,"err");
  bs_.reset(0);
  a_.endRecord(*this,tag);
  return;
}
void org::apache::zookeeper::proto::ErrorResponse::deserialize(::hadoop::IArchive& a_, const char* tag) {
  a_.startRecord(*this,tag);
  a_.deserialize(merr,"err");
  bs_.set(0);
  a_.endRecord(*this,tag);
  return;
}
bool org::apache::zookeeper::proto::ErrorResponse::validate() const {
  if (bs_.size() != bs_.count()) return false;
  return true;
}
bool org::apache::zookeeper::proto::ErrorResponse::operator< (const org::apache::zookeeper::proto::ErrorResponse& peer_) const {
  return (1
    && (merr < peer_.merr)
  );
}
bool org::apache::zookeeper::proto::ErrorResponse::operator== (const org::apache::zookeeper::proto::ErrorResponse& peer_) const {
  return (1
    && (merr == peer_.merr)
  );
}
const ::std::string&org::apache::zookeeper::proto::ErrorResponse::type() const {
  static const ::std::string type_("ErrorResponse");
  return type_;
}
const ::std::string&org::apache::zookeeper::proto::ErrorResponse::signature() const {
  static const ::std::string sig_("LErrorResponse(i)");
  return sig_;
}
void org::apache::zookeeper::proto::CreateResponse::serialize(::hadoop::OArchive& a_, const char* tag) const {
  if (!validate()) throw new ::hadoop::IOException("All fields not set.");
  a_.startRecord(*this,tag);
  a_.serialize(mpath,"path");
  bs_.reset(0);
  a_.endRecord(*this,tag);
  return;
}
void org::apache::zookeeper::proto::CreateResponse::deserialize(::hadoop::IArchive& a_, const char* tag) {
  a_.startRecord(*this,tag);
  a_.deserialize(mpath,"path");
  bs_.set(0);
  a_.endRecord(*this,tag);
  return;
}
bool org::apache::zookeeper::proto::CreateResponse::validate() const {
  if (bs_.size() != bs_.count()) return false;
  return true;
}
bool org::apache::zookeeper::proto::CreateResponse::operator< (const org::apache::zookeeper::proto::CreateResponse& peer_) const {
  return (1
    && (mpath < peer_.mpath)
  );
}
bool org::apache::zookeeper::proto::CreateResponse::operator== (const org::apache::zookeeper::proto::CreateResponse& peer_) const {
  return (1
    && (mpath == peer_.mpath)
  );
}
const ::std::string&org::apache::zookeeper::proto::CreateResponse::type() const {
  static const ::std::string type_("CreateResponse");
  return type_;
}
const ::std::string&org::apache::zookeeper::proto::CreateResponse::signature() const {
  static const ::std::string sig_("LCreateResponse(s)");
  return sig_;
}
void org::apache::zookeeper::proto::ExistsRequest::serialize(::hadoop::OArchive& a_, const char* tag) const {
  if (!validate()) throw new ::hadoop::IOException("All fields not set.");
  a_.startRecord(*this,tag);
  a_.serialize(mpath,"path");
  bs_.reset(0);
  a_.serialize(mwatch,"watch");
  bs_.reset(1);
  a_.endRecord(*this,tag);
  return;
}
void org::apache::zookeeper::proto::ExistsRequest::deserialize(::hadoop::IArchive& a_, const char* tag) {
  a_.startRecord(*this,tag);
  a_.deserialize(mpath,"path");
  bs_.set(0);
  a_.deserialize(mwatch,"watch");
  bs_.set(1);
  a_.endRecord(*this,tag);
  return;
}
bool org::apache::zookeeper::proto::ExistsRequest::validate() const {
  if (bs_.size() != bs_.count()) return false;
  return true;
}
bool org::apache::zookeeper::proto::ExistsRequest::operator< (const org::apache::zookeeper::proto::ExistsRequest& peer_) const {
  return (1
    && (mpath < peer_.mpath)
    && (mwatch < peer_.mwatch)
  );
}
bool org::apache::zookeeper::proto::ExistsRequest::operator== (const org::apache::zookeeper::proto::ExistsRequest& peer_) const {
  return (1
    && (mpath == peer_.mpath)
    && (mwatch == peer_.mwatch)
  );
}
const ::std::string&org::apache::zookeeper::proto::ExistsRequest::type() const {
  static const ::std::string type_("ExistsRequest");
  return type_;
}
const ::std::string&org::apache::zookeeper::proto::ExistsRequest::signature() const {
  static const ::std::string sig_("LExistsRequest(sz)");
  return sig_;
}
void org::apache::zookeeper::proto::ExistsResponse::serialize(::hadoop::OArchive& a_, const char* tag) const {
  if (!validate()) throw new ::hadoop::IOException("All fields not set.");
  a_.startRecord(*this,tag);
  a_.serialize(mstat,"stat");
  bs_.reset(0);
  a_.endRecord(*this,tag);
  return;
}
void org::apache::zookeeper::proto::ExistsResponse::deserialize(::hadoop::IArchive& a_, const char* tag) {
  a_.startRecord(*this,tag);
  a_.deserialize(mstat,"stat");
  bs_.set(0);
  a_.endRecord(*this,tag);
  return;
}
bool org::apache::zookeeper::proto::ExistsResponse::validate() const {
  if (bs_.size() != bs_.count()) return false;
  if (!mstat.validate()) return false;
  return true;
}
bool org::apache::zookeeper::proto::ExistsResponse::operator< (const org::apache::zookeeper::proto::ExistsResponse& peer_) const {
  return (1
    && (mstat < peer_.mstat)
  );
}
bool org::apache::zookeeper::proto::ExistsResponse::operator== (const org::apache::zookeeper::proto::ExistsResponse& peer_) const {
  return (1
    && (mstat == peer_.mstat)
  );
}
const ::std::string&org::apache::zookeeper::proto::ExistsResponse::type() const {
  static const ::std::string type_("ExistsResponse");
  return type_;
}
const ::std::string&org::apache::zookeeper::proto::ExistsResponse::signature() const {
  static const ::std::string sig_("LExistsResponse(LStat(lllliiiliil))");
  return sig_;
}
void org::apache::zookeeper::proto::GetDataResponse::serialize(::hadoop::OArchive& a_, const char* tag) const {
  if (!validate()) throw new ::hadoop::IOException("All fields not set.");
  a_.startRecord(*this,tag);
  a_.serialize(mdata,mdata.length(),"data");
  bs_.reset(0);
  a_.serialize(mstat,"stat");
  bs_.reset(1);
  a_.endRecord(*this,tag);
  return;
}
void org::apache::zookeeper::proto::GetDataResponse::deserialize(::hadoop::IArchive& a_, const char* tag) {
  a_.startRecord(*this,tag);
  { size_t len=0; a_.deserialize(mdata,len,"data");}
  bs_.set(0);
  a_.deserialize(mstat,"stat");
  bs_.set(1);
  a_.endRecord(*this,tag);
  return;
}
bool org::apache::zookeeper::proto::GetDataResponse::validate() const {
  if (bs_.size() != bs_.count()) return false;
  if (!mstat.validate()) return false;
  return true;
}
bool org::apache::zookeeper::proto::GetDataResponse::operator< (const org::apache::zookeeper::proto::GetDataResponse& peer_) const {
  return (1
    && (mdata < peer_.mdata)
    && (mstat < peer_.mstat)
  );
}
bool org::apache::zookeeper::proto::GetDataResponse::operator== (const org::apache::zookeeper::proto::GetDataResponse& peer_) const {
  return (1
    && (mdata == peer_.mdata)
    && (mstat == peer_.mstat)
  );
}
const ::std::string&org::apache::zookeeper::proto::GetDataResponse::type() const {
  static const ::std::string type_("GetDataResponse");
  return type_;
}
const ::std::string&org::apache::zookeeper::proto::GetDataResponse::signature() const {
  static const ::std::string sig_("LGetDataResponse(BLStat(lllliiiliil))");
  return sig_;
}
void org::apache::zookeeper::proto::GetChildrenResponse::serialize(::hadoop::OArchive& a_, const char* tag) const {
  if (!validate()) throw new ::hadoop::IOException("All fields not set.");
  a_.startRecord(*this,tag);
  a_.serialize(mchildren,"children");
  bs_.reset(0);
  a_.endRecord(*this,tag);
  return;
}
void org::apache::zookeeper::proto::GetChildrenResponse::deserialize(::hadoop::IArchive& a_, const char* tag) {
  a_.startRecord(*this,tag);
  a_.deserialize(mchildren,"children");
  bs_.set(0);
  a_.endRecord(*this,tag);
  return;
}
bool org::apache::zookeeper::proto::GetChildrenResponse::validate() const {
  if (bs_.size() != bs_.count()) return false;
  return true;
}
bool org::apache::zookeeper::proto::GetChildrenResponse::operator< (const org::apache::zookeeper::proto::GetChildrenResponse& peer_) const {
  return (1
    && (mchildren < peer_.mchildren)
  );
}
bool org::apache::zookeeper::proto::GetChildrenResponse::operator== (const org::apache::zookeeper::proto::GetChildrenResponse& peer_) const {
  return (1
    && (mchildren == peer_.mchildren)
  );
}
const ::std::string&org::apache::zookeeper::proto::GetChildrenResponse::type() const {
  static const ::std::string type_("GetChildrenResponse");
  return type_;
}
const ::std::string&org::apache::zookeeper::proto::GetChildrenResponse::signature() const {
  static const ::std::string sig_("LGetChildrenResponse([s])");
  return sig_;
}
void org::apache::zookeeper::proto::GetChildren2Response::serialize(::hadoop::OArchive& a_, const char* tag) const {
  if (!validate()) throw new ::hadoop::IOException("All fields not set.");
  a_.startRecord(*this,tag);
  a_.serialize(mchildren,"children");
  bs_.reset(0);
  a_.serialize(mstat,"stat");
  bs_.reset(1);
  a_.endRecord(*this,tag);
  return;
}
void org::apache::zookeeper::proto::GetChildren2Response::deserialize(::hadoop::IArchive& a_, const char* tag) {
  a_.startRecord(*this,tag);
  a_.deserialize(mchildren,"children");
  bs_.set(0);
  a_.deserialize(mstat,"stat");
  bs_.set(1);
  a_.endRecord(*this,tag);
  return;
}
bool org::apache::zookeeper::proto::GetChildren2Response::validate() const {
  if (bs_.size() != bs_.count()) return false;
  if (!mstat.validate()) return false;
  return true;
}
bool org::apache::zookeeper::proto::GetChildren2Response::operator< (const org::apache::zookeeper::proto::GetChildren2Response& peer_) const {
  return (1
    && (mchildren < peer_.mchildren)
    && (mstat < peer_.mstat)
  );
}
bool org::apache::zookeeper::proto::GetChildren2Response::operator== (const org::apache::zookeeper::proto::GetChildren2Response& peer_) const {
  return (1
    && (mchildren == peer_.mchildren)
    && (mstat == peer_.mstat)
  );
}
const ::std::string&org::apache::zookeeper::proto::GetChildren2Response::type() const {
  static const ::std::string type_("GetChildren2Response");
  return type_;
}
const ::std::string&org::apache::zookeeper::proto::GetChildren2Response::signature() const {
  static const ::std::string sig_("LGetChildren2Response([s]LStat(lllliiiliil))");
  return sig_;
}
void org::apache::zookeeper::proto::GetACLResponse::serialize(::hadoop::OArchive& a_, const char* tag) const {
  if (!validate()) throw new ::hadoop::IOException("All fields not set.");
  a_.startRecord(*this,tag);
  a_.serialize(macl,"acl");
  bs_.reset(0);
  a_.serialize(mstat,"stat");
  bs_.reset(1);
  a_.endRecord(*this,tag);
  return;
}
void org::apache::zookeeper::proto::GetACLResponse::deserialize(::hadoop::IArchive& a_, const char* tag) {
  a_.startRecord(*this,tag);
  a_.deserialize(macl,"acl");
  bs_.set(0);
  a_.deserialize(mstat,"stat");
  bs_.set(1);
  a_.endRecord(*this,tag);
  return;
}
bool org::apache::zookeeper::proto::GetACLResponse::validate() const {
  if (bs_.size() != bs_.count()) return false;
  if (!mstat.validate()) return false;
  return true;
}
bool org::apache::zookeeper::proto::GetACLResponse::operator< (const org::apache::zookeeper::proto::GetACLResponse& peer_) const {
  return (1
    && (macl < peer_.macl)
    && (mstat < peer_.mstat)
  );
}
bool org::apache::zookeeper::proto::GetACLResponse::operator== (const org::apache::zookeeper::proto::GetACLResponse& peer_) const {
  return (1
    && (macl == peer_.macl)
    && (mstat == peer_.mstat)
  );
}
const ::std::string&org::apache::zookeeper::proto::GetACLResponse::type() const {
  static const ::std::string type_("GetACLResponse");
  return type_;
}
const ::std::string&org::apache::zookeeper::proto::GetACLResponse::signature() const {
  static const ::std::string sig_("LGetACLResponse([LACL(iLId(ss))]LStat(lllliiiliil))");
  return sig_;
}
void org::apache::zookeeper::server::quorum::LearnerInfo::serialize(::hadoop::OArchive& a_, const char* tag) const {
  if (!validate()) throw new ::hadoop::IOException("All fields not set.");
  a_.startRecord(*this,tag);
  a_.serialize(mserverid,"serverid");
  bs_.reset(0);
  a_.serialize(mprotocolVersion,"protocolVersion");
  bs_.reset(1);
  a_.endRecord(*this,tag);
  return;
}
void org::apache::zookeeper::server::quorum::LearnerInfo::deserialize(::hadoop::IArchive& a_, const char* tag) {
  a_.startRecord(*this,tag);
  a_.deserialize(mserverid,"serverid");
  bs_.set(0);
  a_.deserialize(mprotocolVersion,"protocolVersion");
  bs_.set(1);
  a_.endRecord(*this,tag);
  return;
}
bool org::apache::zookeeper::server::quorum::LearnerInfo::validate() const {
  if (bs_.size() != bs_.count()) return false;
  return true;
}
bool org::apache::zookeeper::server::quorum::LearnerInfo::operator< (const org::apache::zookeeper::server::quorum::LearnerInfo& peer_) const {
  return (1
    && (mserverid < peer_.mserverid)
    && (mprotocolVersion < peer_.mprotocolVersion)
  );
}
bool org::apache::zookeeper::server::quorum::LearnerInfo::operator== (const org::apache::zookeeper::server::quorum::LearnerInfo& peer_) const {
  return (1
    && (mserverid == peer_.mserverid)
    && (mprotocolVersion == peer_.mprotocolVersion)
  );
}
const ::std::string&org::apache::zookeeper::server::quorum::LearnerInfo::type() const {
  static const ::std::string type_("LearnerInfo");
  return type_;
}
const ::std::string&org::apache::zookeeper::server::quorum::LearnerInfo::signature() const {
  static const ::std::string sig_("LLearnerInfo(li)");
  return sig_;
}
void org::apache::zookeeper::server::quorum::QuorumPacket::serialize(::hadoop::OArchive& a_, const char* tag) const {
  if (!validate()) throw new ::hadoop::IOException("All fields not set.");
  a_.startRecord(*this,tag);
  a_.serialize(mtype,"type");
  bs_.reset(0);
  a_.serialize(mzxid,"zxid");
  bs_.reset(1);
  a_.serialize(mdata,mdata.length(),"data");
  bs_.reset(2);
  a_.serialize(mauthinfo,"authinfo");
  bs_.reset(3);
  a_.endRecord(*this,tag);
  return;
}
void org::apache::zookeeper::server::quorum::QuorumPacket::deserialize(::hadoop::IArchive& a_, const char* tag) {
  a_.startRecord(*this,tag);
  a_.deserialize(mtype,"type");
  bs_.set(0);
  a_.deserialize(mzxid,"zxid");
  bs_.set(1);
  { size_t len=0; a_.deserialize(mdata,len,"data");}
  bs_.set(2);
  a_.deserialize(mauthinfo,"authinfo");
  bs_.set(3);
  a_.endRecord(*this,tag);
  return;
}
bool org::apache::zookeeper::server::quorum::QuorumPacket::validate() const {
  if (bs_.size() != bs_.count()) return false;
  return true;
}
bool org::apache::zookeeper::server::quorum::QuorumPacket::operator< (const org::apache::zookeeper::server::quorum::QuorumPacket& peer_) const {
  return (1
    && (mtype < peer_.mtype)
    && (mzxid < peer_.mzxid)
    && (mdata < peer_.mdata)
    && (mauthinfo < peer_.mauthinfo)
  );
}
bool org::apache::zookeeper::server::quorum::QuorumPacket::operator== (const org::apache::zookeeper::server::quorum::QuorumPacket& peer_) const {
  return (1
    && (mtype == peer_.mtype)
    && (mzxid == peer_.mzxid)
    && (mdata == peer_.mdata)
    && (mauthinfo == peer_.mauthinfo)
  );
}
const ::std::string&org::apache::zookeeper::server::quorum::QuorumPacket::type() const {
  static const ::std::string type_("QuorumPacket");
  return type_;
}
const ::std::string&org::apache::zookeeper::server::quorum::QuorumPacket::signature() const {
  static const ::std::string sig_("LQuorumPacket(ilB[LId(ss)])");
  return sig_;
}
void org::apache::zookeeper::server::persistence::FileHeader::serialize(::hadoop::OArchive& a_, const char* tag) const {
  if (!validate()) throw new ::hadoop::IOException("All fields not set.");
  a_.startRecord(*this,tag);
  a_.serialize(mmagic,"magic");
  bs_.reset(0);
  a_.serialize(mversion,"version");
  bs_.reset(1);
  a_.serialize(mdbid,"dbid");
  bs_.reset(2);
  a_.endRecord(*this,tag);
  return;
}
void org::apache::zookeeper::server::persistence::FileHeader::deserialize(::hadoop::IArchive& a_, const char* tag) {
  a_.startRecord(*this,tag);
  a_.deserialize(mmagic,"magic");
  bs_.set(0);
  a_.deserialize(mversion,"version");
  bs_.set(1);
  a_.deserialize(mdbid,"dbid");
  bs_.set(2);
  a_.endRecord(*this,tag);
  return;
}
bool org::apache::zookeeper::server::persistence::FileHeader::validate() const {
  if (bs_.size() != bs_.count()) return false;
  return true;
}
bool org::apache::zookeeper::server::persistence::FileHeader::operator< (const org::apache::zookeeper::server::persistence::FileHeader& peer_) const {
  return (1
    && (mmagic < peer_.mmagic)
    && (mversion < peer_.mversion)
    && (mdbid < peer_.mdbid)
  );
}
bool org::apache::zookeeper::server::persistence::FileHeader::operator== (const org::apache::zookeeper::server::persistence::FileHeader& peer_) const {
  return (1
    && (mmagic == peer_.mmagic)
    && (mversion == peer_.mversion)
    && (mdbid == peer_.mdbid)
  );
}
const ::std::string&org::apache::zookeeper::server::persistence::FileHeader::type() const {
  static const ::std::string type_("FileHeader");
  return type_;
}
const ::std::string&org::apache::zookeeper::server::persistence::FileHeader::signature() const {
  static const ::std::string sig_("LFileHeader(iil)");
  return sig_;
}
void org::apache::zookeeper::txn::TxnHeader::serialize(::hadoop::OArchive& a_, const char* tag) const {
  if (!validate()) throw new ::hadoop::IOException("All fields not set.");
  a_.startRecord(*this,tag);
  a_.serialize(mclientId,"clientId");
  bs_.reset(0);
  a_.serialize(mcxid,"cxid");
  bs_.reset(1);
  a_.serialize(mzxid,"zxid");
  bs_.reset(2);
  a_.serialize(mtime,"time");
  bs_.reset(3);
  a_.serialize(mtype,"type");
  bs_.reset(4);
  a_.endRecord(*this,tag);
  return;
}
void org::apache::zookeeper::txn::TxnHeader::deserialize(::hadoop::IArchive& a_, const char* tag) {
  a_.startRecord(*this,tag);
  a_.deserialize(mclientId,"clientId");
  bs_.set(0);
  a_.deserialize(mcxid,"cxid");
  bs_.set(1);
  a_.deserialize(mzxid,"zxid");
  bs_.set(2);
  a_.deserialize(mtime,"time");
  bs_.set(3);
  a_.deserialize(mtype,"type");
  bs_.set(4);
  a_.endRecord(*this,tag);
  return;
}
bool org::apache::zookeeper::txn::TxnHeader::validate() const {
  if (bs_.size() != bs_.count()) return false;
  return true;
}
bool org::apache::zookeeper::txn::TxnHeader::operator< (const org::apache::zookeeper::txn::TxnHeader& peer_) const {
  return (1
    && (mclientId < peer_.mclientId)
    && (mcxid < peer_.mcxid)
    && (mzxid < peer_.mzxid)
    && (mtime < peer_.mtime)
    && (mtype < peer_.mtype)
  );
}
bool org::apache::zookeeper::txn::TxnHeader::operator== (const org::apache::zookeeper::txn::TxnHeader& peer_) const {
  return (1
    && (mclientId == peer_.mclientId)
    && (mcxid == peer_.mcxid)
    && (mzxid == peer_.mzxid)
    && (mtime == peer_.mtime)
    && (mtype == peer_.mtype)
  );
}
const ::std::string&org::apache::zookeeper::txn::TxnHeader::type() const {
  static const ::std::string type_("TxnHeader");
  return type_;
}
const ::std::string&org::apache::zookeeper::txn::TxnHeader::signature() const {
  static const ::std::string sig_("LTxnHeader(lilli)");
  return sig_;
}
void org::apache::zookeeper::txn::CreateTxnV0::serialize(::hadoop::OArchive& a_, const char* tag) const {
  if (!validate()) throw new ::hadoop::IOException("All fields not set.");
  a_.startRecord(*this,tag);
  a_.serialize(mpath,"path");
  bs_.reset(0);
  a_.serialize(mdata,mdata.length(),"data");
  bs_.reset(1);
  a_.serialize(macl,"acl");
  bs_.reset(2);
  a_.serialize(mephemeral,"ephemeral");
  bs_.reset(3);
  a_.endRecord(*this,tag);
  return;
}
void org::apache::zookeeper::txn::CreateTxnV0::deserialize(::hadoop::IArchive& a_, const char* tag) {
  a_.startRecord(*this,tag);
  a_.deserialize(mpath,"path");
  bs_.set(0);
  { size_t len=0; a_.deserialize(mdata,len,"data");}
  bs_.set(1);
  a_.deserialize(macl,"acl");
  bs_.set(2);
  a_.deserialize(mephemeral,"ephemeral");
  bs_.set(3);
  a_.endRecord(*this,tag);
  return;
}
bool org::apache::zookeeper::txn::CreateTxnV0::validate() const {
  if (bs_.size() != bs_.count()) return false;
  return true;
}
bool org::apache::zookeeper::txn::CreateTxnV0::operator< (const org::apache::zookeeper::txn::CreateTxnV0& peer_) const {
  return (1
    && (mpath < peer_.mpath)
    && (mdata < peer_.mdata)
    && (macl < peer_.macl)
    && (mephemeral < peer_.mephemeral)
  );
}
bool org::apache::zookeeper::txn::CreateTxnV0::operator== (const org::apache::zookeeper::txn::CreateTxnV0& peer_) const {
  return (1
    && (mpath == peer_.mpath)
    && (mdata == peer_.mdata)
    && (macl == peer_.macl)
    && (mephemeral == peer_.mephemeral)
  );
}
const ::std::string&org::apache::zookeeper::txn::CreateTxnV0::type() const {
  static const ::std::string type_("CreateTxnV0");
  return type_;
}
const ::std::string&org::apache::zookeeper::txn::CreateTxnV0::signature() const {
  static const ::std::string sig_("LCreateTxnV0(sB[LACL(iLId(ss))]z)");
  return sig_;
}
void org::apache::zookeeper::txn::CreateTxn::serialize(::hadoop::OArchive& a_, const char* tag) const {
  if (!validate()) throw new ::hadoop::IOException("All fields not set.");
  a_.startRecord(*this,tag);
  a_.serialize(mpath,"path");
  bs_.reset(0);
  a_.serialize(mdata,mdata.length(),"data");
  bs_.reset(1);
  a_.serialize(macl,"acl");
  bs_.reset(2);
  a_.serialize(mephemeral,"ephemeral");
  bs_.reset(3);
  a_.serialize(mparentCVersion,"parentCVersion");
  bs_.reset(4);
  a_.endRecord(*this,tag);
  return;
}
void org::apache::zookeeper::txn::CreateTxn::deserialize(::hadoop::IArchive& a_, const char* tag) {
  a_.startRecord(*this,tag);
  a_.deserialize(mpath,"path");
  bs_.set(0);
  { size_t len=0; a_.deserialize(mdata,len,"data");}
  bs_.set(1);
  a_.deserialize(macl,"acl");
  bs_.set(2);
  a_.deserialize(mephemeral,"ephemeral");
  bs_.set(3);
  a_.deserialize(mparentCVersion,"parentCVersion");
  bs_.set(4);
  a_.endRecord(*this,tag);
  return;
}
bool org::apache::zookeeper::txn::CreateTxn::validate() const {
  if (bs_.size() != bs_.count()) return false;
  return true;
}
bool org::apache::zookeeper::txn::CreateTxn::operator< (const org::apache::zookeeper::txn::CreateTxn& peer_) const {
  return (1
    && (mpath < peer_.mpath)
    && (mdata < peer_.mdata)
    && (macl < peer_.macl)
    && (mephemeral < peer_.mephemeral)
    && (mparentCVersion < peer_.mparentCVersion)
  );
}
bool org::apache::zookeeper::txn::CreateTxn::operator== (const org::apache::zookeeper::txn::CreateTxn& peer_) const {
  return (1
    && (mpath == peer_.mpath)
    && (mdata == peer_.mdata)
    && (macl == peer_.macl)
    && (mephemeral == peer_.mephemeral)
    && (mparentCVersion == peer_.mparentCVersion)
  );
}
const ::std::string&org::apache::zookeeper::txn::CreateTxn::type() const {
  static const ::std::string type_("CreateTxn");
  return type_;
}
const ::std::string&org::apache::zookeeper::txn::CreateTxn::signature() const {
  static const ::std::string sig_("LCreateTxn(sB[LACL(iLId(ss))]zi)");
  return sig_;
}
void org::apache::zookeeper::txn::DeleteTxn::serialize(::hadoop::OArchive& a_, const char* tag) const {
  if (!validate()) throw new ::hadoop::IOException("All fields not set.");
  a_.startRecord(*this,tag);
  a_.serialize(mpath,"path");
  bs_.reset(0);
  a_.endRecord(*this,tag);
  return;
}
void org::apache::zookeeper::txn::DeleteTxn::deserialize(::hadoop::IArchive& a_, const char* tag) {
  a_.startRecord(*this,tag);
  a_.deserialize(mpath,"path");
  bs_.set(0);
  a_.endRecord(*this,tag);
  return;
}
bool org::apache::zookeeper::txn::DeleteTxn::validate() const {
  if (bs_.size() != bs_.count()) return false;
  return true;
}
bool org::apache::zookeeper::txn::DeleteTxn::operator< (const org::apache::zookeeper::txn::DeleteTxn& peer_) const {
  return (1
    && (mpath < peer_.mpath)
  );
}
bool org::apache::zookeeper::txn::DeleteTxn::operator== (const org::apache::zookeeper::txn::DeleteTxn& peer_) const {
  return (1
    && (mpath == peer_.mpath)
  );
}
const ::std::string&org::apache::zookeeper::txn::DeleteTxn::type() const {
  static const ::std::string type_("DeleteTxn");
  return type_;
}
const ::std::string&org::apache::zookeeper::txn::DeleteTxn::signature() const {
  static const ::std::string sig_("LDeleteTxn(s)");
  return sig_;
}
void org::apache::zookeeper::txn::SetDataTxn::serialize(::hadoop::OArchive& a_, const char* tag) const {
  if (!validate()) throw new ::hadoop::IOException("All fields not set.");
  a_.startRecord(*this,tag);
  a_.serialize(mpath,"path");
  bs_.reset(0);
  a_.serialize(mdata,mdata.length(),"data");
  bs_.reset(1);
  a_.serialize(mversion,"version");
  bs_.reset(2);
  a_.endRecord(*this,tag);
  return;
}
void org::apache::zookeeper::txn::SetDataTxn::deserialize(::hadoop::IArchive& a_, const char* tag) {
  a_.startRecord(*this,tag);
  a_.deserialize(mpath,"path");
  bs_.set(0);
  { size_t len=0; a_.deserialize(mdata,len,"data");}
  bs_.set(1);
  a_.deserialize(mversion,"version");
  bs_.set(2);
  a_.endRecord(*this,tag);
  return;
}
bool org::apache::zookeeper::txn::SetDataTxn::validate() const {
  if (bs_.size() != bs_.count()) return false;
  return true;
}
bool org::apache::zookeeper::txn::SetDataTxn::operator< (const org::apache::zookeeper::txn::SetDataTxn& peer_) const {
  return (1
    && (mpath < peer_.mpath)
    && (mdata < peer_.mdata)
    && (mversion < peer_.mversion)
  );
}
bool org::apache::zookeeper::txn::SetDataTxn::operator== (const org::apache::zookeeper::txn::SetDataTxn& peer_) const {
  return (1
    && (mpath == peer_.mpath)
    && (mdata == peer_.mdata)
    && (mversion == peer_.mversion)
  );
}
const ::std::string&org::apache::zookeeper::txn::SetDataTxn::type() const {
  static const ::std::string type_("SetDataTxn");
  return type_;
}
const ::std::string&org::apache::zookeeper::txn::SetDataTxn::signature() const {
  static const ::std::string sig_("LSetDataTxn(sBi)");
  return sig_;
}
void org::apache::zookeeper::txn::CheckVersionTxn::serialize(::hadoop::OArchive& a_, const char* tag) const {
  if (!validate()) throw new ::hadoop::IOException("All fields not set.");
  a_.startRecord(*this,tag);
  a_.serialize(mpath,"path");
  bs_.reset(0);
  a_.serialize(mversion,"version");
  bs_.reset(1);
  a_.endRecord(*this,tag);
  return;
}
void org::apache::zookeeper::txn::CheckVersionTxn::deserialize(::hadoop::IArchive& a_, const char* tag) {
  a_.startRecord(*this,tag);
  a_.deserialize(mpath,"path");
  bs_.set(0);
  a_.deserialize(mversion,"version");
  bs_.set(1);
  a_.endRecord(*this,tag);
  return;
}
bool org::apache::zookeeper::txn::CheckVersionTxn::validate() const {
  if (bs_.size() != bs_.count()) return false;
  return true;
}
bool org::apache::zookeeper::txn::CheckVersionTxn::operator< (const org::apache::zookeeper::txn::CheckVersionTxn& peer_) const {
  return (1
    && (mpath < peer_.mpath)
    && (mversion < peer_.mversion)
  );
}
bool org::apache::zookeeper::txn::CheckVersionTxn::operator== (const org::apache::zookeeper::txn::CheckVersionTxn& peer_) const {
  return (1
    && (mpath == peer_.mpath)
    && (mversion == peer_.mversion)
  );
}
const ::std::string&org::apache::zookeeper::txn::CheckVersionTxn::type() const {
  static const ::std::string type_("CheckVersionTxn");
  return type_;
}
const ::std::string&org::apache::zookeeper::txn::CheckVersionTxn::signature() const {
  static const ::std::string sig_("LCheckVersionTxn(si)");
  return sig_;
}
void org::apache::zookeeper::txn::SetACLTxn::serialize(::hadoop::OArchive& a_, const char* tag) const {
  if (!validate()) throw new ::hadoop::IOException("All fields not set.");
  a_.startRecord(*this,tag);
  a_.serialize(mpath,"path");
  bs_.reset(0);
  a_.serialize(macl,"acl");
  bs_.reset(1);
  a_.serialize(mversion,"version");
  bs_.reset(2);
  a_.endRecord(*this,tag);
  return;
}
void org::apache::zookeeper::txn::SetACLTxn::deserialize(::hadoop::IArchive& a_, const char* tag) {
  a_.startRecord(*this,tag);
  a_.deserialize(mpath,"path");
  bs_.set(0);
  a_.deserialize(macl,"acl");
  bs_.set(1);
  a_.deserialize(mversion,"version");
  bs_.set(2);
  a_.endRecord(*this,tag);
  return;
}
bool org::apache::zookeeper::txn::SetACLTxn::validate() const {
  if (bs_.size() != bs_.count()) return false;
  return true;
}
bool org::apache::zookeeper::txn::SetACLTxn::operator< (const org::apache::zookeeper::txn::SetACLTxn& peer_) const {
  return (1
    && (mpath < peer_.mpath)
    && (macl < peer_.macl)
    && (mversion < peer_.mversion)
  );
}
bool org::apache::zookeeper::txn::SetACLTxn::operator== (const org::apache::zookeeper::txn::SetACLTxn& peer_) const {
  return (1
    && (mpath == peer_.mpath)
    && (macl == peer_.macl)
    && (mversion == peer_.mversion)
  );
}
const ::std::string&org::apache::zookeeper::txn::SetACLTxn::type() const {
  static const ::std::string type_("SetACLTxn");
  return type_;
}
const ::std::string&org::apache::zookeeper::txn::SetACLTxn::signature() const {
  static const ::std::string sig_("LSetACLTxn(s[LACL(iLId(ss))]i)");
  return sig_;
}
void org::apache::zookeeper::txn::SetMaxChildrenTxn::serialize(::hadoop::OArchive& a_, const char* tag) const {
  if (!validate()) throw new ::hadoop::IOException("All fields not set.");
  a_.startRecord(*this,tag);
  a_.serialize(mpath,"path");
  bs_.reset(0);
  a_.serialize(mmax,"max");
  bs_.reset(1);
  a_.endRecord(*this,tag);
  return;
}
void org::apache::zookeeper::txn::SetMaxChildrenTxn::deserialize(::hadoop::IArchive& a_, const char* tag) {
  a_.startRecord(*this,tag);
  a_.deserialize(mpath,"path");
  bs_.set(0);
  a_.deserialize(mmax,"max");
  bs_.set(1);
  a_.endRecord(*this,tag);
  return;
}
bool org::apache::zookeeper::txn::SetMaxChildrenTxn::validate() const {
  if (bs_.size() != bs_.count()) return false;
  return true;
}
bool org::apache::zookeeper::txn::SetMaxChildrenTxn::operator< (const org::apache::zookeeper::txn::SetMaxChildrenTxn& peer_) const {
  return (1
    && (mpath < peer_.mpath)
    && (mmax < peer_.mmax)
  );
}
bool org::apache::zookeeper::txn::SetMaxChildrenTxn::operator== (const org::apache::zookeeper::txn::SetMaxChildrenTxn& peer_) const {
  return (1
    && (mpath == peer_.mpath)
    && (mmax == peer_.mmax)
  );
}
const ::std::string&org::apache::zookeeper::txn::SetMaxChildrenTxn::type() const {
  static const ::std::string type_("SetMaxChildrenTxn");
  return type_;
}
const ::std::string&org::apache::zookeeper::txn::SetMaxChildrenTxn::signature() const {
  static const ::std::string sig_("LSetMaxChildrenTxn(si)");
  return sig_;
}
void org::apache::zookeeper::txn::CreateSessionTxn::serialize(::hadoop::OArchive& a_, const char* tag) const {
  if (!validate()) throw new ::hadoop::IOException("All fields not set.");
  a_.startRecord(*this,tag);
  a_.serialize(mtimeOut,"timeOut");
  bs_.reset(0);
  a_.endRecord(*this,tag);
  return;
}
void org::apache::zookeeper::txn::CreateSessionTxn::deserialize(::hadoop::IArchive& a_, const char* tag) {
  a_.startRecord(*this,tag);
  a_.deserialize(mtimeOut,"timeOut");
  bs_.set(0);
  a_.endRecord(*this,tag);
  return;
}
bool org::apache::zookeeper::txn::CreateSessionTxn::validate() const {
  if (bs_.size() != bs_.count()) return false;
  return true;
}
bool org::apache::zookeeper::txn::CreateSessionTxn::operator< (const org::apache::zookeeper::txn::CreateSessionTxn& peer_) const {
  return (1
    && (mtimeOut < peer_.mtimeOut)
  );
}
bool org::apache::zookeeper::txn::CreateSessionTxn::operator== (const org::apache::zookeeper::txn::CreateSessionTxn& peer_) const {
  return (1
    && (mtimeOut == peer_.mtimeOut)
  );
}
const ::std::string&org::apache::zookeeper::txn::CreateSessionTxn::type() const {
  static const ::std::string type_("CreateSessionTxn");
  return type_;
}
const ::std::string&org::apache::zookeeper::txn::CreateSessionTxn::signature() const {
  static const ::std::string sig_("LCreateSessionTxn(i)");
  return sig_;
}
void org::apache::zookeeper::txn::ErrorTxn::serialize(::hadoop::OArchive& a_, const char* tag) const {
  if (!validate()) throw new ::hadoop::IOException("All fields not set.");
  a_.startRecord(*this,tag);
  a_.serialize(merr,"err");
  bs_.reset(0);
  a_.endRecord(*this,tag);
  return;
}
void org::apache::zookeeper::txn::ErrorTxn::deserialize(::hadoop::IArchive& a_, const char* tag) {
  a_.startRecord(*this,tag);
  a_.deserialize(merr,"err");
  bs_.set(0);
  a_.endRecord(*this,tag);
  return;
}
bool org::apache::zookeeper::txn::ErrorTxn::validate() const {
  if (bs_.size() != bs_.count()) return false;
  return true;
}
bool org::apache::zookeeper::txn::ErrorTxn::operator< (const org::apache::zookeeper::txn::ErrorTxn& peer_) const {
  return (1
    && (merr < peer_.merr)
  );
}
bool org::apache::zookeeper::txn::ErrorTxn::operator== (const org::apache::zookeeper::txn::ErrorTxn& peer_) const {
  return (1
    && (merr == peer_.merr)
  );
}
const ::std::string&org::apache::zookeeper::txn::ErrorTxn::type() const {
  static const ::std::string type_("ErrorTxn");
  return type_;
}
const ::std::string&org::apache::zookeeper::txn::ErrorTxn::signature() const {
  static const ::std::string sig_("LErrorTxn(i)");
  return sig_;
}
void org::apache::zookeeper::txn::Txn::serialize(::hadoop::OArchive& a_, const char* tag) const {
  if (!validate()) throw new ::hadoop::IOException("All fields not set.");
  a_.startRecord(*this,tag);
  a_.serialize(mtype,"type");
  bs_.reset(0);
  a_.serialize(mdata,mdata.length(),"data");
  bs_.reset(1);
  a_.endRecord(*this,tag);
  return;
}
void org::apache::zookeeper::txn::Txn::deserialize(::hadoop::IArchive& a_, const char* tag) {
  a_.startRecord(*this,tag);
  a_.deserialize(mtype,"type");
  bs_.set(0);
  { size_t len=0; a_.deserialize(mdata,len,"data");}
  bs_.set(1);
  a_.endRecord(*this,tag);
  return;
}
bool org::apache::zookeeper::txn::Txn::validate() const {
  if (bs_.size() != bs_.count()) return false;
  return true;
}
bool org::apache::zookeeper::txn::Txn::operator< (const org::apache::zookeeper::txn::Txn& peer_) const {
  return (1
    && (mtype < peer_.mtype)
    && (mdata < peer_.mdata)
  );
}
bool org::apache::zookeeper::txn::Txn::operator== (const org::apache::zookeeper::txn::Txn& peer_) const {
  return (1
    && (mtype == peer_.mtype)
    && (mdata == peer_.mdata)
  );
}
const ::std::string&org::apache::zookeeper::txn::Txn::type() const {
  static const ::std::string type_("Txn");
  return type_;
}
const ::std::string&org::apache::zookeeper::txn::Txn::signature() const {
  static const ::std::string sig_("LTxn(iB)");
  return sig_;
}
void org::apache::zookeeper::txn::MultiTxn::serialize(::hadoop::OArchive& a_, const char* tag) const {
  if (!validate()) throw new ::hadoop::IOException("All fields not set.");
  a_.startRecord(*this,tag);
  a_.serialize(mtxns,"txns");
  bs_.reset(0);
  a_.endRecord(*this,tag);
  return;
}
void org::apache::zookeeper::txn::MultiTxn::deserialize(::hadoop::IArchive& a_, const char* tag) {
  a_.startRecord(*this,tag);
  a_.deserialize(mtxns,"txns");
  bs_.set(0);
  a_.endRecord(*this,tag);
  return;
}
bool org::apache::zookeeper::txn::MultiTxn::validate() const {
  if (bs_.size() != bs_.count()) return false;
  return true;
}
bool org::apache::zookeeper::txn::MultiTxn::operator< (const org::apache::zookeeper::txn::MultiTxn& peer_) const {
  return (1
    && (mtxns < peer_.mtxns)
  );
}
bool org::apache::zookeeper::txn::MultiTxn::operator== (const org::apache::zookeeper::txn::MultiTxn& peer_) const {
  return (1
    && (mtxns == peer_.mtxns)
  );
}
const ::std::string&org::apache::zookeeper::txn::MultiTxn::type() const {
  static const ::std::string type_("MultiTxn");
  return type_;
}
const ::std::string&org::apache::zookeeper::txn::MultiTxn::signature() const {
  static const ::std::string sig_("LMultiTxn([LTxn(iB)])");
  return sig_;
}
