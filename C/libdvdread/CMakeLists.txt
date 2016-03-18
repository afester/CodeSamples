cmake_minimum_required (VERSION 2.6)
project (dvdstreamer)

add_executable(dvdstreamer DVDStream.cpp Socket.cpp)
# add_executable(dvdstreamer menus.c)
target_link_libraries(dvdstreamer dvdnav)
