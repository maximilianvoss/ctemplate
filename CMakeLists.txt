cmake_minimum_required(VERSION 3.6)
project(ctemplate)

set(SOURCE_FILES
        ctemplate.c ctemplate.h test.c translation.c translation.h loader.c loader.h filemanager.c filemanager.h date.c date.h compiler.c compiler.h)

add_executable(ctemplate ${SOURCE_FILES})