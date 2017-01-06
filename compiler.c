#include "compiler.h"
#include <csafestring.h>

void compiler_compileCode(char *sourcePath, char *libraryPath) {

	csafestring_t *string = safe_create(COMPILER);
	safe_strcat(string, " ");
	safe_strcat(string, COMPILER_FLAGS);
	safe_strcat(string, " -o ");
	safe_strcat(string, libraryPath);
	safe_strcat(string, " ");
	safe_strcat(string, sourcePath);

	system(string->data);

	safe_destroy(string);
}
