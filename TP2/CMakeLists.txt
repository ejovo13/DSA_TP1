project(TP2_GRAPHS)
cmake_minimum_required(VERSION 3.10)

include_directories(include)

set(BIN ${CMAKE_SOURCE_DIR}/bin) # Include test binaries
set(SRC ${CMAKE_SOURCE_DIR}/src)
set(INC ${CMAKE_SOURCE_DIR}/include)

file(MAKE_DIRECTORY ${BIN}) # Make binary directory if it doesnt already exist

add_subdirectory(src)
add_subdirectory(test)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY CMAKE_BINARY_DIR)

add_executable(g main.c)
target_link_libraries(g tp2)