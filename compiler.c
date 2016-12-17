#include "compiler.h"
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 4096

void compiler_compileCode(char *sourcePath, char *libraryPath) {
	char buffer[BUFFER_SIZE];

	strcpy(buffer, COMPILER);
	strcat(buffer, " ");
	strcat(buffer, COMPILER_FLAGS);
	strcat(buffer, " -o ");
	strcat(buffer, libraryPath);
	strcat(buffer, " ");
	strcat(buffer, sourcePath);

	system(buffer);
}
