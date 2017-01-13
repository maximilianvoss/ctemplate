#ifndef CTEMPLATE_CTEMPLATE_H
#define CTEMPLATE_CTEMPLATE_H

#include <csafestring.h>

typedef struct {
	void *(*createMap)();
	void (*destroyMap)(void *map);
	char *(*get)(void *map, char *key);
	void (*put)(void *map, char *key, char *value);
	char *(*find)(void *map, char *pattern);
} ctemplate_functions_t;

void ctemplate_init(char *templatePath, char *workingPath, ctemplate_functions_t *methods, char recompile);
char *ctemplate_executeTemplate(char *templateName, char *json);
void ctemplate_unload();
csafestring_t *ctemplate_getWorkingBaseDir();

#endif
