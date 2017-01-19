#include "modules.h"
#include "cset.h"
#include "cremove.h"
#include "cout.h"
#include "cif.h"
#include "cforeach.h"
#include "cchoose.h"
#include "expression.h"

char *modules_findEndOfTag(char *line);

static translation_module_t *modules = NULL;

csafestring_t *modules_extractVariable(char *line, char *name) {
	csafestring_t *searchStr = safe_create(name);
	safe_strchrappend(searchStr, '=');
	char boundaryChar;

	char *startPos = strstr(line, searchStr->data);
	if ( startPos == NULL ) {
		safe_destroy(searchStr);
		return NULL;
	}
	startPos += safe_strlen(searchStr);
	safe_destroy(searchStr);

	boundaryChar = *startPos;
	startPos++;
	csafestring_t *value = safe_create(NULL);
	while ( *startPos != '\0' && *startPos != boundaryChar ) {
		safe_strchrappend(value, *startPos);
		startPos++;
	}
	return value;
}

void modules_register(translation_module_t *module) {
	if ( modules == NULL ) {
		modules = module;
	} else {
		module->next = modules;
		modules = module;
	}
}

void modules_unregister(translation_module_t *module) {
	translation_module_t *tmp;

	if ( modules != NULL ) {
		if ( modules->next == NULL ) {
			modules = NULL;
		} else {
			if ( modules == module ) {
				modules = modules->next;
			} else {
				tmp = modules;
				while ( tmp->next != NULL ) {
					if ( tmp->next == module ) {
						tmp->next = module->next;
						break;
					}
					tmp = tmp->next;
				}
			}
		}
	}
}

void *modules_matches(char *line) {
	translation_module_t *tmp = modules;

	if ( !( !strncmp(line, "<c:", 3) || !strncmp(line, "</c:", 4) || !strncmp(line, "${", 2) ) ) {
		return NULL;
	}
	while ( tmp != NULL ) {
		if ( tmp->tagOpen != NULL && tmp->tagOpenLen != 0 && !strncmp(line, tmp->tagOpen, tmp->tagOpenLen) ) {
			return tmp->functionOpen;
		}
		if ( tmp->tagClose != NULL && tmp->tagCloseLen != 0 && !strncmp(line, tmp->tagClose, tmp->tagCloseLen) ) {
			return tmp->functionClose;
		}
		tmp = tmp->next;
	}
	return NULL;
}

char *modules_execute(void *method, char *line, FILE *out) {
	char *(*exec)(char *, FILE *) = method;
	return exec(line, out);
}

char *modules_findEndOfElement(char *line, char chr) {
	bool inQuote = false;
	bool inDoubleQuote = false;

	while ( *line != '\0' ) {
		if ( *line == '\'' ) {
			if ( inQuote ) {
				inQuote = false;
			} else {
				inQuote = true;
			}
		} else if ( *line == '\"' ) {
			if ( inDoubleQuote ) {
				inDoubleQuote = false;
			} else {
				inDoubleQuote = true;
			}
		} else if ( *line == chr ) {
			if ( !inQuote && !inDoubleQuote ) {
				return line;
			}
		}
		line++;
	}
	return NULL;
}

char *modules_findEndOfTag(char *line) {
	return modules_findEndOfElement(line, '>');
}

void modules_init() {
	cchoose_register();
	cforeach_register();
	cif_register();
	cout_register();
	cremove_register();
	cset_register();
	expression_register();
}

void modules_destroy() {
	cchoose_unregister();
	cforeach_unregister();
	cif_unregister();
	cout_unregister();
	cremove_unregister();
	cset_unregister();
	expression_unregister();
}