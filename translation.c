#include "translation.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 4096

void translation_processLine(FILE *out, char *line);
FILE *translation_createSourceFile(char *filename);
void translation_closeSourceFile(FILE *file);
void translation_compileCode(char *sourcePath, char *libraryPath);

void translation_processTemplate(char *templatePath, char *sourcePath, char *libraryPath) {
	char buffer[BUFFER_SIZE + 1];
	memset(buffer, '\0', BUFFER_SIZE + 1);

	FILE *in = fopen(templatePath, "r");
	FILE *out = translation_createSourceFile(sourcePath);

	while ( ! feof(in)) {

		fgets(buffer, BUFFER_SIZE, in);
		translation_processLine(out, buffer);
	}

	fclose(in);
	translation_closeSourceFile(out);
	translation_compileCode(sourcePath, libraryPath);
}


void translation_processLine(FILE *out, char *line) {
	long length = strlen(line);

	if ( line[length - 1] == '\n' ) {
		line[length - 1] = '\0';
	}

	fprintf (out, "printf (\"%s\\n\");\n", line);
}

FILE *translation_createSourceFile(char *filename) {
	FILE *file = fopen(filename, "w");
	fprintf(file, "#include <stdio.h>\n");
	fprintf(file, "void execute() {\n");

	return file;
}

void translation_closeSourceFile(FILE *file) {
	fprintf(file, "}\n");
	fclose(file);
}


void translation_compileCode(char *sourcePath, char *libraryPath) {
	char buffer[BUFFER_SIZE];
	strcpy(buffer, "gcc -shared -o ");
	strcat(buffer, libraryPath);
	strcat(buffer, " ");
	strcat(buffer, sourcePath);
	
	system(buffer);
}