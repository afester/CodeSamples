set (CMAKE_LEGACY_CYGWIN_WIN32 0) # Remove when CMake >= 2.8.4 is required
cmake_minimum_required (VERSION 2.6)
project (CSSExample)

add_executable(CSSExample main.cpp)

#find_library( PROFILE_LIBRARY NAMES profile
#              PATHS ${CMAKE_HOME_DIRECTORY}/../../../../Tools/Profile/build/profile )
#target_link_libraries(CSSExample CSSParser ${PROFILE_LIBRARY})

include_directories( ${CMAKE_SOURCE_DIR}/parser )

#set (CMAKE_C_COMPILER "/usr/bin/c99")
#set (CMAKE_C_FLAGS "-Wall -pedantic")
