#ifndef CTEMPLATE_CTEMPLATE_H
#define CTEMPLATE_CTEMPLATE_H

void ctemplate_init(char *templatePath, char *workingPath, char recompile);
char *ctemplate_executeTemplate(char *templatePath, void *data);
void ctemplate_unload();
char *ctemplate_getWorkingBaseDir();

#endif
