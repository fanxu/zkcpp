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

#ifndef SRC_CONTRIB_ZKCPP_INCLUDE_ZOOKEEPER_H_
#define SRC_CONTRIB_ZKCPP_INCLUDE_ZOOKEEPER_H_

#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/utility.hpp>
#include <stdint.h>
#include <string>
#include <vector>
#include "zookeeper.jute.hh"
#include "zookeeper_const.hh"
#include "zookeeper_multi.hh"

/** Org namespace. */
namespace org {

/** Apache namespace. */
namespace apache {

/** ZooKeeper namespace. */
namespace zookeeper {

/**
 * Callback interface for watch event.
 */
class Watch {
  public:
    /**
     * @param event Event type that caused this watch to trigger.
     * @param state State of the zookeeper session.
     * @param path Znode path where this watch was set to.
     */
    virtual void process(WatchEvent::type event, SessionState::type state,
                         const std::string& path) = 0;
    virtual ~Watch() {}
};

/**
 * Callback interface for ZooKeeper::set() operation.
 */
class SetCallback {
  public:
    /**
     * @param rc Ok if this set() operation was successful.
     * @param path The path of the znode this set() operation was for
     * @param stat Stat of the resulting znode. Valid iff rc == Ok.
     */
    virtual void process(ReturnCode::type rc, const std::string& path,
                         const data::Stat& stat) = 0;
    virtual ~SetCallback() {}
};

/**
 * Callback interface for ZooKeeper::exists() operation.
 */
class ExistsCallback {
  public:
    /**
     * @param rc Ok if this znode exists.
     * @param path The path of the znode this exists() operation was for
     * @param stat stat of the znode. Valid iff rc == Ok.
     */
    virtual void process(ReturnCode::type rc, const std::string& path,
                         const data::Stat& stat) = 0;
    virtual ~ExistsCallback() {}
};

/**
 * Callback interface for ZooKeeper::get() operation.
 */
class GetCallback {
  public:
    /**
     * @param rc Ok if this get() operation was successful.
     * @param path The path of the znode this get() operation was for
     * @param data Data associated with this znode. Valid iff rc == Ok.
     * @param stat Stat associated with this znode. Valid iff rc == Ok.
     */
    virtual void process(ReturnCode::type rc, const std::string& path,
                         const std::string& data, const data::Stat& stat) = 0;
    virtual ~GetCallback() {}
};

/**
 * Callback interface for ZooKeeper::getAcl() operation.
 */
class GetAclCallback {
  public:
    /**
     * @param rc Ok if this getAcl() operation was successful.
     * @param path The path of the znode this getAcl() operation was for
     * @param acl The list of ACLs for this znode. Valid iff rc == Ok.
     * @param stat Stat associated with this znode. Valid iff rc == Ok.
     */
    virtual void process(ReturnCode::type rc, const std::string& path,
                         const std::vector<data::ACL>& acl,
                         const data::Stat& stat) = 0;
    virtual ~GetAclCallback() {}
};

/**
 * Callback interface for ZooKeeper::getChildren() operation.
 */
class GetChildrenCallback {
  public:
    /**
     * @param rc Ok if this getChildren() operation was successful.
     * @param path The path of the znode this getChildren() operation was for
     * @param children The list of children for this znode. Valid iff rc == Ok.
     * @param stat Stat associated with this znode. Valid iff rc == Ok.
     */
    virtual void process(ReturnCode::type rc, const std::string& path,
                         const std::vector<std::string>& children,
                         const data::Stat& stat) = 0;
    virtual ~GetChildrenCallback() {}
};

/**
 * Callback interface for ZooKeeper::create() operation.
 */
class CreateCallback {
  public:
    /**
     * @param rc One of the ReturnCode::type enums. Most common values are:
     *           - ReturnCode::Ok If the znode was created successfully.
     *           - ReturnCode::NodeExists If the znode already exists.
     *
     * @param pathRequested The path of the znode this operation was for.
     * @param pathCreated The path of the znode that was created by this
     *                    request. Valid iff rc == Ok. This is useful only
     *                    for sequential znode, in which the path of the
     *                    resulting znode is different from the path originally
     *                    specified in the request. For non-seuquential znode,
     *                    this is equal to pathRequested. See CreateMode::type
     *                    for more detail about sequential znode path names.
     */
    virtual void process(ReturnCode::type rc, const std::string& pathRequested,
                         const std::string& pathCreated) = 0;
    virtual ~CreateCallback() {}
};

/**
 * Callback interface for ZooKeeper::remove() operation.
 */
class RemoveCallback {
  public:
    /**
     * @param rc One of the ReturnCode::type enums. Most common values are:
     *           - ReturnCode::Ok If the znode was removed successfully.
     *           - ReturnCode::NoNode Znode doesn't exist.
     *           - ReturnCode::NotEmpty Znode has one or more children.
     * @param path The path of the znode this operation was for.
     */
    virtual void process(ReturnCode::type rc, const std::string& path) = 0;
    virtual ~RemoveCallback() {}
};

/**
 * Callback interface for ZooKeeper::setAcl() operation.
 */
class SetAclCallback {
  public:
    /**
     * @param rc ReturnCode::Ok if this setAcl() operation was successful.
     * @param path The path of the znode this operation was for.
     */
    virtual void process(ReturnCode::type rc, const std::string& path) = 0;
    virtual ~SetAclCallback() {}
};

/**
 * Callback interface for ZooKeeper::sync() operation.
 */
class SyncCallback {
  public:
    /**
     * @param rc ReturnCode::Ok if this sync() operation was successful.
     * @param path The path of the znode this operation was for.
     */
    virtual void process(ReturnCode::type rc, const std::string& path) = 0;
    virtual ~SyncCallback() {}
};

/**
 * Callback interface for ZooKeeper::addAuth() operation.
 */
class AddAuthCallback {
  public:
    /**
     * @param rc Ok if this addAuth() operation was successful.
     * @param scheme The scheme used for this operation.
     * @param cert The certificate used for this operation.
     */
    virtual void process(ReturnCode::type rc, const std::string& scheme,
                         const std::string& cert) = 0;
    virtual ~AddAuthCallback() {}
};

/**
 * Callback interface for ZooKeeper::multi() operation.
 */
class MultiCallback {
  public:
    /**
     * @param rc Ok if this multi() operation was successful.
     * @param results The results of a multi operation.
     */
    virtual void process(ReturnCode::type rc,
                         const boost::ptr_vector<OpResult>& results) = 0;
    virtual ~MultiCallback() {}
};

class ZooKeeperImpl;
class ZooKeeper : boost::noncopyable {
  public:
    ZooKeeper();
    ~ZooKeeper();

