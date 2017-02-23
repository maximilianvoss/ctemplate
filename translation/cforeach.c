#include "cforeach.h"
#include "variable_handler.h"

char *cforeach_openTag(char *line, FILE *out);
char *cforeach_closeTag(char *line, FILE *out);

translation_module_t module_cforeach = {
		.tagOpen = "<c:forEach",
		.tagOpenLen = 10,
		.tagClose = "</c:forEach",
		.tagCloseLen = 11,
		.functionOpen = cforeach_openTag,
		.functionClose = cforeach_closeTag,
		.data = NULL,
		.next = NULL
};

void cforeach_register(translation_module_t *modules) {
	modules_register(modules, &module_cforeach);
}

void cforeach_unregister(translation_module_t *modules) {
	modules_unregister(modules, &module_cforeach);
}

char *cforeach_openTag(char *line, FILE *out) {
	csafestring_t *var = modules_extractVariable(line, "var");
	csafestring_t *begin = modules_extractVariable(line, "begin");
	csafestring_t *end = modules_extractVariable(line, "end");
	csafestring_t *step = modules_extractVariable(line, "step");

	if ( var == NULL || begin == NULL || end == NULL ) {
		safe_destroy(var);
		safe_destroy(begin);
		safe_destroy(end);
		safe_destroy(step);
		return modules_findEndOfTag(line) + 1;
	}

	fprintf(out, "for ( int %s = %s; %s <= %s; ", var->data, begin->data, var->data, end->data);
	if ( step == NULL ) {
		fprintf(out, "%s++) {\n", var->data);
	} else {
		fprintf(out, "%s+=%s) {\n", var->data, step->data);
	}
	fprintf(out, "snprintf(__internal_expressionString, 255, \"%s\", %s);\n", "%d", var->data);
	fprintf(out, "__internal_mfunction->set(__internal_%sValues, \"%s\", __internal_expressionString);\n", VARIABLE_HANDLER_MAP_NOT_SET, var->data);

	safe_destroy(var);
	safe_destroy(begin);
	safe_destroy(end);
	safe_destroy(step);
	return modules_findEndOfTag(line) + 1;
}

char *cforeach_closeTag(char *line, FILE *out) {
	fprintf(out, "}\n");
	return modules_findEndOfTag(line) + 1;
}