set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN})

add_executable(tstack test_stack.c)
target_link_libraries(tstack tp2)