#ifndef CTEMPLATE_CTEMPLATE_H
#define CTEMPLATE_CTEMPLATE_H

#include <csafestring.h>
#include <stdbool.h>
#include "functions.h"
#include "loader.h"
#include "translation/modules.h"

typedef struct {
	loader_module_t *modules;
	csafestring_t *templateBaseDir;
	csafestring_t *workingBaseDir;
	bool alwaysRecompile;
	ctemplate_functions_t *mfunctions;
	ctemplate_utilities_t hfunctions;
	translation_module_t *translation_modules;
} ctemplate_t;

ctemplate_t *ctemplate_init(char *templatePath, char *workingPath, ctemplate_functions_t *methods, bool recompile);
char *ctemplate_executeTemplate(ctemplate_t *ctemplate, char *templateName, char *json);
void ctemplate_destroy(ctemplate_t *ctemplate);

#endif
