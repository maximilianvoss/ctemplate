#include "filemanager.h"
#include "ctemplate.h"
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>

char *filemanager_getFilename(char *path);
char *filemanager_getSuffix(char *path);

char *filemanager_getSourcePath(char *templatePath) {
	char *filename = filemanager_getFilename(templatePath);

	char *sourcePath = (char *) calloc(sizeof(char), 4096);
	strcpy(sourcePath, ctemplate_getWorkingBaseDir());
	strcat(sourcePath, filename);

	char *suffix = filemanager_getSuffix(sourcePath);
	*suffix = 'c';
	*( suffix + 1 ) = '\0';

	return sourcePath;
}

char *filemanager_getCompilationPath(char *templatePath) {
	char *filename = filemanager_getFilename(templatePath);

	char *compilationPath = (char *) calloc(sizeof(char), 4096);
	strcpy(compilationPath, ctemplate_getWorkingBaseDir());
	strcat(compilationPath, filename);

	char *suffix = filemanager_getSuffix(compilationPath);
	*suffix = 's';
	*( suffix + 1 ) = 'o';
	*( suffix + 2 ) = '\0';

	return compilationPath;
}

char *filemanager_getFilename(char *path) {
	char *ptr;
	char *pos = NULL;

	if ( path == NULL ) {
		return NULL;
	}

	ptr = path;
	while ( *ptr != '\0' ) {
		if ( *ptr == '/' ) {
			pos = ptr + 1;
		}
		ptr++;
	}
	return pos;
}

char *filemanager_getSuffix(char *path) {
	char *ptr;
	char *pos = NULL;

	if ( path == NULL ) {
		return NULL;
	}

	ptr = path;
	while ( *ptr != '\0' ) {
		if ( *ptr == '.' ) {
			pos = ptr + 1;
		}
		ptr++;
	}
	return pos;
}

filemanager_fileinfo *filemanager_getStatus(char *path) {
	filemanager_fileinfo *fileinfo;

	fileinfo = (filemanager_fileinfo *) malloc(sizeof(filemanager_fileinfo));
	stat(path, fileinfo);

	return fileinfo;
}

filemanager_time *filemanager_getModifiedDate(filemanager_fileinfo *fileinfo) {
	filemanager_time *tmp;
	filemanager_time *time;

	time = (filemanager_time *) malloc(sizeof(filemanager_time));

	tmp = localtime(&fileinfo->st_mtime);
	memcpy(time, tmp, sizeof(filemanager_time));

	return time;
}

// check if file exists
char filemanager_fileNotExists(filemanager_fileinfo *fileinfo) {
	return fileinfo != NULL;
}