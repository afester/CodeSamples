cmake_minimum_required (VERSION 2.6)

# Define the library which implements the memory monitor
project (HexDump)
add_library(HexDump src/hexdump_c.c)
