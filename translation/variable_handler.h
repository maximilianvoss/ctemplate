#ifndef CTEMPLATE_VARIABLE_HANDLER_H
#define CTEMPLATE_VARIABLE_HANDLER_H

#include <csafestring.h>

#define VARIABLE_HANDLER_MAP_NOT_SET "__internal_Undetermined"

typedef struct {
	csafestring_t *mapName;
	csafestring_t *variableName;
} handler_variable_t;

handler_variable_t *varhandler_create(char *variableName);
void varhandler_destroy(handler_variable_t *handler);

#endif 
