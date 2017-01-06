cmake_minimum_required(VERSION 3.6)
project(ctemplate)

link_directories(/usr/local/lib)
include_directories(/usr/local/include)

#set(CMAKE_VERBOSE_MAKEFILE on)

set(SOURCE_FILES
        ctemplate.c ctemplate.h test.c translation.c translation.h loader.c loader.h filemanager.c filemanager.h date.c date.h compiler.c compiler.h)

add_executable(ctemplate ${SOURCE_FILES})
target_link_libraries(ctemplate csafestring)