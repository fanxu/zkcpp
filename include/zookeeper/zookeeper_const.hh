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

#ifndef SRC_CONTRIB_ZKCPP_INCLUDE_ZOOKEEPER_CONST_H_
#define SRC_CONTRIB_ZKCPP_INCLUDE_ZOOKEEPER_CONST_H_

#include <string>

namespace org {
namespace apache {

/** ZooKeeper namespace. */
namespace zookeeper {

/**
 * ZooKeeper return codes.
 */
namespace ReturnCode {
  enum type {
    /** Everything is OK */
    Ok = 0,

    /**
     * System and server-side errors.
     *
     * SystemError is never returned by ZooKeeper. It is meant to be used
     * to indicate a range. Specifically error codes less than this value,
     * but greater than {@link #ApiError}, are system errors.
     */
    SystemError = -1,

    /** A runtime inconsistency was found */
    RuntimeInconsistency = -2,

    /** A data inconsistency was found */
    DataInconsistency = -3,

    /** Connection to the server has been lost */
    ConnectionLoss = -4,

    /** Error while marshalling or unmarshalling data */
    MarshallingError = -5,

    /** Operation is unimplemented */
    Unimplemented = -6,

    /** Operation timeout */
    OperationTimeout = -7,

    /** Invalid arguments */
    BadArguments = -8,

    /**
     * API errors.
     *
     * ApiError is never returned by the ZooKeeper. It is meant to be used
     * to indicate a range. Specifically error codes less than this
     * value are API errors (while values greater than this and less than
     * {@link #SystemError} indicate a {@link #SystemError}).
     */
    ApiError = -100,

    /** Node does not exist */
    NoNode = -101,

    /** Not authenticated */
    NoAuth = -102,

    /** Version conflict */
    BadVersion = -103,

    /** Ephemeral nodes may not have children */
    NoChildrenForEphemerals = -108,

    /** The node already exists */
    NodeExists = -110,

    /** The node has children */
    NotEmpty = -111,

    /** The session has been expired by the server */
    SessionExpired = -112,

    /** Invalid callback specified */
    InvalidCallback = -113,

    /** Invalid ACL specified */
    InvalidAcl = -114,

    /** Client authentication failed */
    AuthFailed = -115,

    /** Session moved to another server, so operation is ignored */
    SessionMoved = -118,

    /**
     * C++ library specific errors.
     *
     * CppError is never returned by the ZooKeeper. It is meant to be used
     * to indicate a range. Specifically error codes greater than this
     * value are C++ library specific errors.
     */
    CppError = 1,

    /** The session is in an invalid state for a given operation. */
    InvalidState = 2,

    /** Generic error */
    Error = 3,
  };

  const std::string toString(type rc);
}

/**
 * Namespace for session state enums.
 */
namespace SessionState {
  /**
   * ZooKeeper session states.
   *
   * A positive value indicates that the session is in a "recoverable" state.
   * A negative value indicates that the session is in an "unrecoverable" state.
   * Once the client is in an unrecoverable state, the session is no longer
   * valid. You need to call ZooKeeper::init() to establish a new session.
   *
   * For more details about recoverable and unrecoverable states, see:
   *
   * http://wiki.apache.org/hadoop/ZooKeeper/ErrorHandling
   */
  enum type {
    /**
     * The session has been expired and is no longer valid.
     */
    Expired = -112,

    /**
     * Session Authentication failed.
     */
    AuthFailed = -113,

    /**
     * The session is not connected to any ZooKeeper server.
     */
    Connecting = 1,

    /**
     * The session is connected to a ZooKeeper server.
     */
    Connected = 3,

    /*
     * TODO REMOVE
     */
    Associating,
  };

