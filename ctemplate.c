#include "ctemplate.h"
#include "filemanager.h"
#include "loader.h"
#include "date.h"
#include "translation.h"

char ctemplate_isRecompilationNecessary(char *templatePath, char *sourcePath, char *libraryPath);

static loader_module_t *modules = NULL;
static csafestring_t *templateBaseDir;
static csafestring_t *workingBaseDir;
static char alwaysRecompile = 0;

void ctemplate_init(char *templatePath, char *workingPath, char recompile) {

	templateBaseDir = safe_create(templatePath);
	if ( !safe_strcmp(templateBaseDir, "") ) {
		safe_strcpy(templateBaseDir, "./");
	} else if ( templateBaseDir->data[safe_strlen(templateBaseDir)] != '/' ) {
		safe_strchrappend(templateBaseDir, '/');
	}

	workingBaseDir = safe_create(workingPath);
	if ( !safe_strcmp(workingBaseDir, "") ) {
		safe_strcpy(workingBaseDir, "./");
	} else if ( workingBaseDir->data[safe_strlen(workingBaseDir)] != '/' ) {
		safe_strchrappend(workingBaseDir, '/');
	}

	alwaysRecompile = recompile;
}

char *ctemplate_executeTemplate(char *templateName, void *data) {

	if ( templateName == NULL ) {
		return NULL;
	}

	csafestring_t *templatePath = safe_clone(templateBaseDir);
	safe_strcat(templatePath, templateName);

	filemanager_fileinfo *templateInfo = filemanager_getStatus(templatePath->data);
	if ( filemanager_fileNotExists(templateInfo) ) {
		free(templateInfo);
		return NULL;
	}
	free(templateInfo);

	char *templateFile = filemanager_getFilename(templateName);
	csafestring_t *sourcePath = filemanager_calculateSourcePath(templateFile);
	csafestring_t *libraryPath = filemanager_calculateCompilationPath(templateFile);

	loader_module_t *module = loader_getModule(modules, templatePath);
	if ( alwaysRecompile || ctemplate_isRecompilationNecessary(templatePath->data, sourcePath->data, libraryPath->data) ) {

		if ( module != NULL ) {
			modules = loader_unloadModule(module);
		}
		translation_processTemplate(templatePath->data, sourcePath->data, libraryPath->data);
		modules = loader_loadModule(modules, libraryPath);
		module = loader_getModule(modules, libraryPath);
	} else if ( module == NULL ) {
		modules = loader_loadModule(modules, libraryPath);
		module = loader_getModule(modules, libraryPath);
	}

	csafestring_t *output = safe_create(NULL);
	module->method(output);

	char *retVal = (char *) malloc(sizeof(char) * output->buffer_length);
	memcpy (retVal, output->data, output->buffer_length);
	safe_destroy(output);

	modules = loader_unloadModule(module);

	safe_destroy(sourcePath);
	safe_destroy(libraryPath);
	safe_destroy(templatePath);

	return retVal;
}

void ctemplate_unload() {
	while ( modules != NULL ) {
		modules = loader_unloadModule(modules);
	}
	safe_destroy(templateBaseDir);
	safe_destroy(workingBaseDir);
}

char ctemplate_isRecompilationNecessary(char *templatePath, char *sourcePath, char *libraryPath) {

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

	char result = ( date_compare(timeTemplate, timeSource) < 0 ) || ( date_compare(timeTemplate, timeLibrary) < 0 );

	free(fileInfoTemplate);
	free(fileInfoSource);
	free(fileInfoLibrary);
	free(timeTemplate);
	free(timeSource);
	free(timeLibrary);

	return result;
}

csafestring_t *ctemplate_getWorkingBaseDir() {
	return workingBaseDir;
}
