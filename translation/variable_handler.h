#ifndef CTEMPLATE_VARIABLE_HANDLER_H
#define CTEMPLATE_VARIABLE_HANDLER_H

#include <csafestring.h>
#include <stdio.h>

#define ROOT_MAP "__internal_Root"

typedef struct {
	csafestring_t *mapName;
	csafestring_t *variableName;
} handler_variable_t;

handler_variable_t *varhandler_create(char *variableName);
void varhandler_destroy(handler_variable_t *handler);
void varhandler_output(FILE *out, char *str, handler_variable_t *varhandler);
void varhandler_registerVariable(FILE *file, char *varName, char *inputData);
void varhandler_unregisterVariable(FILE *file, char *varName);

#endif 
