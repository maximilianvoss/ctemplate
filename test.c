#include "ctemplate.h"
#include <stdio.h>

int main(int argc, char **argv) {

	ctemplate_init("/Users/voss/Documents/01_Private/00_code/ctemplate/", "/Users/voss/Documents/01_Private/00_code/ctemplate/work/");

	ctemplate_executeTemplate("templates/template1.html", NULL);
	ctemplate_executeTemplate("templates/template2.html", NULL);
	
	ctemplate_unload();
	return 0;
}