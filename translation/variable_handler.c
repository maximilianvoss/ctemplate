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
		handler->mapName = safe_create(VARIABLE_HANDLER_MAP_NOT_SET);
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