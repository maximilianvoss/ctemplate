cmake_minimum_required(VERSION 2.8)

set(SOURCE_FILES
        map.c
        map.h)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")

add_library(utils ${SOURCE_FILES})
target_link_libraries(utils ctemplate)