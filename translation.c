#include "translation.h"
#include "expression.h"
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 4096

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
//	memset(buffer, '\0', BUFFER_SIZE + 1);

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

char *translation_functionSet(char *line, FILE *out) {
	char *tmp;
	char boundValue;
	char boundVar;

	char *value = strstr(line, "value=");
	boundValue = value[6];
	value += 7;

	char *var = strstr(line, "var=");
	boundVar = var[4];
	var += 5;

	char *returnPoint = strchr(value, '>');

	tmp = strchr(value, boundValue);
	*tmp = '\0';

	tmp = strchr(var, boundVar);
	*tmp = '\0';

	if ( !strncmp(value, "${", 2) ) {
		fprintf(out, "mfunction->set(data, \"%s\", ", var);
		expression_eval(value, out, true);
		fprintf(out, ");\n");
	} else {
		fprintf(out, "mfunction->set(data, \"%s\", \"%s\");\n", var, value);
	}

	returnPoint++;
	return returnPoint;
}

char *translation_functionOut(char *line, FILE *out) {
	char *tmp;
	char boundValue;
	char boundDefault;

	char *defaultValue = strstr(line, "default=");
	if ( defaultValue != NULL ) {
		boundDefault = *( defaultValue + 8 );
		defaultValue += 9;
		defaultValue = strdup(defaultValue);
		tmp = strchr(defaultValue, boundDefault);
		*tmp = '\0';
	}

	char *value = strstr(line, "value=");
	boundValue = value[6];
	value += 7;

	tmp = strchr(value, boundValue);
	char *returnPoint = strchr(tmp, '>');
	*tmp = '\0';
	
	if ( !strncmp(value, "${", 2) ) {
//		fprintf(out, "safe_strcat(string, ");
//		expression_eval(value, out, true);
//		fprintf(out, ");\n");

		fprintf(out, "tmp = ");
		expression_eval(value, out, true);
		fprintf(out, ";\n");
		fprintf(out, "safe_strcat(string, (tmp != NULL) ? tmp : \"%s\");\n", ( defaultValue != NULL ) ? defaultValue : "");
	} else {
		fprintf(out, "safe_strcat(string, \"%s\");\n", value);
	}

	if ( defaultValue != NULL ) {
		free(defaultValue);
	}

	returnPoint++;
	return returnPoint;
}

char *translation_functionRemove(char *line, FILE *out) {
	char boundVar;
	char *tmp;
	char *var = strstr(line, "var=");

	boundVar = var[4];
	var += 5;

	char *returnPoint = strchr(var, '>');

	tmp = strchr(var, boundVar);
	*tmp = '\0';

	fprintf(out, "mfunction->unset(data, \"%s\");\n", var);

	returnPoint++;
	return returnPoint;
}

char *translation_functionIf(char *line, FILE *out) {
	char boundTest;
	char *tmp;
	char *test = strstr(line, "test=");
	boundTest = test[5];
	test += 6;

	tmp = strchr(test, boundTest);
	char *returnPoint = strchr(tmp, '>');

	if ( !strncmp(test, "${", 2) ) {
		fprintf(out, "if ( ");
		expression_eval(test, out, false);
		fprintf(out, " ) {\n");
	} else {
		tmp = strchr(test, boundTest);
		*tmp = '\0';
		fprintf(out, "if ( %s ) {\n", test);
	}

	returnPoint++;
	return returnPoint;
}