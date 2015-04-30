cmake_minimum_required (VERSION 2.6)
project (libhello)

add_library(hello SHARED HelloWorld.cpp)
set_target_properties(hello PROPERTIES LIBRARY_OUTPUT_DIRECTORY lib)
message( STATUS "HOME_DIR: " ${CMAKE_HOME_DIRECTORY} )


