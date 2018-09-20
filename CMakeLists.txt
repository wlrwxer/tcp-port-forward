cmake_minimum_required (VERSION 2.8)
project (gate)
aux_source_directory(. DIR_SRCS)
add_executable(gate ${DIR_SRCS})


#include_directories("/home/baba/development/include-c++11")
SET(CMAKE_CXX_FLAGS "-I/usr/local/include -I/usr/local/include/include-c++11 -std=c++1y -DENABLE_TRACE -DENABLE_DEBUG -DENABLE_INFO -DENABLE_WARNING -DENABLE_ERROR -DENABLE_FATAL")


SET(CMAKE_CXX_FLAGS_DEBUG "-I/usr/local/include -I/usr/local/include/include-c++11 -std=c++1y -DENABLE_TRACE -DENABLE_DEBUG -DENABLE_INFO -DENABLE_WARNING -DENABLE_ERROR -DENABLE_FATAL -g -O0")
SET(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

SET(CMAKE_BUILD_TYPE Debug)
#SET(CMAKE_BUILD_TYPE Release)


SET(CMAKE_EXE_LINKER_FLAGS "-L/usr/lib64/ -L/usr/local/lib -lboost_system -lprotobuf -lpthread -lluabind -lboost_regex -llua")
TARGET_LINK_LIBRARIES(gate libluabind.a)
