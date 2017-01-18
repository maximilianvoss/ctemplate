#include "translation.h"
#include "expression.h"
#include <string.h>
#include <csafestring.h>

#define BUFFER_SIZE 4096

csafestring_t *translation_extractVariable(char *line, char *name);
char *translation_findEndOfTag(char *line);
void translation_processLine(FILE *out, char *line);
void translation_createSourceHeader(FILE *file);
void translation_closeSourceFile(FILE *file);
void compiler_compileCode(char *sourcePath, char *libraryPath);
char *translation_functionSet(char *line, FILE *out);
char *translation_functionOut(char *line, FILE *out);
char *translation_functionRemove(char *line, FILE *out);
char *translation_functionIf(char *line, FILE *out);

void translation_processTemplate(char *templatePath, char *sourcePath, char *libraryPath) {
	char buffer[BUFFER_SIZE + 1];
	FILE *in = fopen(templatePath, "r");
	FILE *out = fopen(sourcePath, "w");

	translation_createSourceHeader(out);

	while ( !feof(in) ) {
		memset(buffer, '\0', BUFFER_SIZE + 1);
		fgets(buffer, BUFFER_SIZE, in);
		translation_processLine(out, buffer);
	}

	fclose(in);
	translation_closeSourceFile(out);
	compiler_compileCode(sourcePath, libraryPath);
}

void translation_processLine(FILE *out, char *line) {
	char buffer[BUFFER_SIZE + 1];
	memset(buffer, '\0', BUFFER_SIZE + 1);

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
		} else if ( *line == '<' && *( line + 1 ) == 'c' && *( line + 2 ) == ':' ) {
			if ( *buffer != '\0' ) {
				fprintf(out, "safe_strcat(string, \"%s\");\n", buffer);
				memset(buffer, '\0', BUFFER_SIZE + 1);
				ptr = buffer;
			}

			if ( !strncmp(( line + 3 ), "set", 3) ) {
				line = translation_functionSet(line, out);
			} else if ( !strncmp(( line + 3 ), "out", 3) ) {
				line = translation_functionOut(line, out);
			} else if ( !strncmp(( line + 3 ), "remove", 6) ) {
				line = translation_functionRemove(line, out);
			} else if ( !strncmp(( line + 3 ), "if", 2) ) {
				line = translation_functionIf(line, out);
			}
		} else if ( *line == '<' && *( line + 1 ) == '/' && *( line + 2 ) == 'c' && *( line + 3 ) == ':' ) {
			if ( *buffer != '\0' ) {
				fprintf(out, "safe_strcat(string, \"%s\");\n", buffer);
				memset(buffer, '\0', BUFFER_SIZE + 1);
				ptr = buffer;
			}

			if ( !strncmp(( line + 4 ), "if", 2) ) {
				line = strchr(line, '>');
				line++;
				fprintf(out, "}\n");
			}
		} else {
			*ptr = *line;
			line++;
			ptr++;
		}
	}

	if ( *buffer != '\0' ) {
		fprintf(out, "safe_strcat(string, \"%s\");\n", buffer);
	}
}

void translation_createSourceHeader(FILE *file) {
	fprintf(file, "#include <csafestring.h>\n");
	fprintf(file, "#include <stdio.h>\n");
	fprintf(file, "typedef struct {\n");
	fprintf(file, "	void * (* createMap) ();\n");
	fprintf(file, "	void (* destroyMap) (void *map);\n");
	fprintf(file, "	char * (* get) (void *map, char *key);\n");
	fprintf(file, "	void (* set) (void *map, char *key, char *value);\n");
	fprintf(file, "void (*unset)(void *map, char *key);\n");
	fprintf(file, "	char * (* find) (void *map, char *pattern);\n");
	fprintf(file, "} ctemplate_functions_t;\n\n");
	fprintf(file, "char *floatToString(char *str, size_t size, float expr ) {\n");
	fprintf(file, "snprintf(str, size, \"%s\", expr);\n", "%f");
	fprintf(file, "return str;\n");
	fprintf(file, "}\n\n");
	fprintf(file, "char *intToString(char *str, size_t size, int expr ) {\n");
	fprintf(file, "snprintf(str, size, \"%s\", expr);\n", "%d");
	fprintf(file, "return str;\n");
	fprintf(file, "}\n");
	
	fprintf(file, "void execute(csafestring_t *string, ctemplate_functions_t *mfunction, void *data) {\n");
	fprintf(file, "char expressionString[255];\n");
	fprintf(file, "char *tmp;\n");
}

void translation_closeSourceFile(FILE *file) {
	fprintf(file, "mfunction->destroyMap(data);\n");
	fprintf(file, "}\n");
	fclose(file);
}

csafestring_t *translation_extractVariable(char *line, char *name) {
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

char *translation_findEndOfTag(char *line) {
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
		} else if ( *line == '>' ) {
			if ( !inQuote && !inDoubleQuote ) {
				return line;
			}
		}
		line++;
	}
	return NULL;
}

char *translation_functionSet(char *line, FILE *out) {
	csafestring_t *value = translation_extractVariable(line, "value");
	csafestring_t *var = translation_extractVariable(line, "var");

	if ( value == NULL || var == NULL ) {
		safe_destroy(value);
		safe_destroy(var);
		return translation_findEndOfTag(line) + 1;
	}

	if ( !safe_strncmp(value, "${", 2) ) {
		fprintf(out, "mfunction->set(data, \"%s\", ", var->data);
		expression_eval(value->data, out, true);
		fprintf(out, ");\n");
	} else {
		fprintf(out, "mfunction->set(data, \"%s\", \"%s\");\n", var->data, value->data);
	}

	safe_destroy(value);
	safe_destroy(var);
	return translation_findEndOfTag(line) + 1;
}

char *translation_functionOut(char *line, FILE *out) {
	csafestring_t *value = translation_extractVariable(line, "value");
	csafestring_t *defaultValue = translation_extractVariable(line, "default");

	if ( value == NULL ) {
		safe_destroy(value);
		safe_destroy(defaultValue);
		return translation_findEndOfTag(line) + 1;
	}

	if ( !safe_strncmp(value, "${", 2) ) {
		fprintf(out, "tmp = ");
		expression_eval(value->data, out, true);
		fprintf(out, ";\n");
		fprintf(out, "safe_strcat(string, (tmp != NULL) ? tmp : \"%s\");\n", ( defaultValue != NULL ) ? defaultValue->data : "");
	} else {
		fprintf(out, "safe_strcat(string, \"%s\");\n", value->data);
	}

	safe_destroy(value);
	safe_destroy(defaultValue);
	return translation_findEndOfTag(line) + 1;
}

char *translation_functionRemove(char *line, FILE *out) {
	csafestring_t *var = translation_extractVariable(line, "var");

	if ( var == NULL ) {
		safe_destroy(var);
		return translation_findEndOfTag(line) + 1;
	}

	fprintf(out, "mfunction->unset(data, \"%s\");\n", var->data);

	safe_destroy(var);
	return translation_findEndOfTag(line) + 1;
}

char *translation_functionIf(char *line, FILE *out) {
	csafestring_t *test = translation_extractVariable(line, "test");

	if ( test == NULL ) {
		safe_destroy(test);
		return translation_findEndOfTag(line) + 1;
	}

	if ( !safe_strncmp(test, "${", 2) ) {
		fprintf(out, "if ( ");
		expression_eval(test->data, out, false);
		fprintf(out, " ) {\n");
	} else {
		fprintf(out, "if ( %s ) {\n", test->data);
	}

	safe_destroy(test);
	return translation_findEndOfTag(line) + 1;
}