#ifndef CTEMPLATE_COMPILER_H
#define CTEMPLATE_COMPILER_H

#define COMPILER "gcc"
#ifdef DEBUG
#define COMPILER_FLAGS "-shared -Wall -fPIC -O3 -I/usr/local/include -L/usr/local/lib -lcsafestring"
#else
#define COMPILER_FLAGS "-shared -fPIC -O3 -I/usr/local/include -L/usr/local/lib -lcsafestring"
#endif

#endif