    /**
     * Initializes ZooKeeper session asynchronously.
     */
    ReturnCode::type init(const std::string& hosts, int32_t sessionTimeoutMs,
                    boost::shared_ptr<Watch> watch);

    /**
     * Adds authentication info for this session asynchronously.
     *
     * The application calls this function to specify its credentials for
     * purposes of authentication. The server will use the security provider
     * specified by the scheme parameter to authenticate the client connection.
     * If the authentication request has failed:
     *   - The server connection is dropped, and the session state becomes
     *     SessionState::AuthFailed.
     *   - All the existing watchers are called for WatchEvent::Session event
     *     with SessionState::AuthFailed as the state parameter.
     *
     * @param scheme the id of authentication scheme. Natively supported:
     * "digest" password-based authentication
     * @param cert authentification certificate.
     * @param callback The callback to invoke when the request completes.
     *
     * @return Ok on successfu or one of the following errcodes on failure:
     * ZBADARGUMENTS - invalid input parameters
     * InvalidState - state is either Expired or SessionAuthFailed
     * ZMARSHALLINGERROR - failed to marshall a request; possibly, out of memory
     * ZSYSTEMERROR - a system error occured
     */
    ReturnCode::type addAuth(const std::string& scheme, const std::string& cert,
                       boost::shared_ptr<AddAuthCallback> callback);

    /**
     * Adds authentication info for this session synchronously.
     */
    ReturnCode::type addAuth(const std::string& scheme,
                             const std::string& cert);

    /**
     * Create a znode asynchronously.
     *
     * A znode can only be created if it does not already exists. The CreateMode::type
     * affect the creation of nodes. In Ephemeral mode, the node will
     * automatically get removed if the client session goes away. If the
     * Sequential mode is set, a unique monotonically increasing sequence
     * number is appended to the path name. The sequence number is always
     * fixed length of 10 digits, 0 padded.
     *
     * @param path The name of the znode.
     * @param data The data to be stored in the node.
     * @param acl The initial ACL of the node. The ACL must not be null or
     *            empty.
     * @param mode
     * @param callback the routine to invoke when the request completes.
     *                 The completion will be triggered with one of the
     *                 following codes passed in as the rc argument:
     *
     * ZOK operation completed successfully
     * ZNONODE the parent node does not exist.
     * ZNODEEXISTS the node already exists
     * ZNOAUTH the client does not have permission.
     * ZNOCHILDRENFOREPHEMERALS cannot create children of ephemeral nodes.
     *
     * @return Ok on success
     *         BadArguments invalid input parameters
     *         InvalidState - zhandle state is either Expired or SessionAuthFailed
     *         MarshallingError - failed to marshall a request; possibly, out of memory
     */
    ReturnCode::type create(const std::string& path, const std::string& data,
                      const std::vector<data::ACL>& acl, CreateMode::type mode,
                      boost::shared_ptr<CreateCallback> callback);

