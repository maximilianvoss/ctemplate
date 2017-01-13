#include <string.h>
#include <stdio.h>
#include "../ctemplate.h"
#include "../utils/map.h"

#ifndef TESTPATH
#define TESTPATH ""
#endif

#define TESTCALL(TESTNAME, TESTMETHOD)\
    printf("\nTest case:\t%s\n", TESTNAME);\
    if ( argv[1] == NULL ) \
        TESTMETHOD();\
    else\
        if (!strcmp(argv[1], TESTNAME))\
            return TESTMETHOD();

#define ASSERTSTR(EXPECTED, ACTUAL)\
    printf("Expected: \t%s\nActual: \t%s\n\n", EXPECTED, ACTUAL);\
    if (strcmp(EXPECTED, ACTUAL))\
        return 1;

#define ASSERTISNULL(ACTUAL)\
    printf("Expected: \t(null)\nActual: \t%s\n\n", ACTUAL);\
    if (ACTUAL != NULL)\
        return 1;

#define ASSERTINT(EXPECTED, ACTUAL)\
    printf("Expected: \t%d\nActual: \t%d\n\n", EXPECTED, ACTUAL);\
    if (EXPECTED != ACTUAL)\
        return 1;

int test_fileNotExists() {
	char *value = ctemplate_executeTemplate("filenotexists.txt", NULL);
	ASSERTISNULL(value);
	return 0;
}

int test_cout() {
	char *value = ctemplate_executeTemplate("cout.txt", "{\"var\": \"value\"}");
	ASSERTSTR("var: var,var-eval: value,var-default: defaultValue,var-default-not-set: ", value);
	free(value);
	return 0;
}

int test_cset() {
	char *value = ctemplate_executeTemplate("cset.txt", NULL);
	ASSERTSTR("var1: value1,var2: value1", value);
	free(value);
	return 0;
}

int test_cremove() {
	char *value = ctemplate_executeTemplate("cremove.txt", NULL);
	ASSERTSTR("value,Var not set", value);
	free(value);
	return 0;
}

int main(int argc, char **argv) {
	ctemplate_functions_t methods;
	methods.createMap = hash_createMap;
	methods.destroyMap = hash_destroyMap;
	methods.get = hash_get;
	methods.set = hash_set;
	methods.unset = hash_unset;
	methods.find = hash_find;

	ctemplate_init(TESTPATH, NULL, &methods, 1);

	TESTCALL("test_fileNotExists", test_fileNotExists);
	TESTCALL("test_cset", test_cset);
	TESTCALL("test_cout", test_cout);
	TESTCALL("test_cremove", test_cremove);

	ctemplate_unload();
	return -1;
}