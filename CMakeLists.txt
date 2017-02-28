cmake_minimum_required(VERSION 2.8)

add_subdirectory(libs/json2map)
include_directories(libs/json2map)
include_directories(libs/json2map/libs/csafestring)

project(ctemplate)

set(SOURCE_FILES
        ctemplate.c ctemplate.h translation.c translation.h loader.c loader.h filemanager.c filemanager.h date.c date.h compiler.c compiler.h functions.h)

#set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O3")

add_subdirectory(translation)

add_library(ctemplate ${SOURCE_FILES})
target_link_libraries(ctemplate json2map csafestring dl ctemplate_translation)
install(TARGETS ctemplate DESTINATION /usr/local/lib)
install(FILES ctemplate.h DESTINATION /usr/local/include)

enable_testing()
add_subdirectory(test)
add_subdirectory(example)