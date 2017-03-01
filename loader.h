#ifndef CTEMPLATE_LOADER_H
#define CTEMPLATE_LOADER_H

#include <csafestring.h>
#include "functions.h"

#define MODULE_EXEC_MODULE "execute"

typedef struct s_loader_module {
	csafestring_t *path;
	void *handle;
	void (*method)(csafestring_t *, ctemplate_functions_t *, char *);

	struct s_loader_module *next;
	struct s_loader_module *prev;
} loader_module_t;

loader_module_t *loader_getModule(loader_module_t *modules, csafestring_t *modulePath);
loader_module_t *loader_loadModule(loader_module_t *modules, csafestring_t *modulePath);
loader_module_t *loader_unloadModule(loader_module_t *module);

#endif 
