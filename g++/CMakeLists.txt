cmake_minimum_required(VERSION 2.6)
project(log4z)

set(LIB_DIR ../libs)
set(LIB_NAME ${PROJECT_NAME})

set(source  ../log4z.cpp )

#set(CMAKE_BUILD_TYPE "DEBUG")
if (CMAKE_BUILD_TYPE STREQUAL "DEBUG")
  add_definitions(-Wall -g -ggdb -O0 -std=c++11)
  set(LIB_NAME ${PROJECT_NAME}_d)
else()
  add_definitions(-Wall -O2 -std=c++11 -DNODEBUG)
endif()

message("LIB_NAME=" ${LIB_NAME} ", LINKER FLAGS=" ${CMAKE_EXE_LINKER_FLAGS})

SET(LIBRARY_OUTPUT_PATH ${LIB_DIR})
add_library(${LIB_NAME} STATIC ${source})


