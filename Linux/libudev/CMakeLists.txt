cmake_minimum_required (VERSION 2.6)
project (libudevSample)
add_executable(lus lus.c usblookup.c)
set (CMAKE_C_COMPILER "/usr/bin/c99")
set (CMAKE_C_FLAGS "-Wall -pedantic")
target_link_libraries (lus udev)

