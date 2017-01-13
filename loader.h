#ifndef CTEMPLATE_LOADER_H
#define CTEMPLATE_LOADER_H

#include <csafestring.h>
#include "ctemplate.h"

struct loader_module_s {
	csafestring_t *path;
	void *handle;
	void (*method)(csafestring_t *, ctemplate_functions_t *);

	struct loader_module_s *next;
	struct loader_module_s *prev;
};

typedef struct loader_module_s loader_module_t;

loader_module_t *loader_getModule(loader_module_t *modules, csafestring_t *modulePath);
loader_module_t *loader_loadModule(loader_module_t *modules, csafestring_t *modulePath);
loader_module_t *loader_unloadModule(loader_module_t *module);

#endif 
