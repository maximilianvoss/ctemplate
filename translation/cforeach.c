#include "cforeach.h"
#include "variable_handler.h"

static char *cforeach_openTag(FILE *out, char *line);
static char *cforeach_closeTag(FILE *out, char *line);
static void cforeach_counterLoop(FILE *out, csafestring_t *var, csafestring_t *begin, csafestring_t *end, csafestring_t *step);
static void cforeach_elementLoop(FILE *out, csafestring_t *var, csafestring_t *items);

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
	csafestring_t *begin = modules_extractVariable(line, "begin");
	csafestring_t *end = modules_extractVariable(line, "end");
	csafestring_t *step = modules_extractVariable(line, "step");
	csafestring_t *items = modules_extractVariable(line, "items");

	if ( var != NULL && items != NULL ) {
		cforeach_elementLoop(out, var, items);
	} else if ( var != NULL && begin != NULL && end != NULL ) {
		cforeach_counterLoop(out, var, begin, end, step);
	}

	safe_destroy(var);
	safe_destroy(begin);
	safe_destroy(end);
	safe_destroy(step);
	safe_destroy(items);
	return modules_findEndOfTag(line) + 1;
}

static void cforeach_counterLoop(FILE *out, csafestring_t *var, csafestring_t *begin, csafestring_t *end, csafestring_t *step) {
	fprintf(out, "for ( ");
	fprintf(out, "__internal_mfunction->set(__internal_%s, \"%s\", __internal_hfunction->intToString(__internal_expressionString, EXPR_STRING_LENGTH, %s));", ROOT_MAP,
	        var->data,
	        begin->data);
	fprintf(out, "atoi(__internal_mfunction->get(__internal_%s, \"%s\")) <= %s;", ROOT_MAP, var->data, end->data);
	fprintf(out, "__internal_mfunction->set(__internal_%s, \"%s\", __internal_hfunction->intToString(__internal_expressionString, EXPR_STRING_LENGTH,", ROOT_MAP, var->data);
	if ( step == NULL ) {
		fprintf(out, "atoi(__internal_mfunction->get(__internal_%s, \"%s\")) + 1", ROOT_MAP, var->data);
	} else {
		fprintf(out, "atoi(__internal_mfunction->get(__internal_%s, \"%s\")) + %s", ROOT_MAP, var->data, step->data);
	}
	fprintf(out, "))) {");
}

static void cforeach_elementLoop(FILE *out, csafestring_t *var, csafestring_t *items) {

//	fprintf(out, "void *__internal_requestValues = __internal_mfunction->createMap();\n");
//	fprintf(out, "void *__internal_requestObjects = __internal_mfunction->createMap();\n");
//	fprintf(out, "__internal_mfunction->parseJson(__internal_mfunction->set, __internal_requestValues, __internal_requestObjects, __internal_jsonString);\n");

	fprintf(out, "{\n");
}

static char *cforeach_closeTag(FILE *out, char *line) {
	fprintf(out, "}\n");
	return modules_findEndOfTag(line) + 1;
}