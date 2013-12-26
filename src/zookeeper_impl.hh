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
#ifndef SRC_CONTRIB_ZKCPP_INCLUDE_ZOOKEEPERIMPL_H_
#define SRC_CONTRIB_ZKCPP_INCLUDE_ZOOKEEPERIMPL_H_

#include <stdint.h>
#include <string>
#include "zookeeper/zookeeper.hh"
#include "zookeeper.h"

namespace org { namespace apache { namespace zookeeper {

class ZooKeeperImpl {
  public:
    ZooKeeperImpl();
    ~ZooKeeperImpl();
    ReturnCode::type init(const std::string& hosts, int32_t sessionTimeoutMs,
                    boost::shared_ptr<Watch> watch);
    ReturnCode::type addAuth(const std::string& scheme, const std::string& cert,
                       boost::shared_ptr<AddAuthCallback> callback,
                       bool isSynchronous);
    ReturnCode::type addAuth(const std::string& scheme,
                             const std::string& cert);
    ReturnCode::type create(const std::string& path, const std::string& data,
                      const std::vector<data::ACL>& acl, CreateMode::type mode,
                      boost::shared_ptr<CreateCallback> callback,
                      bool isSynchronous);
    ReturnCode::type create(const std::string& path, const std::string& data,
                      const std::vector<data::ACL>& acl, CreateMode::type mode,
                      std::string& pathCreated);
    ReturnCode::type remove(const std::string& path, int version,
                      boost::shared_ptr<RemoveCallback> callback,
                      bool isSynchronous);
    ReturnCode::type remove(const std::string& path, int version);
    ReturnCode::type exists(const std::string& path,
            boost::shared_ptr<Watch> watch,
            boost::shared_ptr<ExistsCallback> callback,
            bool isSynchronous);
    ReturnCode::type exists(const std::string& path, boost::shared_ptr<Watch> watch,
                            data::Stat& stat);
    ReturnCode::type get(const std::string& path,
                   boost::shared_ptr<Watch>,
                   boost::shared_ptr<GetCallback> callback,
                   bool isSynchronous);
    ReturnCode::type get(const std::string& path,
                         boost::shared_ptr<Watch> watch,
                         std::string& data, data::Stat& stat);
    ReturnCode::type set(const std::string& path, const std::string& data,
                   int version, boost::shared_ptr<SetCallback> callback,
                   bool isSynchronous);
    ReturnCode::type set(const std::string& path, const std::string& data,
                   int version, data::Stat& stat);
    ReturnCode::type getChildren(const std::string& path,
                           boost::shared_ptr<Watch> watch,
                           boost::shared_ptr<GetChildrenCallback> callback,
                           bool isSynchronous);
    ReturnCode::type getChildren(const std::string& path,
                           boost::shared_ptr<Watch> watch,
                           std::vector<std::string>& children,
                           data::Stat& stat);
    ReturnCode::type getAcl(const std::string& path,
                      boost::shared_ptr<GetAclCallback> callback,
                      bool isSynchronous);
    ReturnCode::type getAcl(const std::string& path,
                            std::vector<data::ACL>& acl, data::Stat& stat);
    ReturnCode::type setAcl(const std::string& path, int version,
                      const std::vector<data::ACL>& acl,
                      boost::shared_ptr<SetAclCallback> callback,
                      bool isSynchronous);
    ReturnCode::type setAcl(const std::string& path, int32_t version,
                      const std::vector<data::ACL>& acl);
    ReturnCode::type sync(const std::string& path,
                    boost::shared_ptr<SyncCallback> callback);
    ReturnCode::type multi(const boost::ptr_vector<Op>& ops,
                           boost::shared_ptr<MultiCallback> callback,
                           bool isSynchronous);
    ReturnCode::type multi(const boost::ptr_vector<Op>& ops,
                           boost::ptr_vector<OpResult>& results);
    ReturnCode::type close();
    SessionState::type getState();
    void setState(SessionState::type state);

  private:
    static void watchCallback(zhandle_t *zh, int type, int state, const char *path,
                         void *watcherCtx);
    static void stringCompletion(int rc, const std::string& value,
                                 const void *data);
    static void removeCompletion(int rc, const void *data);
    static void setAclCompletion(int rc, const void *data);
    static void syncCompletion(int rc, const std::string& path, const void *data);
    static void existsCompletion(int rc, const data::Stat& stat,
                                 const void* data);
    static void setCompletion(int rc, const data::Stat& stat,
                              const void* data);
    static void dataCompletion(int rc, const std::string& value,
                               const data::Stat& stat, const void *data);
    static void childrenCompletion(int rc, const std::vector<std::string>& children,
                                   const data::Stat& stat, const void *data);
    static void aclCompletion(int rc, const std::vector<data::ACL>& acl,
                              const data::Stat& stat, const void *data);
    static void authCompletion(int rc, const void *data);
    static void syncCompletion(int rc, const char *value, const void *data);
    static void multiCompletion(int rc,
      const boost::ptr_vector<OpResult>& results, const void* data);
    zhandle_t* handle_;
    bool inited_;
    SessionState::type state_;
};
}}}

#endif  // SRC_CONTRIB_ZKCPP_INCLUDE_ZOOKEEPERIMPL_H_
