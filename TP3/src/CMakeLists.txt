add_library(tp3 ejovo_rand.c tree.c)

if (UNIX)
    target_link_libraries(tp3 PUBLIC m)
endif (UNIX)