#include "translation.h"
#include "translation/variable_handler.h"
#include "compiler.h"

#define BUFFER_SIZE 4096

static void translation_processLine(translation_module_t *translation_modules, FILE *out, char *line);
static void translation_createSourceHeader(FILE *file);
static void translation_closeSourceFile(FILE *file);

void translation_processTemplate(translation_module_t *translation_modules, char *templatePath, char *sourcePath, char *libraryPath) {
	char buffer[BUFFER_SIZE + 1];
	FILE *in = fopen(templatePath, "r");
	FILE *out = fopen(sourcePath, "w");

	translation_createSourceHeader(out);

	while ( !feof(in) ) {
		memset(buffer, '\0', BUFFER_SIZE + 1);
		fgets(buffer, BUFFER_SIZE, in);
		translation_processLine(translation_modules, out, buffer);
	}

	fclose(in);
	translation_closeSourceFile(out);
	compiler_compileCode(sourcePath, libraryPath);
}

static void translation_createSourceHeader(FILE *file) {
	fprintf(file, "#include <csafestring.h>\n");
	fprintf(file, "#include <stdio.h>\n");
	fprintf(file, "typedef struct {\n");
	fprintf(file, "	void * (* createMap) ();\n");
	fprintf(file, "	void (* destroyMap) (void *map);\n");
	fprintf(file, "	char * (* get) (void *map, char *key);\n");
	fprintf(file, "	void (* set) (void *map, char *key, char *value);\n");
	fprintf(file, "void (*unset)(void *map, char *key);\n");
	fprintf(file, "	char * (* find) (void *map, char *pattern);\n");
	fprintf(file, " void (*parseJson)(void (*set)(void *map, char *key, char *value), void *data, void *objects, char *json);\n");
	fprintf(file, "} ctemplate_functions_t;\n\n");
	fprintf(file, "char *__internal_floatToString(char *str, size_t size, float expr ) {\n");
	fprintf(file, "snprintf(str, size, \"%s\", expr);\n", "%f");
	fprintf(file, "return str;\n");
	fprintf(file, "}\n\n");
	fprintf(file, "char *__internal_intToString(char *str, size_t size, int expr ) {\n");
	fprintf(file, "snprintf(str, size, \"%s\", expr);\n", "%d");
	fprintf(file, "return str;\n");
	fprintf(file, "}\n");

	fprintf(file, "void execute(csafestring_t *__internal_string, ctemplate_functions_t *__internal_mfunction, char *__internal_jsonString) {\n");
	fprintf(file, "void *__internal_%sValues = __internal_mfunction->createMap();\n", VARIABLE_HANDLER_MAP_NOT_SET);
	fprintf(file, "void *__internal_requestValues = __internal_mfunction->createMap();\n");
	fprintf(file, "void *__internal_requestObjects = __internal_mfunction->createMap();\n");
	fprintf(file, "char __internal_expressionString[255];\n");
	fprintf(file, "char *__internal_tmp;\n");
	fprintf(file, "__internal_mfunction->set(__internal_%sValues, \"request\", __internal_jsonString);\n", VARIABLE_HANDLER_MAP_NOT_SET);
	fprintf(file, "__internal_mfunction->parseJson(__internal_mfunction->set, __internal_requestValues, __internal_requestObjects, __internal_jsonString);\n");
}

static void translation_processLine(translation_module_t *translation_modules, FILE *out, char *line) {
	char buffer[BUFFER_SIZE + 1];
	memset(buffer, '\0', BUFFER_SIZE + 1);
	void *method;

	char *ptr = buffer;
	while ( *line != '\0' ) {
		if ( *line == '\n' && *( line + 1 ) == '\0' ) {
			*ptr = '\0';
			line++;
			ptr++;
		} else if ( *line == '\n' ) {
			line++;
		} else if ( ( *line == ' ' || *line == '\t' ) && ( *( line + 1 ) == ' ' || *( line + 1 ) == '\t' ) ) {
			line++;
		} else if ( *line == '\t' ) {
			*ptr = ' ';
			line++;
			ptr++;
		} else if ( *line == '\"' ) {
			*ptr = '\\';
			ptr++;
			*ptr = '\"';
			ptr++;
			line++;
		} else {
			method = modules_matches(translation_modules, line);
			if ( method != NULL ) {
				if ( *buffer != '\0' ) {
					fprintf(out, "safe_strcat(__internal_string, \"%s\");\n", buffer);
					memset(buffer, '\0', BUFFER_SIZE + 1);
					ptr = buffer;
				}
				line = modules_execute(method, line, out);
			} else {
				*ptr = *line;
				line++;
				ptr++;
			}
		}
	}

	if ( *buffer != '\0' ) {
		fprintf(out, "safe_strcat(__internal_string, \"%s\");\n", buffer);
	}
}

static void translation_closeSourceFile(FILE *file) {
	fprintf(file, "__internal_mfunction->destroyMap(__internal_%sValues);\n", VARIABLE_HANDLER_MAP_NOT_SET);
	fprintf(file, "__internal_mfunction->destroyMap(__internal_requestValues);\n");
	fprintf(file, "__internal_mfunction->destroyMap(__internal_requestObjects);\n");
	fprintf(file, "}\n");
	fclose(file);
}





