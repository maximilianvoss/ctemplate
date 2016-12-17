#ifndef CTEMPLATE_FILEMANAGER_H
#define CTEMPLATE_FILEMANAGER_H

typedef struct stat filemanager_fileinfo;
typedef struct tm filemanager_time;

char *filemanager_getSourcePath(char *templatePath);
char *filemanager_getCompilationPath(char *templatePath);
char *filemanager_getFilename(char *path);
filemanager_fileinfo *filemanager_getStatus(char *path);
filemanager_time* filemanager_getModifiedDate(filemanager_fileinfo *fileinfo);
char filemanager_fileNotExists(filemanager_fileinfo *fileinfo);

#endif
