cmake_minimum_required(VERSION 3.22)
project(test)

list(APPEND CMAKE_PREFIX_PATH "/tmp/install")

find_package(gflags)

add_executable(test test.cc)
target_link_libraries(test
    PRIVATE gflags
)