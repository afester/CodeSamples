cmake_minimum_required (VERSION 2.6)
project (SQLiteSample)

add_executable(sqllitesample SQLiteSample.c)
target_link_libraries(sqllitesample sqlite3)
