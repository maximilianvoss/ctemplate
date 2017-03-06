#ifndef CTEMPLATE_COMPILER_H
#define CTEMPLATE_COMPILER_H

void compiler_compileCode(char *sourcePath, char *libraryPath);

#define COMPILER "gcc"
#ifdef DEBUG
#define COMPILER_FLAGS "-shared -Wall -fPIC -O0 -g "
#else
#define COMPILER_FLAGS "-shared -std=c99 -fPIC -O3 "
#endif

#endif
