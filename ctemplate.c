#include "ctemplate.h"
#include "filemanager.h"
#include "loader.h"
#include "date.h"
#include "translation.h"
#include <stdlib.h>
#include <string.h>

char ctemplate_recompilationNecessary(char *templatePath, char *sourcePath, char *libraryPath);

static loader_module_t *modules = NULL;
static char *templateBaseDir = NULL;
static char *workingBaseDir = NULL;

void ctemplate_init(char *templatePath, char *workingPath) {
	size_t length;
	
	if ( templatePath != NULL ) {
		length = strlen(templatePath);
		templateBaseDir = (char *) calloc(sizeof(char), length + 2);
		strcpy(templateBaseDir, templatePath);
		if ( templateBaseDir[length] != '/' ) {
			templateBaseDir[length + 1] = '/';
		}
	}

	if ( workingPath != NULL ) {
		length = strlen(workingPath);
		workingBaseDir = (char *) calloc(sizeof(char), length + 2);
		strcpy(workingBaseDir, workingPath);
		if ( workingBaseDir[length] != '/' ) {
			workingBaseDir[length + 1] = '/';
		}
	}
}

char *ctemplate_executeTemplate(char *templatePath, void *data) {
	
	if ( templatePath == NULL ) {
		return NULL;
	}
	
	size_t length = strlen(templateBaseDir) + strlen(templatePath) + 1;
	char *templateFullPath = (char *) calloc(sizeof(char), length);
	strcpy(templateFullPath, templateBaseDir);
	strcat(templateFullPath, templatePath);
	
	char *templateName = filemanager_getFilename(templatePath);
	char *sourcePath = filemanager_getSourcePath(templatePath);
	char *libraryPath = filemanager_getCompilationPath(templatePath);

	loader_module_t* module = loader_getModule(modules, templateName);
	if ( ctemplate_recompilationNecessary(templateFullPath, sourcePath, libraryPath) ) {
		
		if ( module != NULL ) {
			modules = loader_unloadModule(module);
		}
		translation_processTemplate(templateFullPath, sourcePath, libraryPath);
		modules = loader_loadModule(modules, libraryPath, templateName);
		module = loader_getModule(modules, templateName);
	} else if ( module == NULL ) {
		modules = loader_loadModule(modules, libraryPath, templateName);
		module = loader_getModule(modules, templateName);
	}

	
	module->method();
	modules = loader_unloadModule(module);
	
	return NULL;
}

void ctemplate_unload() {
	while ( modules != NULL ) {
		modules = loader_unloadModule(modules);
	}
	free(templateBaseDir);
	free(workingBaseDir);
}

char ctemplate_recompilationNecessary(char *templatePath, char *sourcePath, char *libraryPath) {
	
	filemanager_fileinfo *fileInfoTemplate = filemanager_getStatus(templatePath);
	filemanager_fileinfo *fileInfoSource = filemanager_getStatus(sourcePath);
	filemanager_fileinfo *fileInfoLibrary = filemanager_getStatus(libraryPath);

	if ( filemanager_fileNotExists(fileInfoSource) || filemanager_fileNotExists(fileInfoLibrary) ) {
		free(fileInfoTemplate);
		free(fileInfoSource);
		free(fileInfoLibrary);
		return 1;
	}

	filemanager_time *timeTemplate = filemanager_getModifiedDate(fileInfoTemplate);
	filemanager_time *timeSource = filemanager_getModifiedDate(fileInfoSource);
	filemanager_time *timeLibrary = filemanager_getModifiedDate(fileInfoLibrary);
	
	char result = (date_compare(timeTemplate, timeSource) < 0 ) || ( date_compare(timeTemplate, timeLibrary) < 0 );
	
	free(fileInfoTemplate);
	free(fileInfoSource);
	free(fileInfoLibrary);
	free(timeTemplate);
	free(timeSource);
	free(timeLibrary);
	
	return result;
}

char *ctemplate_getWorkingBaseDir() {
	return workingBaseDir;
}
