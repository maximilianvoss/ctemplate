#include "translation.h"
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 4096

void translation_processLine(FILE *out, char *line);
void translation_createSourceHeader(FILE *file);
void translation_closeSourceFile(FILE *file);
void compiler_compileCode(char *sourcePath, char *libraryPath);

void translation_processTemplate(char *templatePath, char *sourcePath, char *libraryPath) {
	char buffer[BUFFER_SIZE + 1];
	memset(buffer, '\0', BUFFER_SIZE + 1);

	FILE *in = fopen(templatePath, "r");
	FILE *out = fopen(sourcePath, "w");

	translation_createSourceHeader(out);

	while ( !feof(in) ) {
		fgets(buffer, BUFFER_SIZE, in);
		translation_processLine(out, buffer);
	}

	fclose(in);
	translation_closeSourceFile(out);
	compiler_compileCode(sourcePath, libraryPath);
}


void translation_processLine(FILE *out, char *line) {
	long length = strlen(line);

	if ( line[length - 1] == '\n' ) {
		line[length - 1] = '\0';
	}

	fprintf(out, "printf (\"%s\\n\");\n", line);
}

void translation_createSourceHeader(FILE *file) {
	fprintf(file, "#include <stdio.h>\n");
	fprintf(file, "void execute() {\n");
}

void translation_closeSourceFile(FILE *file) {
	fprintf(file, "}\n");
	fclose(file);
}