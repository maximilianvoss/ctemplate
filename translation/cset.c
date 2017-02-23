#include <csafestring.h>
#include "cset.h"
#include "expression.h"
#include "variable_handler.h"

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

void cset_register(translation_module_t *modules) {
	modules_register(modules, &module_cset);
}

void cset_unregister(translation_module_t *modules) {
	modules_unregister(modules, &module_cset);
}

char *cset_openTag(char *line, FILE *out) {
	csafestring_t *value = modules_extractVariable(line, "value");
	csafestring_t *var = modules_extractVariable(line, "var");

	if ( value == NULL || var == NULL ) {
		safe_destroy(value);
		safe_destroy(var);
		return modules_findEndOfTag(line) + 1;
	}

	handler_variable_t *varhandler = varhandler_create(var->data);
	if ( !safe_strncmp(value, "${", 2) ) {
		fprintf(out, "__internal_mfunction->set(__internal_%sValues, \"%s\", ", varhandler->mapName->data, varhandler->variableName->data);
		expression_eval(value->data, out, true);
		fprintf(out, ");\n");
	} else {
		fprintf(out, "__internal_mfunction->set(__internal_%sValues, \"%s\", \"%s\");\n", varhandler->mapName->data, varhandler->variableName->data, value->data);
	}

	varhandler_destroy(varhandler);
	safe_destroy(value);
	safe_destroy(var);
	return modules_findEndOfTag(line) + 1;
}

char *cset_closeTag(char *line, FILE *out) {
	return modules_findEndOfTag(line) + 1;
}