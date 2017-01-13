#include "../ctemplate.h"
#include <stdio.h>
#include "../utils/map.h"

#ifndef EXAMPLEPATH
#define EXAMPLEPATH ""
#endif

int main(int argc, char **argv) {
	ctemplate_functions_t methods;
	methods.createMap = hash_createMap;
	methods.destroyMap = hash_destroyMap;
	methods.get = hash_get;
	methods.put = hash_put;
	methods.find = hash_find;

	ctemplate_init(EXAMPLEPATH, NULL, &methods, 1);
	printf("%s\n", ctemplate_executeTemplate("template.html", "{\"variable\":\"value\"}"));
	ctemplate_unload();
	return 0;
}