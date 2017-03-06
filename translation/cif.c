#include "cif.h"
#include "expression.h"

static char *cif_openTag(FILE *out, char *line);
static char *cif_closeTag(FILE *out, char *line);

static translation_module_t module_cif = {
		.tagOpen = "<c:if",
		.tagOpenLen = 5,
		.tagClose = "</c:if",
		.tagCloseLen = 6,
		.functionOpen = cif_openTag,
		.functionClose = cif_closeTag,
		.data = NULL,
		.next = NULL
};

void cif_register(translation_module_t *modules) {
	modules_register(modules, &module_cif);
}

void cif_unregister(translation_module_t *modules) {
	modules_unregister(modules, &module_cif);
}

static char *cif_openTag(FILE *out, char *line) {
	csafestring_t *test = modules_extractVariable(line, "test");

	if ( test == NULL ) {
		safe_destroy(test);
		return modules_findEndOfTag(line) + 1;
	}

	fprintf(out, "if ( ");
	if ( !safe_strncmp(test, "${", 2) ) {
		expression_eval(out, test->data, false);
	} else {
		fprintf(out, "%s", test->data);
	}
	fprintf(out, " ) {\n");

	safe_destroy(test);
	return modules_findEndOfTag(line) + 1;
}

static char *cif_closeTag(FILE *out, char *line) {
	fprintf(out, "}\n");
	return modules_findEndOfTag(line) + 1;
}