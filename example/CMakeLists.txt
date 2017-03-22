cmake_minimum_required(VERSION 2.8)

set(SOURCE_FILES
        example.c
        ../utils/map.c
        ../utils/map.h)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DEXAMPLEPATH='\"${CMAKE_SOURCE_DIR}/example\"'")

if (NOT TARGET ctemplate_example)
    add_executable(ctemplate_example ${SOURCE_FILES})
    target_link_libraries(ctemplate_example ctemplate)
endif ()