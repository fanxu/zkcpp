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
#include "path_utils.hh"
#include "zookeeper/logging.hh"
#include <boost/algorithm/string/predicate.hpp>
ENABLE_LOGGING;

namespace org {
namespace apache {
namespace zookeeper {

std::string PathUtils::
prependChroot(const std::string& path, const std::string& chroot) {
  if (chroot.empty()) {
    return path;
  } else if (path == "/") {
    return chroot;
  } else {
    return chroot + path;
  }
}


/**
 * Strips off the chroot string from the path.
 *
 * If the path does not start with the chroot, this method returns the path
 * unmodified.
 */
std::string PathUtils::
stripChroot(const std::string& path, const std::string& chroot) {
  //ZOOKEEPER-1027
  if (!boost::starts_with(path, chroot)) {
    LOG_ERROR(boost::format("server path %s does not include chroot path %s") %
              path % chroot);
    return path;
  }
  if (path == chroot) {
    return "/";
  }
  return path.substr(chroot.size());
}

}}}
