#include "../ctemplate.h"

#ifndef EXAMPLEPATH
#define EXAMPLEPATH ""
#endif

int main(int argc, char **argv) {
	ctemplate_init(EXAMPLEPATH, NULL, 1);

	ctemplate_executeTemplate("template.html", NULL);

	ctemplate_unload();
	return 0;
}