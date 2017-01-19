#include "cif.h"
#include "expression.h"
#include "modules.h"

char *cif_openTag(char *line, FILE *out);
char *cif_closeTag(char *line, FILE *out);

translation_module_t module_cif = {
		.tagOpen = "<c:if",
		.tagOpenLen = 5,
		.tagClose = "</c:if",
		.tagCloseLen = 6,
		.functionOpen = cif_openTag,
		.functionClose = cif_closeTag,
		.data = NULL,
		.next = NULL
};

void cif_register() {
	modules_register(&module_cif);
}

void cif_unregister() {
	modules_unregister(&module_cif);
}

char *cif_openTag(char *line, FILE *out) {
	csafestring_t *test = modules_extractVariable(line, "test");

	if ( test == NULL ) {
		safe_destroy(test);
		return modules_findEndOfTag(line) + 1;
	}

	fprintf(out, "if ( ");
	if ( !safe_strncmp(test, "${", 2) ) {
		expression_eval(test->data, out, false);
	} else {
		fprintf(out, "%s", test->data);
	}
	fprintf(out, " ) {\n");

	safe_destroy(test);
	return modules_findEndOfTag(line) + 1;
}

char *cif_closeTag(char *line, FILE *out) {
	fprintf(out, "}\n");
	return modules_findEndOfTag(line) + 1;
}