#include "../ctemplate.h"
#include "../utils/map.h"

#ifndef EXAMPLEPATH
#define EXAMPLEPATH ""
#endif

int main(int argc, char **argv) {
	ctemplate_functions_t methods;
	methods.createMap = hash_createMap;
	methods.destroyMap = hash_destroyMap;
	methods.get = hash_get;
	methods.set = hash_set;
	methods.unset = hash_unset;

	ctemplate_t *ctemplate = ctemplate_init(EXAMPLEPATH, NULL, &methods, 1);
	char *value = ctemplate_executeTemplate(ctemplate, "template.html", "{\"variable\":\"value\"}");
	printf("%s\n", value);
	free(value);
	ctemplate_destroy(ctemplate);
	return 0;
}