set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN})

add_executable(tstack test_stack.c)
add_executable(tqueue test_queue.c)

target_link_libraries(tstack tp2)
target_link_libraries(tqueue tp2)