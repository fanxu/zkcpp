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
#ifndef MEMORY_INSTREAM_HH_
#define MEMORY_INSTREAM_HH_

#include <recordio.hh>
#include <boost/noncopyable.hpp>
#include <algorithm>

namespace org { namespace apache { namespace zookeeper {

/**
 * A memory-backed hadoop::InStream.
 *
 * This class does not take the ownership of the buffer passed in the
 * constructor.
 */
class MemoryInStream : public hadoop::InStream, boost::noncopyable {
  public:
    MemoryInStream(const void* buf, size_t buflen) :
      buf_(buf), buflen_(buflen), offset_(0) {}
    virtual ~MemoryInStream() {}
    ssize_t read(void *buf, size_t buflen) {
      if (buf == NULL || buflen == 0) {
        return 0;
      }
      size_t numBytes = std::min(buflen, buflen_ - offset_);
      memmove(buf, (uint8_t*)buf_ + offset_, numBytes);
      offset_ += numBytes;
      return numBytes;
    }

  private:
    MemoryInStream() {}
    const void* buf_;
    size_t buflen_;
    size_t offset_;
};

}}}  // namespace org::apache::zookeeper

#endif  // MEMORY_INSTREAM_HH_