  /**
   * Returns SessionState::type enum in human-readable string format.
   */
  const std::string toString(type state);
};

/**
 * These constants indicate the event that caused a watch to trigger. They
 * are possible values of the first parameter of the watcher callback.
 */
namespace WatchEvent {
  enum type {
    /**
     * Session state has changed.
     *
     * This is generated when a client loses contact or reconnects with a
     * server.
     */
    SessionStateChanged = -1,

    /**
     * Znode has been created.
     *
     * This event is triggered for watches that were set using
     * ZooKeeper::exists(), but not for those that were set using
     * ZooKeeper::get() or ZooKeeper::getChildren().
     */
    ZnodeCreated = 1,

    /**
     * Znode has been removed.
     *
     * This event is triggered for watches that were set using any of
     * ZooKeeper::exists(), ZooKeeper::get(), or ZooKeeper::getChildren().
     */
    ZnodeRemoved = 2,

    /**
     * Znode data has changed.
     *
     * This event is triggered for watches that were set using
     * ZooKeeper::exists() or ZooKeeper::get(), but not for those that were
     * set using ZooKeeper::getChildren().
     */
    ZnodeDataChanged = 3,

    /**
     * A change has occurred in the list of children.
     *
     * This event is triggered for watches that were set using
     * ZooKeeper::getChildren(), but not for those that were set using
     * ZooKeeper::get() or ZooKeeper::exists().
     */
    ZnodeChildrenChanged = 4,
  };

  /**
   * Returns WatchEvent::type enum in human-readable string format.
   */
  const std::string toString(type eventType);
};

/**
 * Namespace for create mode enums.
 */
namespace CreateMode {
  /**
   * Create modes.
   *
   * In seuqential mode (either PersistentSequential of rEphemeralSequential),
   * ZooKeeper appends a monotonicly increasing counter to the end of path. This
   * counter is unique to the parent znode. The counter has a format of "%010d"
   * -- that is 10 digits with 0 (zero) padding (e.g. "<path>0000000001"). The
   * counter used to store the next sequence number is a signed int (4 bytes)
   * maintained by the parent znode, the counter will overflow when incremented
   * beyond 2147483647 (resulting in a name "<path>-2147483647").
   */
  enum type {
    /**
     * The znode will not be automatically deleted upon client's disconnect.
     */
    Persistent = 0,

    /**
     * The znode will not be automatically deleted upon client's disconnect,
     * and its name will be appended with a monotonically increasing number.
     */
    PersistentSequential = 2,

    /**
     * The znode will be deleted upon the client's disconnect.
     */
    Ephemeral = 1,

    /**
     * The znode will be deleted upon the client's disconnect, and its name
     * will be appended with a monotonically increasing number.
     */
    EphemeralSequential = 3,
  };

  const std::string toString(int32_t flags);
};

/**
 * Namespace for znode permission enum.
 */
namespace Permission {
  enum type {
    /** Permission to read data on a znode. */
    Read = 1 << 0,

    /** Permission to write data on a znode. */
    Write = 1 << 1,

    /** Permission to create children. */
    Create = 1 << 2,

    /** Permission to delete children. */
    Delete = 1 << 3,

    /** Permission to set permissions on a znode. */
    Admin = 1 << 4,
    All = Read | Write | Create | Delete | Admin,
  };

  const std::string toString(int32_t flags);
};

/**
 * Namespace for ZooKeeper op code enum.
 */
namespace OpCode {
  enum type {
    Notify = 0,
    Create = 1,
    Remove = 2,
    Exists = 3,
    GetData = 4,
    SetData = 5,
    GetAcl = 6,
    SetAcl = 7,
    GetChildren = 8,
    Sync= 9,
    Ping = 11,
    GetChildren2 = 12,
    Check = 13,
    Multi = 14,
    CreateSession = -10,
    CloseSession = -11,
    SetAuth = 100,
    SetWatches = 101,
    Error = -1,
  };

  const std::string toString(OpCode::type code);
};
}}}

#endif  // SRC_CONTRIB_ZKCPP_INCLUDE_ZOOKEEPER_CONST_H_
