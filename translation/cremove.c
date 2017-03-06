#include <csafestring.h>
#include "cremove.h"

static char *cremove_openTag(FILE *out, char *line);
static char *cremove_closeTag(FILE *out, char *line);

static translation_module_t module_cremove = {
		.tagOpen = "<c:remove",
		.tagOpenLen = 9,
		.tagClose = "</c:remove",
		.tagCloseLen = 10,
		.functionOpen = cremove_openTag,
		.functionClose = cremove_closeTag,
		.data = NULL,
		.next = NULL
};

void cremove_register(translation_module_t *modules) {
	modules_register(modules, &module_cremove);
}

void cremove_unregister(translation_module_t *modules) {
	modules_unregister(modules, &module_cremove);
}

static char *cremove_openTag(FILE *out, char *line) {
	csafestring_t *var = modules_extractVariable(line, "var");

	if ( var == NULL ) {
		safe_destroy(var);
		return modules_findEndOfTag(line) + 1;
	}

	fprintf(out, "__internal_mfunction->unset(__internal_root, \"%s*\");\n", var->data);

	safe_destroy(var);
	return modules_findEndOfTag(line) + 1;
}

static char *cremove_closeTag(FILE *out, char *line) {
	return modules_findEndOfTag(line) + 1;
}