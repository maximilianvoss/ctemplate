cmake_minimum_required(VERSION 2.8)

set(SOURCE_FILES
        testcases.c)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DTESTPATH='\"${CMAKE_SOURCE_DIR}/test\"'")

add_executable(testcases ${SOURCE_FILES})
target_link_libraries(testcases ctemplate utils)

add_test(NAME test_fileNotExists COMMAND testcases test_fileNotExists)
add_test(NAME test_cset COMMAND testcases test_cset)
add_test(NAME test_cout COMMAND testcases test_cout)
add_test(NAME test_cremove COMMAND testcases test_cremove)
add_test(NAME test_cif COMMAND testcases test_cif)
add_test(NAME test_expression COMMAND testcases test_expression)