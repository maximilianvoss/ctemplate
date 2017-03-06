#include <csafestring.h>
#include "cset.h"
#include "expression.h"

static char *cset_openTag(FILE *out, char *line);
static char *cset_closeTag(FILE *out, char *line);

static translation_module_t module_cset = {
		.tagOpen = "<c:set",
		.tagOpenLen = 6,
		.tagClose = "</c:set",
		.tagCloseLen = 7,
		.functionOpen = cset_openTag,
		.functionClose = cset_closeTag,
		.data = NULL,
		.next = NULL
};

void cset_register(translation_module_t *modules) {
	modules_register(modules, &module_cset);
}

void cset_unregister(translation_module_t *modules) {
	modules_unregister(modules, &module_cset);
}

static char *cset_openTag(FILE *out, char *line) {
	csafestring_t *value = modules_extractVariable(line, "value");
	csafestring_t *var = modules_extractVariable(line, "var");

	if ( value == NULL || var == NULL ) {
		safe_destroy(value);
		safe_destroy(var);
		return modules_findEndOfTag(line) + 1;
	}

	if ( !safe_strncmp(value, "${", 2) ) {
		fprintf(out, "setVariable(__internal_mfunction, __internal_root, \"%s\",", var->data);
		expression_eval(out, value->data, true);
		fprintf(out, ");\n");
	} else {
		fprintf(out, "setVariable(__internal_mfunction, __internal_root, \"%s\", \"%s\");\n", var->data, value->data);
	}

	safe_destroy(value);
	safe_destroy(var);
	return modules_findEndOfTag(line) + 1;
}

static char *cset_closeTag(FILE *out, char *line) {
	return modules_findEndOfTag(line) + 1;
}