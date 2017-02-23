#include "cchoose.h"
#include "expression.h"

static char *cchoose_openTag(char *line, FILE *out);
static char *cchoose_closeTag(char *line, FILE *out);
static char *cwhen_openTag(char *line, FILE *out);
static char *cwhen_closeTag(char *line, FILE *out);
static char *cotherwise_openTag(char *line, FILE *out);
static char *cotherwise_closeTag(char *line, FILE *out);

static translation_module_t module_cchoose = {
		.tagOpen = "<c:choose",
		.tagOpenLen = 9,
		.tagClose = "</c:choose",
		.tagCloseLen = 10,
		.functionOpen = cchoose_openTag,
		.functionClose = cchoose_closeTag,
		.data = NULL,
		.next = NULL
};

static translation_module_t module_cwhen = {
		.tagOpen = "<c:when",
		.tagOpenLen = 7,
		.tagClose = "</c:when",
		.tagCloseLen = 8,
		.functionOpen = cwhen_openTag,
		.functionClose = cwhen_closeTag,
		.data = NULL,
		.next = NULL
};

static translation_module_t module_cotherwise = {
		.tagOpen = "<c:otherwise",
		.tagOpenLen = 12,
		.tagClose = "</c:otherwise",
		.tagCloseLen = 12,
		.functionOpen = cotherwise_openTag,
		.functionClose = cotherwise_closeTag,
		.data = NULL,
		.next = NULL
};

void cchoose_register(translation_module_t *modules) {
	if ( module_cchoose.data == NULL ) {
		module_cchoose.data = (bool *) malloc(sizeof(bool));
	}
	modules_register(modules, &module_cchoose);
	modules_register(modules, &module_cwhen);
	modules_register(modules, &module_cotherwise);
}

void cchoose_unregister(translation_module_t *modules) {
	free(module_cchoose.data);
	modules_unregister(modules, &module_cchoose);
	modules_unregister(modules, &module_cwhen);
	modules_unregister(modules, &module_cotherwise);
}

static char *cchoose_openTag(char *line, FILE *out) {
	bool *data = (bool *) module_cchoose.data;
	*data = true;
	return modules_findEndOfTag(line) + 1;
}

static char *cchoose_closeTag(char *line, FILE *out) {
	return modules_findEndOfTag(line) + 1;
}

static char *cwhen_openTag(char *line, FILE *out) {
	csafestring_t *test = modules_extractVariable(line, "test");
	bool *data = (bool *) module_cchoose.data;

	if ( test == NULL ) {
		safe_destroy(test);
		return modules_findEndOfTag(line) + 1;
	}

	if ( *data ) {
		*data = false;
		fprintf(out, "if ( ");
	} else {
		fprintf(out, "else if ( ");
	}
	if ( !safe_strncmp(test, "${", 2) ) {
		expression_eval(test->data, out, false);
	} else {
		fprintf(out, "%s", test->data);
	}
	fprintf(out, " ) {\n");

	safe_destroy(test);
	return modules_findEndOfTag(line) + 1;
}

static char *cwhen_closeTag(char *line, FILE *out) {
	fprintf(out, "}\n");
	return modules_findEndOfTag(line) + 1;
}

static char *cotherwise_openTag(char *line, FILE *out) {
	fprintf(out, "else {\n");
	return modules_findEndOfTag(line) + 1;
}

static char *cotherwise_closeTag(char *line, FILE *out) {
	fprintf(out, "}\n");
	return modules_findEndOfTag(line) + 1;
}