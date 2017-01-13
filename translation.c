#include "translation.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 4096

void translation_processLine(FILE *out, char *line);
void translation_createSourceHeader(FILE *file);
void translation_closeSourceFile(FILE *file);
void compiler_compileCode(char *sourcePath, char *libraryPath);
char *translation_functionSet(char *line, FILE *out);
char *translation_functionOut(char *line, FILE *out);

void translation_processTemplate(char *templatePath, char *sourcePath, char *libraryPath) {
	char buffer[BUFFER_SIZE + 1];
	memset(buffer, '\0', BUFFER_SIZE + 1);

	FILE *in = fopen(templatePath, "r");
	FILE *out = fopen(sourcePath, "w");

	translation_createSourceHeader(out);

	while ( !feof(in) ) {
		fread(buffer, 1, BUFFER_SIZE, in);
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
			}
		} else {
			*ptr = *line;
			line++;
			ptr++;
		}
	}

	fprintf(out, "safe_strcat(string, \"%s\");\n", buffer);
}

void translation_createSourceHeader(FILE *file) {
	fprintf(file, "#include <csafestring.h>\n");
	fprintf(file, "typedef struct {\n");
	fprintf(file, "	void * (* createMap) ();\n");
	fprintf(file, "	void (* destroyMap) (void *map);\n");
	fprintf(file, "	char * (* get) (void *map, char *key);\n");
	fprintf(file, "	void (* put) (void *map, char *key, char *value);\n");
	fprintf(file, "	char * (* find) (void *map, char *pattern);\n");
	fprintf(file, "} ctemplate_functions_t;\n");
	fprintf(file, "void execute(csafestring_t *string, ctemplate_functions_t *mfunction, void *data) {\n");
	fprintf(file, "char *tmpCout;\n");
}

void translation_closeSourceFile(FILE *file) {
	fprintf(file, "mfunction->destroyMap(data);\n");
	fprintf(file, "}\n");
	fclose(file);
}

char *translation_functionSet(char *line, FILE *out) {
	char *tmp;
	char *returnPoint = strchr(line, '>');

	char *value = strstr(line, "value=\"");
	value = strchr(value, '\"');
	value++;

	char *var = strstr(line, "var=\"");
	var = strchr(var, '\"');
	var++;

	tmp = strchr(value, '\"');
	*tmp = '\0';

	tmp = strchr(var, '\"');
	*tmp = '\0';

	if ( !strncmp(value, "${", 2) ) {
		value += 2;
		tmp = strchr(value, '}');
		*tmp = '\0';
		fprintf(out, "mfunction->put(data, \"%s\", mfunction->get(data, \"%s\"));", var, value);
	} else {
		fprintf(out, "mfunction->put(data, \"%s\", \"%s\");", var, value);
	}

	returnPoint++;
	return returnPoint;
}

char *translation_functionOut(char *line, FILE *out) {
	char *tmp;
	char *returnPoint = strchr(line, '>');

	char *defaultValue = strstr(line, "default=\"");
	if ( defaultValue != NULL ) {
		defaultValue = strchr(defaultValue, '\"');
		defaultValue++;
		defaultValue = strdup(defaultValue);
		tmp = strchr(defaultValue, '\"');
		*tmp = '\0';
	} 

	char *value = strstr(line, "value=\"");
	value = strchr(value, '\"');
	value++;

	if ( !strncmp(value, "${", 2) ) {
		value += 2;
		tmp = strchr(value, '}');
		*tmp = '\0';

		fprintf(out, "tmpCout = mfunction->get(data, \"%s\");\n", value);
		fprintf(out, "safe_strcat(string, (tmpCout != NULL) ? tmpCout : \"%s\");\n", ( defaultValue != NULL ) ? defaultValue : "");
	} else {
		tmp = strchr(value, '\"');
		*tmp = '\0';
		fprintf(out, "safe_strcat(string, \"%s\");\n", value);
	}

	if ( defaultValue != NULL ) {
		free(defaultValue);
	}

	returnPoint++;
	return returnPoint;
}