#include "ctemplate.h"
#include <stdio.h>

int main(int argc, char **argv) {

	ctemplate_executeTemplate("/Users/voss/Documents/01_Private/00_code/ctemplate/templates/template1.html", NULL);
	ctemplate_executeTemplate("/Users/voss/Documents/01_Private/00_code/ctemplate/templates/template2.html", NULL);
	
	ctemplate_unload();
	return 0;
}