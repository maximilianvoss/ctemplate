cmake_minimum_required(VERSION 2.8)

set(SOURCE_FILES
        example.c)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DEXAMPLEPATH='\"${CMAKE_SOURCE_DIR}/example\"'")

add_executable(example ${SOURCE_FILES})
target_link_libraries(example ctemplate)
