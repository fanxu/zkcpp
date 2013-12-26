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
#ifndef SRC_CONTRIB_ZKCPP_SRC_WATCH_MANAGER_HH_
#define SRC_CONTRIB_ZKCPP_SRC_WATCH_MANAGER_HH_

#include <zookeeper/zookeeper.hh>
#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>
#include <list>
#include <string>

namespace org {
namespace apache {
namespace zookeeper {

class WatchManager {
  typedef boost::unordered_map<std::string,
                               std::list<boost::shared_ptr<Watch> > >
          watch_map;
  public:
    void getWatches(WatchEvent::type event,
                   SessionState::type state,
                   const std::string& path,
                   std::list<boost::shared_ptr<Watch> >& waches);
    void setDefaultWatch(boost::shared_ptr<Watch> watch);
    void addToExistsWatches(const std::string& path,
                            boost::shared_ptr<Watch> watch);
    void addToGetDataWatches(const std::string& path,
                             boost::shared_ptr<Watch> watch);
    void addToGetChildrenWatches(const std::string& path,
                                 boost::shared_ptr<Watch> watch);
    void getExistsPaths(std::vector<std::string>& paths);
    void getGetDataPaths(std::vector<std::string>& paths);
    void getGetChildrenPaths(std::vector<std::string>& paths);
  private:
    void moveWatches(watch_map& from, const std::string&path,
        std::list<boost::shared_ptr<Watch> >& to);
    void addWatch(watch_map& watches, const std::string& path,
                  boost::shared_ptr<Watch> watch);
    void getPaths(const watch_map& watches, std::vector<std::string>& paths);

    boost::shared_ptr<Watch> defaultWatch_;
    watch_map existsWatches_;
    watch_map getDataWatches_;
    watch_map getChildrenWatches_;
};

class WatchRegistration {
  public:
    virtual ~WatchRegistration() = 0;
    virtual bool activate(ReturnCode::type rc) = 0;

  protected:
    WatchRegistration(boost::shared_ptr<WatchManager> manager,
                      const std::string& path, boost::shared_ptr<Watch> watch);
    boost::shared_ptr<WatchManager> manager_;
    std::string path_;
    boost::shared_ptr<Watch> watch_;
};

class ExistsWatchRegistration : public WatchRegistration {
  public:
    ExistsWatchRegistration(boost::shared_ptr<WatchManager> manager,
        const std::string& path, boost::shared_ptr<Watch> watch);
    virtual bool activate(ReturnCode::type rc);
};

class GetDataWatchRegistration : public WatchRegistration {
  public:
    GetDataWatchRegistration(boost::shared_ptr<WatchManager> manager,
        const std::string& path, boost::shared_ptr<Watch> watch);
    virtual bool activate(ReturnCode::type rc);
};

class GetChildrenWatchRegistration : public WatchRegistration {
  public:
    GetChildrenWatchRegistration(boost::shared_ptr<WatchManager> manager,
        const std::string& path, boost::shared_ptr<Watch> watch);
    virtual bool activate(ReturnCode::type rc);
};

}}}  // namespace org::apache::zookeeper

#endif // SRC_CONTRIB_ZKCPP_SRC_WATCH_MANAGER_HH_
