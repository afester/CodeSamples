cmake_minimum_required (VERSION 2.6)

project (C64disc)
include_directories(../../c/hexdump/src)
add_executable(C64disc main.cpp)
target_link_libraries(C64disc ../../../c/hexdump/build/HexDump)
