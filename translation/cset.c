#include "cset.h"
#include "expression.h"
#include "modules.h"

char *cset_openTag(char *line, FILE *out);
char *cset_closeTag(char *line, FILE *out);

translation_module_t module_cset = {
		.tagOpen = "<c:set",
		.tagOpenLen = 6,
		.tagClose = "</c:set",
		.tagCloseLen = 7,
		.functionOpen = cset_openTag,
		.functionClose = cset_closeTag,
		.data = NULL,
		.next = NULL
};

void cset_register() {
	modules_register(&module_cset);
}

void cset_unregister() {
	modules_unregister(&module_cset);
}

char *cset_openTag(char *line, FILE *out) {
	csafestring_t *value = modules_extractVariable(line, "value");
	csafestring_t *var = modules_extractVariable(line, "var");

	if ( value == NULL || var == NULL ) {
		safe_destroy(value);
		safe_destroy(var);
		return modules_findEndOfTag(line) + 1;
	}

	if ( !safe_strncmp(value, "${", 2) ) {
		fprintf(out, "mfunction->set(data, \"%s\", ", var->data);
		expression_eval(value->data, out, true);
		fprintf(out, ");\n");
	} else {
		fprintf(out, "mfunction->set(data, \"%s\", \"%s\");\n", var->data, value->data);
	}

	safe_destroy(value);
	safe_destroy(var);
	return modules_findEndOfTag(line) + 1;
}

char *cset_closeTag(char *line, FILE *out) {
	return modules_findEndOfTag(line) + 1;
}