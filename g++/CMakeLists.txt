cmake_minimum_required(VERSION 2.6)
project(liblog4z)

set(source  ../log4z.cpp )

set(LIB_SUFFIX "")
if (WIN32)
  if(CMAKE_GENERATOR MATCHES "Win64")
    if (NOT CMAKE_BUILD_TYPE STREQUAL "DEBUG")
      set(LIB_SUFFIX "_win64")
    else()
      set(LIB_SUFFIX "_win64d")
    endif()
  else()
    if (NOT CMAKE_BUILD_TYPE STREQUAL "DEBUG")
      set(LIB_SUFFIX "_win32")
    else()
      set(LIB_SUFFIX "_win32d")
    endif()
  endif()
else(WIN32)
  if (CMAKE_BUILD_TYPE STREQUAL "DEBUG")
    add_definitions(-Wall -g -ggdb -O0 -std=c++11)
    set(LIB_SUFFIX "_d")
  else()
    add_definitions(-Wall -O2 -DNODEBUG -std=c++11)
    set(LIB_SUFFIX "")
  endif()
endif(WIN32)



SET(LIBRARY_OUTPUT_PATH ${LIB_DIR})

add_library(log4z${LIB_SUFFIX} STATIC ${source})

install(TARGETS log4z${LIB_SUFFIX} 
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib)
install(FILES ../log4z.h ../COPYRIGHT DESTINATION include/log4z)

