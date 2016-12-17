#include "ctemplate.h"
#include "filemanager.h"
#include "loader.h"
#include "date.h"
#include "translation.h"
#include <stdlib.h>

char ctemplate_recompilationNecessary(char *templatePath, char *sourcePath);

static loader_module_t *modules = NULL;

char *ctemplate_executeTemplate(char *templatePath, void *data) {
	
	char *templateName = filemanager_getFilename(templatePath);
	char *sourcePath = filemanager_getSourcePath(templatePath);
	char *libraryPath = filemanager_getCompilationPath(templatePath);

	loader_module_t* module = loader_getModule(modules, templateName);
	if ( ctemplate_recompilationNecessary(templatePath, sourcePath) ) {
		
		if ( module != NULL ) {
			modules = loader_unloadModule(module);
		}
		translation_processTemplate(templatePath, sourcePath, libraryPath);
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
}

char ctemplate_recompilationNecessary(char *templatePath, char *sourcePath) {
	
	filemanager_fileinfo *fileInfoTemplate = filemanager_getStatus(templatePath);
	filemanager_time *timeTemplate = filemanager_getModifiedDate(fileInfoTemplate);

	filemanager_fileinfo *fileInfoSource = filemanager_getStatus(sourcePath);
	filemanager_time *timeSource = filemanager_getModifiedDate(fileInfoSource);

	if ( filemanager_fileExists(fileInfoSource) ) {
		free(fileInfoTemplate);
		free(timeTemplate);
		free(fileInfoSource);
		free(timeSource);
		return 1;
	}
	
	if ( date_compare(timeTemplate, timeSource) < 0 ) {
		free(fileInfoTemplate);
		free(timeTemplate);
		free(fileInfoSource);
		free(timeSource);
		return 0;
	}
	free(fileInfoTemplate);
	free(timeTemplate);
	free(fileInfoSource);
	free(timeSource);
	return 1;
}