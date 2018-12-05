cmake_minimum_required (VERSION 2.6)

# Define the library which implements the memory monitor
project (MemoryMonitor)
add_library(MemoryMonitor monitor.c)

# Define a test application which uses the memory monitor library
project (MemoryMonitorTest)
add_executable(MemoryMonitorTest testMonitor.c)
target_link_libraries(MemoryMonitorTest MemoryMonitor)
