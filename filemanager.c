#include "filemanager.h"
#include "ctemplate.h"
#include <sys/stat.h>
#include <time.h>

char *filemanager_getSuffix(char *path);

csafestring_t *filemanager_calculateSourcePath(char *templateFile) {
	csafestring_t *sourcePath = safe_clone(ctemplate_getWorkingBaseDir());
	safe_strcat(sourcePath, templateFile);

	char *suffix = filemanager_getSuffix(sourcePath->data);
	*suffix = 'c';
	*( suffix + 1 ) = '\0';

	return sourcePath;
}

csafestring_t *filemanager_calculateCompilationPath(char *templateFile) {
	csafestring_t *compilationPath = safe_clone(ctemplate_getWorkingBaseDir());
	safe_strcat(compilationPath, templateFile);

	char *suffix = filemanager_getSuffix(compilationPath->data);
	*suffix = 's';
	*( suffix + 1 ) = 'o';
	*( suffix + 2 ) = '\0';

	return compilationPath;
}

char *filemanager_getFilename(char *path) {
	if ( path == NULL ) {
		return NULL;
	}
	return ( strrchr(path, '/') + 1 );
}

char *filemanager_getSuffix(char *path) {
	if ( path == NULL ) {
		return NULL;
	}
	return ( strrchr(path, '.') + 1 );
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