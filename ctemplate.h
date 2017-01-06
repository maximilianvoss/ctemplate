#ifndef CTEMPLATE_CTEMPLATE_H
#define CTEMPLATE_CTEMPLATE_H

#include <csafestring.h>

void ctemplate_init(char *templatePath, char *workingPath, char recompile);
char *ctemplate_executeTemplate(char *templateName, void *data);
void ctemplate_unload();
csafestring_t *ctemplate_getWorkingBaseDir();

#endif
