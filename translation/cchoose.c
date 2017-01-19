#include "cchoose.h"
#include "expression.h"
#include "modules.h"

char *cchoose_openTag(char *line, FILE *out);
char *cchoose_closeTag(char *line, FILE *out);
char *cwhen_openTag(char *line, FILE *out);
char *cwhen_closeTag(char *line, FILE *out);
char *cotherwise_openTag(char *line, FILE *out);
char *cotherwise_closeTag(char *line, FILE *out);

translation_module_t module_cchoose = {
		.tagOpen = "<c:choose",
		.tagOpenLen = 9,
		.tagClose = "</c:choose",
		.tagCloseLen = 10,
		.functionOpen = cchoose_openTag,
		.functionClose = cchoose_closeTag,
		.data = NULL,
		.next = NULL
};

translation_module_t module_cwhen = {
		.tagOpen = "<c:when",
		.tagOpenLen = 7,
		.tagClose = "</c:when",
		.tagCloseLen = 8,
		.functionOpen = cwhen_openTag,
		.functionClose = cwhen_closeTag,
		.data = NULL,
		.next = NULL
};

translation_module_t module_cotherwise = {
		.tagOpen = "<c:otherwise",
		.tagOpenLen = 12,
		.tagClose = "</c:otherwise",
		.tagCloseLen = 12,
		.functionOpen = cotherwise_openTag,
		.functionClose = cotherwise_closeTag,
		.data = NULL,
		.next = NULL
};

void cchoose_register() {
	if ( module_cchoose.data == NULL ) {
		module_cchoose.data = (bool *) malloc(sizeof(bool));
	}
	modules_register(&module_cchoose);
	modules_register(&module_cwhen);
	modules_register(&module_cotherwise);
}

void cchoose_unregister() {
	free(module_cchoose.data);
	modules_unregister(&module_cchoose);
	modules_unregister(&module_cwhen);
	modules_unregister(&module_cotherwise);
}

char *cchoose_openTag(char *line, FILE *out) {
	bool *data = (bool *) module_cchoose.data;
	*data = true;
	return modules_findEndOfTag(line) + 1;
}

char *cchoose_closeTag(char *line, FILE *out) {
	return modules_findEndOfTag(line) + 1;
}

char *cwhen_openTag(char *line, FILE *out) {
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

char *cwhen_closeTag(char *line, FILE *out) {
	fprintf(out, "}\n");
	return modules_findEndOfTag(line) + 1;
}

char *cotherwise_openTag(char *line, FILE *out) {
	fprintf(out, "else {\n");
	return modules_findEndOfTag(line) + 1;
}

char *cotherwise_closeTag(char *line, FILE *out) {
	fprintf(out, "}\n");
	return modules_findEndOfTag(line) + 1;
}