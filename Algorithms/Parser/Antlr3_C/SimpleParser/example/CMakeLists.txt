set (CMAKE_LEGACY_CYGWIN_WIN32 0) # Remove when CMake >= 2.8.4 is required
cmake_minimum_required (VERSION 2.6)
project (SimpleExample)

add_executable(SimpleExample main.cpp)
target_link_libraries(SimpleExample SimpleParser)
include_directories( ${CMAKE_BINARY_DIR}/parser  ${CMAKE_HOME_DIRECTORY}/../../../../nodistrib/antlr-3.5.2/include )
set (CMAKE_C_COMPILER "/usr/bin/c99")
set (CMAKE_C_FLAGS "-Wall -pedantic")
