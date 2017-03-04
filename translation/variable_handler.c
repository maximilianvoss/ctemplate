#include "variable_handler.h"

handler_variable_t *varhandler_create(char *variableName) {
	handler_variable_t *handler;

	char *pos;
	if ( variableName == NULL ) {
		return NULL;
	}
	handler = calloc(sizeof(handler_variable_t), 1);

	pos = strchr(variableName, '.');
	if ( pos == NULL ) {
		handler->mapName = safe_create(ROOT_MAP);
		handler->variableName = safe_create(variableName);
	} else {
		handler->mapName = safe_create(NULL);
		safe_strncpy(handler->mapName, variableName, pos - variableName);

		handler->variableName = safe_create(pos + 1);
	}
	return handler;
}

void varhandler_destroy(handler_variable_t *handler) {
	if ( handler != NULL ) {
		safe_destroy(handler->mapName);
		safe_destroy(handler->variableName);
		free(handler);
	}
}

void varhandler_output(FILE *out, char *str, handler_variable_t *varhandler) {
	fprintf(out, str, varhandler->mapName->data, varhandler->variableName->data);
}

void varhandler_registerVariable(FILE *file, char *varName, char *inputData) {
	fprintf(file, "void *__internal_%s = __internal_mfunction->createMap();\n", varName);
	fprintf(file, "__internal_mfunction->set(__internal_%s, \"%s\", __internal_jsonString);\n", ROOT_MAP, varName);
	fprintf(file, "__internal_mfunction->parseJson(__internal_mfunction->set, __internal_%s, %s);\n", varName, inputData);
}

void varhandler_unregisterVariable(FILE *file, char *varName) {
	fprintf(file, "__internal_mfunction->destroyMap(__internal_%s);\n", varName);
	fprintf(file, "__internal_mfunction->unset(__internal_%s, \"%s\");\n", ROOT_MAP, varName);
}