#include "cout.h"
#include "expression.h"

char *cout_openTag(char *line, FILE *out);
char *cout_closeTag(char *line, FILE *out);

translation_module_t module_cout = {
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

char *cout_openTag(char *line, FILE *out) {
	csafestring_t *value = modules_extractVariable(line, "value");
	csafestring_t *defaultValue = modules_extractVariable(line, "default");

	if ( value == NULL ) {
		safe_destroy(value);
		safe_destroy(defaultValue);
		return modules_findEndOfTag(line) + 1;
	}

	if ( !safe_strncmp(value, "${", 2) ) {
		fprintf(out, "tmp = ");
		expression_eval(value->data, out, true);
		fprintf(out, ";\n");
		fprintf(out, "safe_strcat(string, (tmp != NULL) ? tmp : \"%s\");\n", ( defaultValue != NULL ) ? defaultValue->data : "");
	} else {
		fprintf(out, "safe_strcat(string, \"%s\");\n", value->data);
	}

	safe_destroy(value);
	safe_destroy(defaultValue);
	return modules_findEndOfTag(line) + 1;
}

char *cout_closeTag(char *line, FILE *out) {
	return modules_findEndOfTag(line) + 1;
}