#include "modules.h"
#include "cset.h"
#include "cremove.h"
#include "cout.h"
#include "cif.h"
#include "cforeach.h"
#include "cchoose.h"
#include "expression.h"

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

void modules_register(translation_module_t *modules, translation_module_t *module) {
	module->next = modules->next;
	modules->next = module;
}

void modules_unregister(translation_module_t *modules, translation_module_t *module) {
	translation_module_t *tmp = modules;
	while ( tmp->next != NULL ) {
		if ( tmp->next == module ) {
			tmp->next = module->next;
			break;
		}
		tmp = tmp->next;
	}
}

void *modules_matches(translation_module_t *modules, char *line) {
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

char *modules_execute(FILE *out, void *method, char *line) {
	char *(*exec)(FILE *, char *) = method;
	return exec(out, line);
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

translation_module_t *modules_init() {
	translation_module_t *modules = (translation_module_t *) malloc(sizeof(translation_module_t));
	modules->tagOpen = NULL;
	modules->tagClose = NULL;
	modules->next = NULL;

	cchoose_register(modules);
	cforeach_register(modules);
	cif_register(modules);
	cout_register(modules);
	cremove_register(modules);
	cset_register(modules);
	expression_register(modules);

	return modules;
}

void modules_destroy(translation_module_t *modules) {
	cchoose_unregister(modules);
	cforeach_unregister(modules);
	cif_unregister(modules);
	cout_unregister(modules);
	cremove_unregister(modules);
	cset_unregister(modules);
	expression_unregister(modules);

	free(modules);
}