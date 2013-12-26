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
#ifndef STRING_OUT_STREAM_HH_
#define STRING_OUT_STREAM_HH_

#include <recordio.hh>
#include <boost/noncopyable.hpp>
#include <algorithm>
#include <string>

namespace org { namespace apache { namespace zookeeper {

/**
 * An std::string-backed hadoop::OutStream.
 */
class StringOutStream : public hadoop::OutStream, boost::noncopyable {
  public:
    explicit StringOutStream(std::string& str) : str_(str) {}

    ssize_t write(const void* buf, size_t buflen) {
      if (buf == NULL || buflen == 0) {
        return 0;
      }
      str_.append((const char*)buf, buflen);
      return buflen;
    }

  private:
    StringOutStream();
    std::string& str_;
};

}}}  // namespace org::apache::zookeeper

#endif  // STRING_OUT_STREAM_HH_
