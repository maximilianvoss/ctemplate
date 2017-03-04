#ifndef CTEMPLATE_MODULES_H
#define CTEMPLATE_MODULES_H

#include <stdio.h>
#include <csafestring.h>
#include <stdbool.h>

typedef struct s_translation_module {
	char *tagOpen;
	size_t tagOpenLen;
	char *tagClose;
	size_t tagCloseLen;
	char *(*functionOpen)(FILE *out, char *line);
	char *(*functionClose)(FILE *out, char *line);
	void *data;
	struct s_translation_module *next;
} translation_module_t;


csafestring_t *modules_extractVariable(char *line, char *name);
translation_module_t *modules_init();
void modules_destroy(translation_module_t *modules);
void *modules_matches(translation_module_t *modules, char *line);
char *modules_execute(FILE *out, void *method, char *line);
void modules_register(translation_module_t *modules, translation_module_t *module);
void modules_unregister(translation_module_t *modules, translation_module_t *module);
char *modules_findEndOfTag(char *line);
char *modules_findEndOfElement(char *line, char chr);

#endif
