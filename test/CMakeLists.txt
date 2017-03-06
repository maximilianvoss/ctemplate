cmake_minimum_required(VERSION 2.8)

set(SOURCE_FILES
        testcases.c
        ../utils/map.c
        ../utils/map.h)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DTESTPATH='\"${CMAKE_SOURCE_DIR}/test\"'")

add_executable(ctemplate_testcases ${SOURCE_FILES})
target_link_libraries(ctemplate_testcases ctemplate)

add_test(NAME test_fileNotExists COMMAND ctemplate_testcases test_fileNotExists)
add_test(NAME test_cset COMMAND ctemplate_testcases test_cset)
add_test(NAME test_cout COMMAND ctemplate_testcases test_cout)
add_test(NAME test_cremove COMMAND ctemplate_testcases test_cremove)
add_test(NAME test_cif COMMAND ctemplate_testcases test_cif)
add_test(NAME test_expression COMMAND ctemplate_testcases test_expression)
add_test(NAME test_cchoose COMMAND ctemplate_testcases test_cchoose)
add_test(NAME test_cforeach COMMAND ctemplate_testcases test_cforeach)
add_test(NAME test_cforeachobject COMMAND ctemplate_testcases test_cforeachobject)