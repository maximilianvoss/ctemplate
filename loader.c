#include <dlfcn.h>
#include "loader.h"

loader_module_t *loader_getModule(loader_module_t *modules, csafestring_t *modulePath) {
	while ( modules != NULL ) {
		if ( !safe_strcmp(modulePath, modules->path->data) ) {
			return modules;
		}
		modules = modules->next;
	}
	return NULL;
}

loader_module_t *loader_loadModule(loader_module_t *modules, csafestring_t *modulePath) {

	loader_module_t *module;
	module = (loader_module_t *) malloc(sizeof(loader_module_t));

	module->path = safe_clone(modulePath);

	module->handle = dlopen(module->path->data, RTLD_LAZY);
	module->method = dlsym(module->handle, MODULE_EXEC_METHOD);

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

	safe_destroy(module->path);
	free(module);

	return tmp;
}