set (CMAKE_LEGACY_CYGWIN_WIN32 0) # Remove when CMake >= 2.8.4 is required
set (BUILD_SHARED_LIBS TRUE)
cmake_minimum_required (VERSION 2.6)

set (PARSER_NAME CSS)
project (lib${PARSER_NAME}Parser)

add_library( ${PARSER_NAME}Parser 
             ${PARSER_NAME}Lexer.cpp 
             ${PARSER_NAME}Parser.cpp)

include_directories(.
                    ${CMAKE_SOURCE_DIR}/parser
                    ${CMAKE_BINARY_DIR}/parser
                    ../../../../../nodistrib/antlr-3.5.2/include)
#set (CMAKE_C_COMPILER "/usr/bin/c99")
#set (CMAKE_C_FLAGS "-Wall -pedantic")

# Build the parser
add_custom_command(OUTPUT  ${PARSER_NAME}Lexer.cpp
                           ${PARSER_NAME}Parser.cpp
                           ${PARSER_NAME}.tokens
                           ${PARSER_NAME}Lexer.hpp
                           ${PARSER_NAME}Parser.hpp
                   COMMAND ${JAVA_EXE} ARGS -jar ${ANTLR_JAR} -fo . -verbose ../../parser/${PARSER_NAME}.g)
