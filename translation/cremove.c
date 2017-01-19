#include "cremove.h"
#include "expression.h"
#include "modules.h"

char *cremove_openTag(char *line, FILE *out);
char *cremove_closeTag(char *line, FILE *out);

translation_module_t module_cremove = {
		.tagOpen = "<c:remove",
		.tagOpenLen = 9,
		.tagClose = "</c:remove",
		.tagCloseLen = 10,
		.functionOpen = cremove_openTag,
		.functionClose = cremove_closeTag,
		.data = NULL,
		.next = NULL
};

void cremove_register() {
	modules_register(&module_cremove);
}

void cremove_unregister() {
	modules_unregister(&module_cremove);
}

char *cremove_openTag(char *line, FILE *out) {
	csafestring_t *var = modules_extractVariable(line, "var");

	if ( var == NULL ) {
		safe_destroy(var);
		return modules_findEndOfTag(line) + 1;
	}

	fprintf(out, "mfunction->unset(data, \"%s\");\n", var->data);

	safe_destroy(var);
	return modules_findEndOfTag(line) + 1;
}

char *cremove_closeTag(char *line, FILE *out) {
	return modules_findEndOfTag(line) + 1;
}