    /**
     * Synchronously create a znode.
     */
    ReturnCode::type create(const std::string& path, const std::string& data,
                      const std::vector<data::ACL>& acl, CreateMode::type mode,
                      std::string& pathCreated);

    /**
     * Removes a znode asynchronously.
     *
     * @param path the name of the znode.
     * @param version the expected version of the znode. The function will fail
     *                if the actual version of the znode does not match the
     *                expected version. If -1 is used the version check will not
     *                take place.
     * @param callback The callback to invoke when the request completes.
     *
     * @return One of the ReturnCode::type enums. Most common values are:
     *         - ReturnCode::Ok if this request has been enqueued successfully.
     *         - ReturnCode::InvalidState - The session state is either in
     *           SessionState::Expired or in SessionState::AuthFailed.
     */
    ReturnCode::type remove(const std::string& path, int32_t version,
                      boost::shared_ptr<RemoveCallback> callback);

    /**
     * Removes a znode synchronously.
     */
    ReturnCode::type remove(const std::string& path, int32_t version);

    /**
     * Checks the existence of a znode in zookeeper asynchronously.
     *
     * This function allows one to specify a watch, a callback object. The callback
     * will be called once the watch has fired. The associated context data will be 
     * passed to the function as the watcher context parameter. 
     * 
     * @param path The name of the node.
     * @param watch if non-null a watch will set on the specified znode on the server.
     * The watch will be set even if the node does not exist. This allows clients 
     * to watch for nodes to appear.
     * @param callback The callback to invoke when the request completes. If you don't
     *                 need to get called back, pass an empty pointer (i.e. 
     *                 boost::shared_ptr<ExistsCallback>()). 
     *
     * @return One of the ReturnCode::type enums. Most common values are:
     *         - ReturnCode::Ok if this request has been enqueued successfully.
     *         - ReturnCode::InvalidState - The session state is either in
     *           SessionState::Expired or in SessionState::AuthFailed.
     * </ul>
     */
    ReturnCode::type exists(const std::string& path,
            boost::shared_ptr<Watch> watch,
            boost::shared_ptr<ExistsCallback> callback);

    /**
     * A synchronous version of exists().
     *
     * @param path The name of the node.
     * @param watch if non-null a watch will set on the specified znode on the server.
     * The watch will be set even if the node does not exist. This allows clients
     * to watch for nodes to appear.
     * @param[out] stat The stat of this znode. Valid iff rc == ReturnCode::Ok.
     *
     * @return One of the ReturnCode::type enums. Most common values are:
     *         - ReturnCode::Ok The znode exists.
     *         - ReturnCode::NoNode The znode does not exist.
     */
    ReturnCode::type exists(const std::string& path,
                            boost::shared_ptr<Watch> watch, data::Stat& stat);

    /**
     * Gets the data associated with a znode.
     *
     * @param path The name of the znode.
     * @param watch If non-null, a watch will be set at the server to notify
     * the client if the znode changes.
     * @param callback The callback to invoke when the request completes.
     *
     * @return ReturnCode::Ok if the request has been enqueued successfully.
     */
    ReturnCode::type get(const std::string& path,
                         boost::shared_ptr<Watch> watch,
                         boost::shared_ptr<GetCallback> callback);

    /**
     * Gets the data associated with a znode synchronously.
     */
    ReturnCode::type get(const std::string& path,
                         boost::shared_ptr<Watch> watch,
                         std::string& data, data::Stat& stat);

    /**
     * Sets the data associated with a znode.
     *
     * @param path The name of the znode.
     * @param data Data to set.
     * @param version The expected version of the znode. This operation will fail if
     *                the actual version of the node does not match the expected
     *                version. If -1 is used the version check will not take place.
     * @param callback The callback to invoke when the request completes.
     *
     * @return ReturnCode::Ok if the request has been enqueued successfully.
     */
    ReturnCode::type set(const std::string& path, const std::string& data,
                   int32_t version, boost::shared_ptr<SetCallback> callback);

    /**
     * Sets the data associated with a znode synchronously.
     */
    ReturnCode::type set(const std::string& path, const std::string& data,
                         int32_t version, data::Stat& stat);

    /**
     * Gets the children and the stat of a znode asynchronously.
     *
     * @param path The name of the znode.
     * @param watch If non-null, a watch will be set at the server to notify
     *              the client if the node changes.
     * @param callback The callback to invoke when the request completes.
     *
     * @return ReturnCode::Ok if the request has been enqueued successfully.
     */
    ReturnCode::type getChildren(const std::string& path,
                           boost::shared_ptr<Watch> watch,
                           boost::shared_ptr<GetChildrenCallback> callback);

