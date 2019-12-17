cmake_minimum_required (VERSION 2.6)
project (Win32Simple)

add_executable(Win32Simple WIN32 test.cpp)
target_link_libraries (Win32Simple Ntdll.lib)
