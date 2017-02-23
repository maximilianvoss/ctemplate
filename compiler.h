#ifndef CTEMPLATE_COMPILER_H
#define CTEMPLATE_COMPILER_H

void compiler_compileCode(char *sourcePath, char *libraryPath);

#define COMPILER "gcc"
#ifdef DEBUG
#define COMPILER_FLAGS "-shared -Wall -fPIC -O3 -I/usr/local/include -L/usr/local/lib -lcsafestring"
#else
#define COMPILER_FLAGS "-shared -std=c99 -fPIC -O3 -I/usr/local/include -L/usr/local/lib -lcsafestring"
#endif

#endif
