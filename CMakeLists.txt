cmake_minimum_required(VERSION 3.22)
project(test)

execute_process(COMMAND ${CMAKE_COMMAND} -S ${CMAKE_SOURCE_DIR}/deps -B ${CMAKE_BINARY_DIR}/deps)
execute_process(COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}/deps)

list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR}/deps/install/gflags)
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR}/deps/install/glog)

find_package(gflags)
find_package(glog)

add_executable(test test.cc)
target_link_libraries(test
    PRIVATE gflags
    PRIVATE glog::glog
)