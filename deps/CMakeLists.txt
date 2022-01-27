cmake_minimum_required(VERSION 3.22)
project(test)

include(ExternalProject)

ExternalProject_Add(gflags
    GIT_REPOSITORY https://github.com/gflags/gflags.git
    GIT_TAG v2.2.0

    CMAKE_ARGS
    -D CMAKE_INSTALL_PREFIX:PATH=${CMAKE_BINARY_DIR}/install/gflags
)

ExternalProject_Add(glog
    GIT_REPOSITORY https://github.com/google/glog.git
    GIT_TAG v0.4.0

    CMAKE_ARGS
    -D CMAKE_PREFIX_PATH:PATH=${CMAKE_BINARY_DIR}/install/gflags
    -D CMAKE_INSTALL_PREFIX:PATH=${CMAKE_BINARY_DIR}/install/glog

    DEPENDS gflags
)