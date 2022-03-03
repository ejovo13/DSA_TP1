add_library(tp2 graph.c list.c queue.c stack.c)

target_compile_options(tp2 PUBLIC --coverage)
target_link_options(tp2 PUBLIC --coverage)
target_link_libraries(tp2 gcov)


# add_custom_command()