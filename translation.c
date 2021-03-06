#include "translation.h"
#include "compiler.h"
#include "loader.h"

#define BUFFER_SIZE 4096

static void translation_processLine(translation_module_t *translation_modules, FILE *out, csafestring_t *output, char *line);
static void translation_createSourceHeader(FILE *file);
static void translation_closeSourceFile(FILE *file);

void translation_processTemplate(translation_module_t *translation_modules, char *templatePath, char *sourcePath, char *libraryPath) {
	char buffer[BUFFER_SIZE + 1];
	FILE *in = fopen(templatePath, "r");
	FILE *out = fopen(sourcePath, "w");
	csafestring_t *output = safe_create(NULL);

	translation_createSourceHeader(out);

	while ( !feof(in) ) {
		memset(buffer, '\0', BUFFER_SIZE + 1);
		fgets(buffer, BUFFER_SIZE, in);
		translation_processLine(translation_modules, out, output, buffer);
	}
	translation_processLine(translation_modules, out, output, NULL);

	safe_destroy(output);
	fclose(in);
	translation_closeSourceFile(out);
	compiler_compileCode(sourcePath, libraryPath);
}

static void translation_createSourceHeader(FILE *file) {
	fprintf(file, "#include <stdio.h>\n");
	fprintf(file, "#include <stdlib.h>\n");
	fprintf(file, "#include <string.h>\n");
	fprintf(file, "#define EXPR_STRING_LENGTH 255\n\n");
	
	fprintf(file, "typedef struct {\n");
	fprintf(file, "	void * (* createMap)();\n");
	fprintf(file, "	void (* destroyMap)(void *);\n");
	fprintf(file, "	char * (* get)(void *, char *);\n");
	fprintf(file, "	void (* set)(void *, char *, char *);\n");
	fprintf(file, "	void (* unset)(void *, char *);\n");
	fprintf(file, " void (* parseJson)(void (* set)(void *, char *, char *), void *, char *, char *);\n");
	fprintf(file, "} ctemplate_functions_t;\n\n");

	fprintf(file, "typedef struct {\n");
	fprintf(file, "	char *(* intToString)(char *, size_t, int);\n");
	fprintf(file, "	char *(* floatToString)(char *, size_t, float);\n");
	fprintf(file, "	char *(* safe_strcat)(void *, const char *);\n");
	fprintf(file, "} ctemplate_utilities_t;\n\n");

	fprintf(file, "void setVariable(ctemplate_functions_t *mfunctions, void *map, char *varName, char *jsonString) {\n");
	fprintf(file, "	char *objName = calloc(sizeof(char), strlen(varName) + 4 );\n");
	fprintf(file, "	strcpy(objName, varName);\n");
	fprintf(file, "	strcat(objName, \"[o]\");\n");
	fprintf(file, "	mfunctions->set(map, objName, jsonString);\n");
	fprintf(file, "	mfunctions->parseJson(mfunctions->set, map, varName, jsonString);\n");
	fprintf(file, "	free(objName);\n");
	fprintf(file, "}\n\n");

	fprintf(file, "char *getVariable(ctemplate_functions_t *mfunctions, void *map, char *varName) {\n");
	fprintf(file, " char *tmp = mfunctions->get(map, varName);\n");
	fprintf(file, " if ( tmp != NULL ) {\n");
	fprintf(file, " 	return tmp;\n");
	fprintf(file, " }\n");
	fprintf(file, "	char *objName = calloc(sizeof(char), strlen(varName) + 4 );\n");
	fprintf(file, "	strcpy(objName, varName);\n");
	fprintf(file, "	strcat(objName, \"[o]\");\n");
	fprintf(file, " tmp = mfunctions->get(map, objName);\n");
	fprintf(file, " if ( tmp != NULL ) {\n");
	fprintf(file, "	 free(objName);\n");
	fprintf(file, "	 return tmp;\n");
	fprintf(file, " }\n");
	fprintf(file, "	strcpy(objName, varName);\n");
	fprintf(file, "	strcat(objName, \"[x]\");\n");
	fprintf(file, " tmp = mfunctions->get(map, objName);\n");
	fprintf(file, "	free(objName);\n");
	fprintf(file, "	return tmp;\n");
	fprintf(file, "}\n\n");
	
	fprintf(file, "void %s(void *__internal_string, ctemplate_functions_t *__internal_mfunction, ctemplate_utilities_t *__internal_hfunction, char *__internal_jsonString) {\n",
	        MODULE_EXEC_METHOD);
	fprintf(file, "char __internal_expressionString[EXPR_STRING_LENGTH];\n");
	fprintf(file, "char *__internal_tmp;\n");
	fprintf(file, "void *__internal_root = __internal_mfunction->createMap();\n");
	fprintf(file, "setVariable(__internal_mfunction, __internal_root, \"request\", __internal_jsonString);\n\n");
}

static void translation_processLine(translation_module_t *translation_modules, FILE *out, csafestring_t *output, char *line) {
	void *method;

	if ( line == NULL ) {
		if ( output->data[0] != '\0' ) {
			fprintf(out, "__internal_hfunction->safe_strcat(__internal_string, \"%s\");\n", output->data);
			safe_emptyBuffer(output);
		}
		return;
	}
	
	while ( *line != '\0' ) {
		if ( *line == '\n' && *( line + 1 ) == '\0' ) {
			safe_strchrappend(output, '\0');
			line++;
		} else if ( *line == '\n' ) {
			line++;
		} else if ( ( *line == ' ' || *line == '\t' ) && ( *( line + 1 ) == ' ' || *( line + 1 ) == '\t' ) ) {
			line++;
		} else if ( *line == '\t' ) {
			safe_strchrappend(output, ' ');
			line++;
		} else if ( *line == '\"' ) {
			safe_strcat(output, "\\\"");
			line++;
		} else {
			method = modules_matches(translation_modules, line);
			if ( method != NULL ) {
				if ( output->data[0] != '\0' ) {
					fprintf(out, "__internal_hfunction->safe_strcat(__internal_string, \"%s\");\n", output->data);
					safe_emptyBuffer(output);
				}
				line = modules_execute(out, method, line);
			} else {
				safe_strchrappend(output, *line);
				line++;
			}
		}
	}
}

static void translation_closeSourceFile(FILE *file) {
	fprintf(file, "__internal_mfunction->destroyMap(__internal_root);\n");
	fprintf(file, "}\n");
	fclose(file);
}





