# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN})

# add_executable(tstack test_stack.c)
# add_executable(tqueue test_queue.c)

# target_link_libraries(tstack tp2)
# target_link_libraries(tqueue tp2)

list(APPEND TESTS
    queue
    stack
    graph
)

list(APPEND TEST_LIBS
    tp2
    gcov
)

list(APPEND TEST_INC
    ${INC}
)

find_library(REQUIRED NAMES gcov )

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN}/test)

# set(TEST_BIN_DIR "${CMAKE_CURRENT_BINARY_DIR}" PARENT_SCOPE)
macro(ejovo_add_test TESTNAME)

    # create an exectuable in which the tests will be stored
    set(TEST_BIN "test_${TESTNAME}")

    message(STATUS "Adding test ${TESTNAME}")
    add_executable(${TEST_BIN} "t${TESTNAME}.c")
    target_link_libraries(${TEST_BIN} ${TEST_LIBS})
    target_include_directories(${TEST_BIN} PUBLIC ${TEST_INC})
    target_compile_options(${TEST_BIN} PUBLIC -g --coverage)
    target_link_options(${TEST_BIN} PUBLIC --coverage)
    # target_compile_options(${TEST_BIN} PUBLIC -g -fprofile-generate -fprofile-arcs -ftest-coverage -L /usr/lib -lgcov --coverage)
    set_target_properties(${TEST_BIN}
        PROPERTIES
        # OUTPUT_NAME ${TESTNAME}
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
    add_test(${TESTNAME} test_${TESTNAME})

endmacro()

# Now for each test, generate an lcov report?

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/gcoverage)


foreach(T ${TESTS})
    ejovo_add_test(${T})
endforeach(T)


# Custom targets to try and work with gcov
# add_custom_
set(OBJECT_DIR ${CMAKE_BINARY_DIR}/src/CMakeFiles/tp2.dir)

add_custom_target(init
    COMMAND ${CMAKE_MAKE_PROGRAM} clean
    COMMAND rm -f ${OBJECT_DIR}/*.gcno
    COMMAND rm -f ${OBJECT_DIR}/*.gcda
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_custom_target(gcov)
add_custom_target(lcov)

add_custom_command(TARGET gcov
    POST_BUILD
    COMMAND echo "=================== GCOV ===================="
    COMMAND echo "Object dir: ${OBJECT_DIR}"
    COMMAND gcov -b ${CMAKE_SOURCE_DIR}/src/*.c -o ${OBJECT_DIR}
    COMMAND echo "-- Source diretorie: ${CMAKE_SOURCE_DIR}/src/"
    COMMAND echo "-- Coverage files have been output to ${CMAKE_BINARY_DIR}/gcoverage"
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/gcoverage
)

# add_custom_command(TARGET lcov
#     COMMAND echo "=================== LCOV ===================="
#     COMMAND echo "-- Passing lcov tool under code coverage"
#     COMMAND lcov --capture --directory ../ --output-file lcoverage/main_coverage.info
#     COMMAND echo "-- Generating HTML output files"
#     COMMAND genhtml lcoverage/main_coverage.info --output-directory lcoverage
# )