set (CMAKE_LEGACY_CYGWIN_WIN32 0) # Remove when CMake >= 2.8.4 is required
cmake_minimum_required (VERSION 2.6)
project (LibClangExample)

add_executable(lclang main.cpp)

find_library( CLANG_LIBRARY NAMES clang
            PATHS /usr/lib/llvm-3.5/lib )

target_link_libraries(lclang ${CLANG_LIBRARY})

include_directories(/usr/lib/llvm-3.5/include)

set (CMAKE_C_COMPILER "/usr/bin/c99")
set (CMAKE_C_FLAGS "-Wall -pedantic")
