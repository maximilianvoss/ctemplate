cmake_minimum_required(VERSION 2.8)

set(SOURCE_FILES
        cif.c
        cif.h
        modules.c
        modules.h
        expression_builder.c
        expression_builder.h
        expression.c
        expression.h cset.c cset.h cout.c cout.h cremove.c cremove.h cchoose.c cchoose.h cforeach.c cforeach.h)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")

if (NOT TARGET ctemplate_translation)
    add_library(ctemplate_translation ${SOURCE_FILES})
    target_link_libraries(ctemplate_translation ctemplate)
endif ()