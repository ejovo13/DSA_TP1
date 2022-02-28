project(TP2_GRAPHS)
cmake_minimum_required(VERSION 3.10)

include_directories(include)

add_subdirectory(src)

add_executable(g main.c)
target_link_libraries(g tp2)