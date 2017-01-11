cmake_minimum_required(VERSION 2.8)
project(ctemplate)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH})
include("${CMAKE_SOURCE_DIR}/cmake/include.cmake")

link_directories(/usr/local/lib)
include_directories(/usr/local/include)

set(SOURCE_FILES
        ctemplate.c ctemplate.h translation.c translation.h loader.c loader.h filemanager.c filemanager.h date.c date.h compiler.c compiler.h)

#set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O3")

add_library(ctemplate ${SOURCE_FILES})
target_link_libraries(ctemplate ${CSAFESTRING_LIBRARY} dl)
install(TARGETS ctemplate DESTINATION /usr/local/lib)
install(FILES ctemplate.h DESTINATION /usr/local/include)

enable_testing()
add_subdirectory(test)
add_subdirectory(example)