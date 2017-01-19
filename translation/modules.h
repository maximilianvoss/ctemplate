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
	char *(*functionOpen)(char *line, FILE *out);
	char *(*functionClose)(char *line, FILE *out);
	void *data;
	struct s_translation_module *next;
} translation_module_t;


csafestring_t *modules_extractVariable(char *line, char *name);
void modules_init();
void modules_destroy();
void *modules_matches(char *line);
char *modules_execute(void *method, char *line, FILE *out);
void modules_register(translation_module_t *module);
void modules_unregister(translation_module_t *module);
char *modules_findEndOfTag(char *line);
char *modules_findEndOfElement(char *line, char chr);

#endif
