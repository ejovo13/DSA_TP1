project(TP3_GRAPHS)
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

enable_testing(ON)

include_directories(include)
include_directories(ejovo)

set(BIN ${CMAKE_SOURCE_DIR}/bin) # Include test binaries
set(SRC ${CMAKE_SOURCE_DIR}/src)
set(INC ${CMAKE_SOURCE_DIR}/include)

file(MAKE_DIRECTORY ${BIN}) # Make binary directory if it doesnt already exist

add_subdirectory(src)
add_subdirectory(test)

## Add the informatique TP as a dependency...
add_subdirectory(${CMAKE_SOURCE_DIR}/../libejovo ${CMAKE_BINARY_DIR}/dependency)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

set(CMAKE_C_COMPILER gcc)
set(EJOVO_HEADERS ${CMAKE_SOURCE_DIR}/../Informatique-TP/inc)

add_executable(bst main.c)
target_link_libraries(bst tp3 ejovo)
target_include_directories(bst PUBLIC ${EJOVO_HEADERS})
message("Including EJOVO_HEADERS: ${EJOVO_HEADERS}")
# target_compile_options(g g)
add_compile_options(-g)