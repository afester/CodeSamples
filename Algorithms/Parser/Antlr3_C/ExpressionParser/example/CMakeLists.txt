set (CMAKE_LEGACY_CYGWIN_WIN32 0) # Remove when CMake >= 2.8.4 is required
cmake_minimum_required (VERSION 2.6)
project (ExpressionExample)

add_executable(ExpressionExample main.cpp)


find_library( PROFILE_LIBRARY NAMES profile
              PATHS ${CMAKE_HOME_DIRECTORY}/../../../../Tools/Profile/build/profile )

target_link_libraries(ExpressionExample ExpressionParser ${PROFILE_LIBRARY})
include_directories( ${CMAKE_SOURCE_DIR}/parser
                     ${CMAKE_BINARY_DIR}/parser
                     ${CMAKE_HOME_DIRECTORY}/../../../../nodistrib/antlr-3.5.2/include
                     ${CMAKE_HOME_DIRECTORY}/../../../../Tools/Profile/profile )
set (CMAKE_C_COMPILER "/usr/bin/c99")
set (CMAKE_C_FLAGS "-Wall -pedantic")
