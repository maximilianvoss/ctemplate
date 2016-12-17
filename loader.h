#ifndef CTEMPLATE_LOADER_H
#define CTEMPLATE_LOADER_H

struct loader_module_s {
	char *name;
	char *path;
	void *handle;
	void (*method)();

	struct loader_module_s *next;
	struct loader_module_s *prev;
};
typedef struct loader_module_s loader_module_t;

loader_module_t *loader_getModule(loader_module_t *modules, char *moduleName);
loader_module_t *loader_loadModule(loader_module_t *modules, char *modulePath, char *moduleName);
loader_module_t *loader_unloadModule(loader_module_t *module);

#endif 
