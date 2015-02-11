cmake_minimum_required (VERSION 2.6)
project (libudevSample)
add_executable(lus lus.c)
target_link_libraries (lus udev)

