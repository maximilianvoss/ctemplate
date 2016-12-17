#include <dlfcn.h>
#include "loader.h"
#include <string.h>
#include <stdlib.h>

loader_module_t *loader_getModule(loader_module_t *modules, char *moduleName) {
	while ( modules != NULL ) {
		if ( !strcmp(moduleName, modules->name) ) {
			return modules;
		}
		modules = modules->next;
	}
	return NULL;
}

loader_module_t *loader_loadModule(loader_module_t *modules, char *modulePath, char *moduleName) {

	loader_module_t *module;
	module = (loader_module_t *) malloc(sizeof(loader_module_t));

	module->name = (char *) calloc(sizeof(char), strlen(moduleName) + 1);
	strcpy(module->name, moduleName);

	module->path = (char *) calloc(sizeof(char), strlen(modulePath) + 1);
	strcpy(module->path, modulePath);

	module->handle = dlopen(module->path, RTLD_LAZY);
	module->method = dlsym(module->handle, "execute");

	if ( modules == NULL ) {
		module->next = NULL;
		module->prev = NULL;
	} else {
		module->prev = NULL;
		module->next = modules;

		modules->prev = module;
	}
	return module;
}

loader_module_t *loader_unloadModule(loader_module_t *module) {

	loader_module_t *tmp;

	if ( module->prev != NULL ) {
		module->prev->next = module->next;
	}
	if ( module->next != NULL ) {
		module->next->prev = module->prev;
	}

	if ( module->prev == NULL ) {
		tmp = module->next;
	} else {
		tmp = module->prev;
	}

	dlclose(module->handle);

	free(module->path);
	free(module->name);
	free(module);

	return tmp;
}