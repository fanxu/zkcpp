cmake_minimum_required(VERSION 2.6)
project(zkcpp)
include(CheckTypeSize)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -gdwarf-3")

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/generated
  ${PROJECT_SOURCE_DIR}/src
  ${PROJECT_SOURCE_DIR}/tests
)

file(GLOB header
  ${PROJECT_SOURCE_DIR}/include/*h
  ${PROJECT_SOURCE_DIR}/generated/*.h
  ${PROJECT_SOURCE_DIR}/generated/*.hh
  ${PROJECT_SOURCE_DIR}/src/*h
)

file(GLOB source
  ${PROJECT_SOURCE_DIR}/src/*.cc
  ${PROJECT_SOURCE_DIR}/src/*.c
  ${PROJECT_SOURCE_DIR}/generated/*.c
  ${PROJECT_SOURCE_DIR}/generated/*.cc
)

file(GLOB testsrc
  ${PROJECT_SOURCE_DIR}/tests/*.cc
)

# Set parameters needed by config.h.in to generate config.h
set(PACKAGE_STRING "\"ZooKeeper C++ Client 3.5.0\"")

if (CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "-g -Wall")
endif()

# Check if ndfs_t type is defined in poll.h. If it is not defined, use
# unsigned int.
set(CMAKE_EXTRA_INCLUDE_FILES poll.h)
check_type_size("nfds_t" NFDS_T_SIZE)
set(CMAKE_EXTRA_INCLUDE_FILES)
if(NFDS_T_SIZE)
  set(POLL_NFDS_TYPE nfds_t)
else(NFDS_T_SIZE)
set(POLL_NFDS_TYPE "unsigned int")
endif(NFDS_T_SIZE)
configure_file(
  ${PROJECT_SOURCE_DIR}/config.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/config.h
)

# Look for log4cpp
find_path(LOG4CXX_INCLUDE_DIR log4cxx/log4cxx.h)
find_library(LOG4CXX_LIBRARY NAMES log4cxx)

if(LOG4CXX_INCLUDE_DIR AND LOG4CXX_LIBRARY)
  set(LOG4CXX_FOUND TRUE)
endif()

if(LOG4CXX_FOUND)
  message(STATUS "Found log4cxx: ${LOG4CXX_LIBRARY}")
else()
  message(FATAL_ERROR "Could not find log4cxx")
endif()


# Look for boost with thread
find_package(Boost COMPONENTS thread REQUIRED)
find_package(Boost COMPONENTS system REQUIRED)
find_package(Boost COMPONENTS filesystem REQUIRED)

# Look for googletest
find_path(GTEST_INCLUDE_DIR gtest/gtest.h)
find_library(GTEST_LIBRARY NAMES gtest)

if(GTEST_INCLUDE_DIR AND GTEST_LIBRARY)
  set(GTEST_FOUND TRUE)
endif()

if(GTEST_FOUND)
  message(STATUS "Found googletest: ${GTEST_LIBRARY}")
else()
  message(FATAL_ERROR "Could not find googletest")
endif()

# Treat C source code as C++ source code.
set_source_files_properties(${source} ${testsrc} PROPERTIES LANGUAGE CXX)

add_definitions(
  -DZKSERVER_CMD="${PROJECT_SOURCE_DIR}/tests/zkServer.sh"
)

include_directories(
  ${Boost_INCLUDE_DIRS}
  ${GTEST_INCLUDE_DIR}
)

add_library(zkcpp STATIC ${header} ${source})
target_link_libraries (zkcpp
  ${LOG4CXX_LIBRARY}
  ${Boost_THREAD_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
  ${Boost_FILESYSTEM_LIBRARY}
)


add_executable(zktest ${testsrc})
target_link_libraries(zktest
  zkcpp
  ${GTEST_LIBRARY}
)

# Examples
add_executable(zk_hello_world
  ${PROJECT_SOURCE_DIR}/examples/zk_hello_world.cc
)
target_link_libraries(zk_hello_world
  zkcpp
)


# Add "make lint" target
add_custom_target(lint ${PROJECT_SOURCE_DIR}/cpplint.py ${testsrc} ${header})

# Add "make check" target
add_custom_target(check ${CMAKE_CURRENT_BINARY_DIR}/zktest)

# Add "make valcheck" target
add_custom_target(valcheck valgrind --leak-check=full ${CMAKE_CURRENT_BINARY_DIR}/zktest)

file(COPY ${PROJECT_SOURCE_DIR}/tests/log4cxx.properties DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# For "make install" target
FILE(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/include/zookeeper/*")
INSTALL(FILES ${headers} DESTINATION include/zookeeper)
INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/generated/zookeeper.jute.hh
              ${CMAKE_CURRENT_SOURCE_DIR}/generated/recordio.hh
              ${CMAKE_CURRENT_SOURCE_DIR}/generated/archive.hh
              ${CMAKE_CURRENT_SOURCE_DIR}/generated/exception.hh
              DESTINATION include/zookeeper)
INSTALL(TARGETS zkcpp
  ARCHIVE DESTINATION lib
)
