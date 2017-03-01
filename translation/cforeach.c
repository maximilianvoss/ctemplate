#include "cforeach.h"
#include "variable_handler.h"

static char *cforeach_openTag(char *line, FILE *out);
static char *cforeach_closeTag(char *line, FILE *out);
static void cforeach_counterLoop(csafestring_t *var, csafestring_t *begin, csafestring_t *end, csafestring_t *step, FILE *out);
static void cforeach_elementLoop(csafestring_t *var, csafestring_t *items, FILE *out);

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

static char *cforeach_openTag(char *line, FILE *out) {
	csafestring_t *var = modules_extractVariable(line, "var");
	csafestring_t *begin = modules_extractVariable(line, "begin");
	csafestring_t *end = modules_extractVariable(line, "end");
	csafestring_t *step = modules_extractVariable(line, "step");
	csafestring_t *items = modules_extractVariable(line, "items");

	if ( var != NULL && items != NULL ) {
		cforeach_elementLoop(var, items, out);
	} else if ( var != NULL && begin != NULL && end != NULL ) {
		cforeach_counterLoop(var, begin, end, step, out);
	}

	safe_destroy(var);
	safe_destroy(begin);
	safe_destroy(end);
	safe_destroy(step);
	safe_destroy(items);
	return modules_findEndOfTag(line) + 1;
}

static void cforeach_counterLoop(csafestring_t *var, csafestring_t *begin, csafestring_t *end, csafestring_t *step, FILE *out) {
	fprintf(out, "for ( ");
	fprintf(out, "__internal_mfunction->set(__internal_%sValues, \"%s\", __internal_hfunction->intToString(__internal_expressionString, 255, %s));", VARIABLE_HANDLER_MAP_NOT_SET, var->data,
	        begin->data);
	fprintf(out, "atoi(__internal_mfunction->get(__internal_%sValues, \"%s\")) <= %s;", VARIABLE_HANDLER_MAP_NOT_SET, var->data, end->data);
	fprintf(out, "__internal_mfunction->set(__internal_%sValues, \"%s\", __internal_hfunction->intToString(__internal_expressionString, 255,", VARIABLE_HANDLER_MAP_NOT_SET, var->data);
	if ( step == NULL ) {
		fprintf(out, "atoi(__internal_mfunction->get(__internal_%sValues, \"%s\")) + 1", VARIABLE_HANDLER_MAP_NOT_SET, var->data);
	} else {
		fprintf(out, "atoi(__internal_mfunction->get(__internal_%sValues, \"%s\")) + %s", VARIABLE_HANDLER_MAP_NOT_SET, var->data, step->data);
	}
	fprintf(out, "))) {");
}

static void cforeach_elementLoop(csafestring_t *var, csafestring_t *items, FILE *out) {

//	fprintf(out, "void *__internal_requestValues = __internal_mfunction->createMap();\n");
//	fprintf(out, "void *__internal_requestObjects = __internal_mfunction->createMap();\n");
//	fprintf(out, "__internal_mfunction->parseJson(__internal_mfunction->set, __internal_requestValues, __internal_requestObjects, __internal_jsonString);\n");

	fprintf(out, "{\n");
}

static char *cforeach_closeTag(char *line, FILE *out) {
	fprintf(out, "}\n");
	return modules_findEndOfTag(line) + 1;
}