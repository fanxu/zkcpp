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

#ifndef ZOOKEEPER_H_
#define ZOOKEEPER_H_

#include <stdlib.h>
#ifndef WIN32
#include <sys/socket.h>
#include <sys/time.h>
#else
#include "winconfig.h"
#endif
#include <stdio.h>
#include <ctype.h>

#include <boost/ptr_container/ptr_vector.hpp>
#include "zookeeper/zookeeper.hh"
using namespace org::apache::zookeeper;

#ifdef __cplusplus
extern "C" {
#endif

extern const int ZOOKEEPER_WRITE;
extern const int ZOOKEEPER_READ;
class zhandle_t;

zhandle_t *zookeeper_init(const char *host, boost::shared_ptr<Watch> watch,
  int recv_timeout, int flags);
int zookeeper_close(zhandle_t *zh);
int zoo_recv_timeout(zhandle_t *zh);
const void *zoo_get_context(zhandle_t *zh);
void zoo_set_context(zhandle_t *zh, void *context);
struct sockaddr* zookeeper_get_connected_host(zhandle_t *zh,
        struct sockaddr *addr, socklen_t *addr_len);
typedef void (*void_completion_t)(int rc, const void *data);
typedef void (*multi_completion_t)(int rc,
             const boost::ptr_vector<OpResult>& results,
             const void *data);
typedef void (*stat_completion_t)(int rc, const data::Stat& stat,
        const void *data);
typedef void (*data_completion_t)(int rc, const std::string& value,
        const org::apache::zookeeper::data::Stat& stat, const void *data);
typedef void (*strings_completion_t)(int rc,
        const std::vector<std::string>& strings, const void *data);
typedef void (*strings_stat_completion_t)(int rc,
        const std::vector<std::string>& strings, const data::Stat& stat,
        const void *data);
typedef void
        (*string_completion_t)(int rc, const std::string& value, const void *data);
typedef void (*acl_completion_t)(int rc, const std::vector<data::ACL>& acl,
        const data::Stat& stat, const void *data);
SessionState::type zoo_state(zhandle_t *zh);
ReturnCode::type zoo_acreate(zhandle_t *zh, const std::string& path, const char *value,
        int valuelen, const std::vector<org::apache::zookeeper::data::ACL>& acl,
        int flags, string_completion_t completion, const void *data,
        bool isSynchronous);
int zoo_adelete(zhandle_t *zh, const std::string& path, int version,
        void_completion_t completion, const void *data, bool isSynchronous);
int zoo_awexists(zhandle_t *zh, const std::string& path,
        boost::shared_ptr<Watch> watch,
        stat_completion_t completion, const void *data, bool isSynchronous);
int zoo_aget(zhandle_t *zh, const std::string& path, int watch,
        data_completion_t completion, const void *data);
int zoo_awget(zhandle_t *zh, const std::string& path,
        boost::shared_ptr<Watch> watch,
        data_completion_t completion, const void *data, bool isSynchronous);
int zoo_aset(zhandle_t *zh, const std::string& path, const char *buffer, int buflen, 
        int version, stat_completion_t completion, const void *data,
        bool isSynchronous);
int zoo_awget_children2(zhandle_t *zh, const std::string& path,
        boost::shared_ptr<Watch> watch,
        strings_stat_completion_t completion, const void *data,
        bool isSynchronous);
int zoo_async(zhandle_t *zh, const std::string& path,
        string_completion_t completion, const void *data);
int zoo_aget_acl(zhandle_t *zh, const std::string& path, acl_completion_t completion, 
        const void *data, bool isSynchronous);
int zoo_aset_acl(zhandle_t *zh, const std::string& path, int version,
        const std::vector<org::apache::zookeeper::data::ACL>& acl,
        void_completion_t, const void *data, bool isSynchronous);
int zoo_amulti(zhandle_t *zh,
        const boost::ptr_vector<org::apache::zookeeper::Op>& ops,
        multi_completion_t, const void *data, bool isSynchronous);
int zoo_add_auth(zhandle_t *zh,const char* scheme,const char* cert, 
	int certLen, void_completion_t completion, const void *data,
        bool isSynchronous);
bool is_unrecoverable(zhandle_t *zh);
void zoo_deterministic_conn_order(int yesOrNo);

#ifdef __cplusplus
}
#endif

#endif /*ZOOKEEPER_H_*/
