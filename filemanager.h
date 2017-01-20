#ifndef CTEMPLATE_FILEMANAGER_H
#define CTEMPLATE_FILEMANAGER_H

#include <csafestring.h>
#include "ctemplate.h"

typedef struct stat filemanager_fileinfo;
typedef struct tm filemanager_time;

csafestring_t *filemanager_calculateSourcePath(ctemplate_t *ctemplate, char *templateFile);
csafestring_t *filemanager_calculateCompilationPath(ctemplate_t *ctemplate, char *templateFile);
char *filemanager_getFilename(char *path);
filemanager_fileinfo *filemanager_getStatus(char *path);
filemanager_time* filemanager_getModifiedDate(filemanager_fileinfo *fileinfo);
int filemanager_fileNotExists(char *filename);

#endif
