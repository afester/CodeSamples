set (CMAKE_LEGACY_CYGWIN_WIN32 0) # Remove when CMake >= 2.8.4 is required
cmake_minimum_required (VERSION 2.6)

include(CMakeConfig.txt)

add_subdirectory(parser)
add_subdirectory(example)
