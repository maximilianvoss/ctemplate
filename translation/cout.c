#include "cout.h"
#include "expression.h"

static char *cout_openTag(FILE *out, char *line);
static char *cout_closeTag(FILE *out, char *line);

static translation_module_t module_cout = {
		.tagOpen = "<c:out",
		.tagOpenLen = 6,
		.tagClose = "</c:out",
		.tagCloseLen = 7,
		.functionOpen = cout_openTag,
		.functionClose = cout_closeTag,
		.data = NULL,
		.next = NULL
};

void cout_register(translation_module_t *modules) {
	modules_register(modules, &module_cout);
}

void cout_unregister(translation_module_t *modules) {
	modules_unregister(modules, &module_cout);
}

static char *cout_openTag(FILE *out, char *line) {
	csafestring_t *value = modules_extractVariable(line, "value");
	csafestring_t *defaultValue = modules_extractVariable(line, "default");

	if ( value == NULL ) {
		safe_destroy(value);
		safe_destroy(defaultValue);
		return modules_findEndOfTag(line) + 1;
	}

	if ( !safe_strncmp(value, "${", 2) ) {
		fprintf(out, "__internal_tmp = ");
		expression_eval(out, value->data, true);
		fprintf(out, ";\n");
		fprintf(out, "__internal_hfunction->safe_strcat(__internal_string, (__internal_tmp != NULL) ? __internal_tmp : \"%s\");\n", ( defaultValue != NULL ) ? defaultValue->data : "");
	} else {
		fprintf(out, "__internal_hfunction->safe_strcat(__internal_string, \"%s\");\n", value->data);
	}

	safe_destroy(value);
	safe_destroy(defaultValue);
	return modules_findEndOfTag(line) + 1;
}

static char *cout_closeTag(FILE *out, char *line) {
	return modules_findEndOfTag(line) + 1;
}