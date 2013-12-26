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
#include "watch_manager.hh"
#include <boost/foreach.hpp>
#include <zookeeper/logging.hh>
ENABLE_LOGGING;

namespace org {
namespace apache {

/** ZooKeeper namespace. */
namespace zookeeper {

void WatchManager::
moveWatches(watch_map& from, const std::string&path,
            std::list<boost::shared_ptr<Watch> >& to) {
  watch_map::iterator itr = from.find(path);
  if (itr != from.end()) {
    to.insert(to.end(), itr->second.begin(), itr->second.end());
    from.erase(itr);
  }
}

void WatchManager::
getWatches(WatchEvent::type event,
    SessionState::type state,
    const std::string& path,
    std::list<boost::shared_ptr<Watch> >& watches) {
  watches.clear();

  switch (event) {
    case WatchEvent::SessionStateChanged:
      if (defaultWatch_.get() != NULL) {
        watches.push_back(defaultWatch_);
      }
      BOOST_FOREACH(const watch_map::value_type& pair, existsWatches_) {
        watches.insert(watches.end(), pair.second.begin(), pair.second.end());
      }
      BOOST_FOREACH(const watch_map::value_type& pair, getDataWatches_) {
        watches.insert(watches.end(), pair.second.begin(), pair.second.end());
      }
      BOOST_FOREACH(const watch_map::value_type& pair, getChildrenWatches_) {
        watches.insert(watches.end(), pair.second.begin(), pair.second.end());
      }
      break;
    case WatchEvent::ZnodeCreated:
      moveWatches(existsWatches_, path, watches);
    case WatchEvent::ZnodeDataChanged:
      moveWatches(getDataWatches_, path, watches);
    case WatchEvent::ZnodeChildrenChanged:
      moveWatches(getChildrenWatches_, path, watches);
      break;
    case WatchEvent::ZnodeRemoved:
      moveWatches(getDataWatches_, path, watches);
      moveWatches(getChildrenWatches_, path, watches);
      break;
  }
  LOG_DEBUG(boost::format("Got %d watch(es): event=%s, state=%s, path=%s") %
            watches.size() % WatchEvent::toString(event) %
            SessionState::toString(state) % path);
}

void WatchManager::
setDefaultWatch(boost::shared_ptr<Watch> watch) {
  defaultWatch_ = watch;
}

void WatchManager::
addWatch(watch_map& watches, const std::string& path,
         boost::shared_ptr<Watch> watch) {
  watch_map::iterator itr;
  itr = watches.find(path);
  if (itr == watches.end()) {
    std::list<boost::shared_ptr<Watch> > watchList;
    watchList.push_back(watch);
    watches[path] = watchList;
  } else {
    itr->second.push_back(watch);
  }
}

void WatchManager::
addToExistsWatches(const std::string& path,
    boost::shared_ptr<Watch> watch) {
  addWatch(existsWatches_, path, watch);
}

void WatchManager::
addToGetDataWatches(const std::string& path,
    boost::shared_ptr<Watch> watch) {
  addWatch(getDataWatches_, path, watch);
}

void WatchManager::
addToGetChildrenWatches(const std::string& path,
    boost::shared_ptr<Watch> watch) {
  addWatch(getChildrenWatches_, path, watch);
}

void WatchManager::
getPaths(const watch_map& watches, std::vector<std::string>& paths) {
  paths.clear();
  BOOST_FOREACH(const watch_map::value_type& pair, watches) {
    paths.push_back(pair.first);
  }
}

void WatchManager::
getExistsPaths(std::vector<std::string>& paths) {
  getPaths(existsWatches_, paths);
}

void WatchManager::
getGetDataPaths(std::vector<std::string>& paths) {
  getPaths(getDataWatches_, paths);
}

void WatchManager::
getGetChildrenPaths(std::vector<std::string>& paths) {
  getPaths(getChildrenWatches_, paths);
}

WatchRegistration::
WatchRegistration(boost::shared_ptr<WatchManager> manager,
                  const std::string& path,
                  boost::shared_ptr<Watch> watch) :
  manager_(manager), path_(path), watch_(watch) {
}


WatchRegistration::
~WatchRegistration() {
}

ExistsWatchRegistration::
ExistsWatchRegistration(boost::shared_ptr<WatchManager> manager,
        const std::string& path, boost::shared_ptr<Watch> watch) :
  WatchRegistration(manager, path, watch) {
}

bool ExistsWatchRegistration::
activate(ReturnCode::type rc) {
  if (rc == ReturnCode::Ok) {
    manager_->addToGetDataWatches(path_, watch_);
    return true;
  } else if (rc == ReturnCode::NoNode) {
    manager_->addToExistsWatches(path_, watch_);
    return true;
  } else {
    return false;
  }
}

GetDataWatchRegistration::
GetDataWatchRegistration(boost::shared_ptr<WatchManager> manager,
        const std::string& path, boost::shared_ptr<Watch> watch) :
  WatchRegistration(manager, path, watch) {
}

bool GetDataWatchRegistration::
activate(ReturnCode::type rc) {
  if (rc == ReturnCode::Ok) {
    manager_->addToGetDataWatches(path_, watch_);
    return true;
  } else {
    return false;
  }
}

GetChildrenWatchRegistration::
GetChildrenWatchRegistration(boost::shared_ptr<WatchManager> manager,
        const std::string& path, boost::shared_ptr<Watch> watch) :
  WatchRegistration(manager, path, watch) {
}

bool GetChildrenWatchRegistration::
activate(ReturnCode::type rc) {
  if (rc == ReturnCode::Ok) {
    manager_->addToGetChildrenWatches(path_, watch_);
    return true;
  } else {
    return false;
  }
}

}}}  // namespace org::apache::zookeeper