    /**
     * Gets the children and the stat of a znode synchronously.
     *
     * @param path The name of the znode.
     * @param watch If non-null, a watch will be set at the server to notify
     *              the client if the node changes.
     * @param callback The callback to invoke when the request completes.
     *
     * @return ReturnCode::Ok if the request has been enqueued successfully.
     */
    ReturnCode::type getChildren(const std::string& path,
                           boost::shared_ptr<Watch> watch,
                           std::vector<std::string>& children,
                           data::Stat& stat);

    /**
     * Gets the acl associated with a znode.
     *
     * @param path The name of the znode.
     * @param callback The callback to invoke when the request completes.
     *
     * @return ReturnCode::Ok if the request has been enqueued successfully.
     */
    ReturnCode::type getAcl(const std::string& path,
                      boost::shared_ptr<GetAclCallback> callback);

    /**
     * Gets the acl associated with a znode synchronously.
     */
    ReturnCode::type getAcl(const std::string& path,
                            std::vector<data::ACL>& acl, data::Stat& stat);

    /**
     * Sets the Acl associated with a znode.
     *
     * @param path The name of the znode.
     * @param version The expected version of the znode. This operation will fail if
     *                the actual version of the node does not match the expected
     *                version. If -1 is used the version check will not take place.
     * @param acl ACL to set.
     * @param callback The callback to invoke when the request completes.
     *
     * @return ReturnCode::Ok if the request has been enqueued successfully.
     */
    ReturnCode::type setAcl(const std::string& path, int32_t version,
                            const std::vector<data::ACL>& acl,
                            boost::shared_ptr<SetAclCallback> callback);

    /**
     * Sets the Acl associated with a znode synchronously.
     *
     * @param path The name of the znode.
     * @param version The expected version of the znode. This operation will fail if
     *                the actual version of the node does not match the expected
     *                version. If -1 is used the version check will not take place.
     * @param acl Acl to set.
     * @param callback The callback to invoke when the request completes.
     *
     * @return ReturnCode::Ok if the request has been enqueued successfully.
     */
    ReturnCode::type setAcl(const std::string& path, int32_t version,
                            const std::vector<data::ACL>& acl);

    /**
     * Asynchronously flushes the channel between process and leader.
     *
     * When the callback gets invoked with ReturnCode Ok, ZooKeeper guarantees
     * that all the operations requested before calling sync() have been
     * completed and their callbacks have been invoked.
     *
     * @param path Currently unused. You can pass any value and this method will
     *             simply ignore it, except that you'll get this value back in
     *             the callback. This parameter might get used in the future
     *             when ZooKeeper supports multiple namespaces to only sync the
     *             namespace that contains the path.
     * @param callback the routine to invoke when the request completes.
     *
     * @return Ok If the request was enqueued successfully. One of the errors in
     *         the ReturnCode in case of failure.
     */
    ReturnCode::type sync(const std::string& path,
                    boost::shared_ptr<SyncCallback> callback);

    /**
     * Atomically executes multiple operations.
     */
    ReturnCode::type multi(const boost::ptr_vector<Op>& ops,
                           boost::shared_ptr<MultiCallback> callback);

    /**
     * Synchronous version of multi.
     */
    ReturnCode::type multi(const boost::ptr_vector<Op>& ops,
                           boost::ptr_vector<OpResult>& results);

    /**
     * Closes this ZooKeeper session.
     *
     * After this call, the client session will no longer be valid.
     * The function will flush any outstanding send requests before return.
     * As a result it may block.
     *
     * Ok success
     * BadArguments Invalid input parameters.
     * MarshallingError Failed to marshall a request; possibly out of memory.
     * OperationTimeout Failed to flush the buffers within the specified timeout.
     * ConnectionLoss A network error occured while attempting to send request to server
     */
    ReturnCode::type close();

    /**
     * Gets the current state of this ZooKeeper object.
     *
     * @see State
     */
    SessionState::type getState();

    /**
     * Gets the ZooKeeper session ID.
     *
     * This ZooKeeper object must be in "Connected" state for this operation
     * to succeed.
     *
     * @param[out] id Session ID.
     */
    ReturnCode::type getSessionId(int64_t& id) {
      return ReturnCode::Unimplemented;
    }

    /**
     * Gets the ZooKeeper session password.
     *
     * This ZooKeeper object must be in "Connected" state for this operation
     * to succeed.
     *
     * @param[out] password Session password.
     */
    ReturnCode::type getSessionPassword(std::string& password) {
      return ReturnCode::Unimplemented;
    }

  private:
    ZooKeeperImpl* impl_;
};
}}}

#endif  // SRC_CONTRIB_ZKCPP_INCLUDE_ZOOKEEPER_H_
