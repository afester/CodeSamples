SET (CMAKE_LEGACY_CYGWIN_WIN32 0) # Remove when CMake >= 2.8.4 is required
cmake_minimum_required (VERSION 2.6)

project (profile)

if(${WIN32})
  add_library(profile Win32Profile.cpp)
else()
  add_library(profile UnixProfile.cpp)
endif()

set (CMAKE_C_COMPILER "/usr/bin/c99")
set (CMAKE_C_FLAGS "-Wall -pedantic")
