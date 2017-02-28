#include <json2map.h>
#include "ctemplate.h"
#include "filemanager.h"
#include "date.h"
#include "translation.h"

static void ctemplate_parseJson(void (*set)(void *map, char *key, char *value), void *data, void *objects, char *json);
static char *ctemplate_executeModule(ctemplate_t *ctemplate, loader_module_t *module, char *json);
static char ctemplate_isRecompilationNecessary(char *templatePath, char *sourcePath, char *libraryPath);
static loader_module_t *ctemplate_moduleLoader(ctemplate_t *ctemplate, csafestring_t *templatePath, csafestring_t *sourcePath, csafestring_t *libraryPath);

ctemplate_t *ctemplate_init(char *templatePath, char *workingPath, ctemplate_functions_t *methods, bool recompile) {

	ctemplate_t *ctemplate = (ctemplate_t *) malloc(sizeof(ctemplate_t));


	ctemplate->templateBaseDir = safe_create(templatePath);
	if ( !safe_strcmp(ctemplate->templateBaseDir, "") ) {
		safe_strcpy(ctemplate->templateBaseDir, "./");
	} else if ( ctemplate->templateBaseDir->data[safe_strlen(ctemplate->templateBaseDir)] != '/' ) {
		safe_strchrappend(ctemplate->templateBaseDir, '/');
	}

	ctemplate->workingBaseDir = safe_create(workingPath);
	if ( !safe_strcmp(ctemplate->workingBaseDir, "") ) {
		safe_strcpy(ctemplate->workingBaseDir, "./");
	} else if ( ctemplate->workingBaseDir->data[safe_strlen(ctemplate->workingBaseDir)] != '/' ) {
		safe_strchrappend(ctemplate->workingBaseDir, '/');
	}

	ctemplate->mfunctions = methods;
	ctemplate->alwaysRecompile = recompile;
	ctemplate->modules = NULL;
	ctemplate->translation_modules = modules_init();
	ctemplate->mfunctions->parseJson = ctemplate_parseJson;

	return ctemplate;
}

char *ctemplate_executeTemplate(ctemplate_t *ctemplate, char *templateName, char *json) {

	if ( templateName == NULL ) {
		return NULL;
	}

	csafestring_t *templatePath = safe_clone(ctemplate->templateBaseDir);
	safe_strcat(templatePath, templateName);

	filemanager_fileinfo *templateInfo = filemanager_getStatus(templatePath->data);
	if ( filemanager_fileNotExists(templatePath->data) ) {
		safe_destroy(templatePath);
		free(templateInfo);
		return NULL;
	}
	free(templateInfo);

	char *templateFile = filemanager_getFilename(templateName);
	csafestring_t *sourcePath = filemanager_calculateSourcePath(ctemplate, templateFile);
	csafestring_t *libraryPath = filemanager_calculateCompilationPath(ctemplate, templateFile);

	loader_module_t *module = ctemplate_moduleLoader(ctemplate, templatePath, sourcePath, libraryPath);
	char *retVal = ctemplate_executeModule(ctemplate, module, json);

	safe_destroy(sourcePath);
	safe_destroy(libraryPath);
	safe_destroy(templatePath);

	return retVal;
}

static char *ctemplate_executeModule(ctemplate_t *ctemplate, loader_module_t *module, char *json) {
	csafestring_t *output = safe_create(NULL);
	module->method(output, ctemplate->mfunctions, json);

	char *retVal = (char *) malloc(sizeof(char) * output->buffer_length);
	memcpy (retVal, output->data, output->buffer_length);

	safe_destroy(output);
	return retVal;
}

static void ctemplate_parseJson(void (*set)(void *map, char *key, char *value), void *data, void *objects, char *json) {
	json2map_t *json2mapObj = json2map_init();
	json2map_registerDataHook(json2mapObj, data, set);
	json2map_registerObjectHook(json2mapObj, objects, set);
	json2map_parse(json2mapObj, NULL, json);
	json2map_destroy(json2mapObj);
}


static loader_module_t *ctemplate_moduleLoader(ctemplate_t *ctemplate, csafestring_t *templatePath, csafestring_t *sourcePath, csafestring_t *libraryPath) {
	loader_module_t *module = loader_getModule(ctemplate->modules, templatePath);
	if ( ctemplate->alwaysRecompile || ctemplate_isRecompilationNecessary(templatePath->data, sourcePath->data, libraryPath->data) ) {
		if ( module != NULL ) {
			ctemplate->modules = loader_unloadModule(module);
		}
		translation_processTemplate(ctemplate->translation_modules, templatePath->data, sourcePath->data, libraryPath->data);
		ctemplate->modules = loader_loadModule(ctemplate->modules, libraryPath);
		module = loader_getModule(ctemplate->modules, libraryPath);
	} else if ( module == NULL ) {
		ctemplate->modules = loader_loadModule(ctemplate->modules, libraryPath);
		module = loader_getModule(ctemplate->modules, libraryPath);
	}
	return module;
}

void ctemplate_destroy(ctemplate_t *ctemplate) {
	while ( ctemplate->modules != NULL ) {
		ctemplate->modules = loader_unloadModule(ctemplate->modules);
	}
	safe_destroy(ctemplate->templateBaseDir);
	safe_destroy(ctemplate->workingBaseDir);
	modules_destroy(ctemplate->translation_modules);
	free(ctemplate);
}

static char ctemplate_isRecompilationNecessary(char *templatePath, char *sourcePath, char *libraryPath) {
	if ( filemanager_fileNotExists(sourcePath) || filemanager_fileNotExists(libraryPath) ) {
		return 1;
	}

	filemanager_fileinfo *fileInfoTemplate = filemanager_getStatus(templatePath);
	filemanager_fileinfo *fileInfoSource = filemanager_getStatus(sourcePath);
	filemanager_fileinfo *fileInfoLibrary = filemanager_getStatus(libraryPath);

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

