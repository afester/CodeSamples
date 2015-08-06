set (CMAKE_LEGACY_CYGWIN_WIN32 0) # Remove when CMake >= 2.8.4 is required
cmake_minimum_required (VERSION 2.6)
project (ProfileExample)

add_executable(ProfileExample TestProfile.cpp)
target_link_libraries(ProfileExample profile)
include_directories( ${CMAKE_SOURCE_DIR}/profile )
set (CMAKE_C_COMPILER "/usr/bin/c99")
set (CMAKE_C_FLAGS "-Wall -pedantic")
