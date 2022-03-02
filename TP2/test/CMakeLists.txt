# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN})

# add_executable(tstack test_stack.c)
# add_executable(tqueue test_queue.c)

# target_link_libraries(tstack tp2)
# target_link_libraries(tqueue tp2)

list(APPEND TESTS
    queue
    stack
)

list(APPEND TEST_LIBS
    tp2
)

list(APPEND TEST_INC
    ${INC}
)

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN}/test)

# set(TEST_BIN_DIR "${CMAKE_CURRENT_BINARY_DIR}" PARENT_SCOPE)
macro(ejovo_add_test TESTNAME)

    # create an exectuable in which the tests will be stored
    set(TEST_BIN "test_${TESTNAME}")

    message(STATUS "Adding test ${TESTNAME}")
    add_executable(${TEST_BIN} "t${TESTNAME}.c")
    target_link_libraries(${TEST_BIN} ${TEST_LIBS})
    target_include_directories(${TEST_BIN} PUBLIC ${TEST_INC})
    target_compile_options(${TEST_BIN} PUBLIC -g)
    set_target_properties(${TEST_BIN}
        PROPERTIES
        # OUTPUT_NAME ${TESTNAME}
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
    add_test(${TESTNAME} test_${TESTNAME})

endmacro()

foreach(T ${TESTS})
    ejovo_add_test(${T})
endforeach(T)