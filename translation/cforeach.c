#include <libs/csafestring/csafestring.h>
#include "cforeach.h"

static char *cforeach_openTag(FILE *out, char *line);
static char *cforeach_closeTag(FILE *out, char *line);
static void cforeach_counterLoop(FILE *out, csafestring_t *var, csafestring_t *begin, csafestring_t *end, csafestring_t *step);
static void cforeach_elementLoop(FILE *out, csafestring_t *var, csafestring_t *statusVar, csafestring_t *items);

static translation_module_t module_cforeach = {
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

static char *cforeach_openTag(FILE *out, char *line) {
	csafestring_t *var = modules_extractVariable(line, "var");
	csafestring_t *statusVar = modules_extractVariable(line, "statusVar");
	csafestring_t *begin = modules_extractVariable(line, "begin");
	csafestring_t *end = modules_extractVariable(line, "end");
	csafestring_t *step = modules_extractVariable(line, "step");
	csafestring_t *items = modules_extractVariable(line, "items");

	if ( var != NULL && items != NULL && statusVar != NULL ) {
		cforeach_elementLoop(out, var, statusVar, items);
	} else if ( var != NULL && begin != NULL && end != NULL ) {
		cforeach_counterLoop(out, var, begin, end, step);
	}

	safe_destroy(var);
	safe_destroy(statusVar);
	safe_destroy(begin);
	safe_destroy(end);
	safe_destroy(step);
	safe_destroy(items);
	return modules_findEndOfTag(line) + 1;
}

static void cforeach_counterLoop(FILE *out, csafestring_t *var, csafestring_t *begin, csafestring_t *end, csafestring_t *step) {
	fprintf(out, "for ( ");
	fprintf(out, "setVariable(__internal_mfunction, __internal_root, \"%s\", __internal_hfunction->intToString(__internal_expressionString, EXPR_STRING_LENGTH, %s));\n", var->data, begin->data);

	fprintf(out, "atoi(");
	fprintf(out, "getVariable(__internal_mfunction, __internal_root, \"%s\")", var->data);
	fprintf(out, ") <= %s;", end->data);

	fprintf(out, "setVariable(__internal_mfunction, __internal_root, \"%s\", ", var->data);
	fprintf(out, "__internal_hfunction->intToString(__internal_expressionString, EXPR_STRING_LENGTH,");

	if ( step == NULL ) {
		fprintf(out, "atoi(");
		fprintf(out, "getVariable(__internal_mfunction, __internal_root, \"%s\")", var->data);
		fprintf(out, ") + 1");
	} else {
		fprintf(out, "atoi(");
		fprintf(out, "getVariable(__internal_mfunction, __internal_root, \"%s\")", var->data);
		fprintf(out, ") + %s", step->data);
	}
	fprintf(out, ")");
	fprintf(out, ")");
	fprintf(out, ") {\n");
}

static void cforeach_elementLoop(FILE *out, csafestring_t *var, csafestring_t *statusVar, csafestring_t *items) {
	fprintf(out, "for ( ");
	fprintf(out, "setVariable(__internal_mfunction, __internal_root, \"%s\", __internal_hfunction->intToString(__internal_expressionString, EXPR_STRING_LENGTH, 0));\n", statusVar->data);

	fprintf(out, "atoi(");
	fprintf(out, "getVariable(__internal_mfunction, __internal_root, \"%s\")", statusVar->data);
	fprintf(out, ") < atoi(");
	fprintf(out, "getVariable(__internal_mfunction, __internal_root, \"%s\")", items->data);
	fprintf(out, ");");

	fprintf(out, "setVariable(__internal_mfunction, __internal_root, \"%s\", ", statusVar->data);
	fprintf(out, "__internal_hfunction->intToString(__internal_expressionString, EXPR_STRING_LENGTH,");

	fprintf(out, "atoi(");
	fprintf(out, "getVariable(__internal_mfunction, __internal_root, \"%s\")", statusVar->data);
	fprintf(out, ") + 1");

	fprintf(out, ")");
	fprintf(out, ")");
	fprintf(out, ") {\n");

	fprintf(out, "strncpy(__internal_expressionString, \"%s\", 254);", items->data);
	fprintf(out, "strncat(__internal_expressionString, \"[\", 254);\n");
	fprintf(out, "strncat(__internal_expressionString, getVariable(__internal_mfunction, __internal_root, \"%s\"), 254);\n", statusVar->data);
	fprintf(out, "strncat(__internal_expressionString, \"]\", 254);\n");

	fprintf(out, "__internal_tmp = getVariable(__internal_mfunction, __internal_root, __internal_expressionString);\n");
	fprintf(out, "setVariable(__internal_mfunction, __internal_root, \"%s\", __internal_tmp);\n", var->data);
}

static char *cforeach_closeTag(FILE *out, char *line) {
	fprintf(out, "}\n");
	return modules_findEndOfTag(line) + 1;
}