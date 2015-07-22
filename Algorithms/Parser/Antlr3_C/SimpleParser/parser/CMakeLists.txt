set (CMAKE_LEGACY_CYGWIN_WIN32 0) # Remove when CMake >= 2.8.4 is required
cmake_minimum_required (VERSION 2.6)
project (libSimpleParser)

add_library(SimpleParser SimpleLexer.cpp SimpleParser.cpp)

include_directories(../../../../../nodistrib/antlr-3.5.2/include)
set (CMAKE_C_COMPILER "/usr/bin/c99")
set (CMAKE_C_FLAGS "-Wall -pedantic")

add_custom_command(OUTPUT  SimpleLexer.cpp SimpleParser.cpp  Simple.tokens  SimpleLexer.hpp  SimpleParser.hpp
                   COMMAND /cygdrive/c/Program\ Files/Java/jdk1.7.0_80/bin/java ARGS -jar C:\\\\Users\\\\afester\\\\Projects\\\\CodeSamples\\\\nodistrib\\\\antlr-3.5.2-complete-no-st3.jar -verbose ../../parser/Simple.g)
