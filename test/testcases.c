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

int test_cif() {
	char *value = ctemplate_executeTemplate("cif.txt", NULL);
	ASSERTSTR("Works again", value);
	free(value);
	return 0;
}

int test_expression() {
	char *value = ctemplate_executeTemplate("expression.txt", "{\"var1\":\"value\", \"var2\":4 }");
	ASSERTSTR(
			"0: Direct Expression,1: Hallo,2: Hallo,3: value,4: 1,5: 0,6: 40,7: 40.000000,8: 40,9: 40.000000,10: 0,11: 1,12: 1,13: 1,14: 1,15: 0,16: 1,17: 1,18: 0,19: 0,20: 1,21: 0,22: 1,23: 0,\
24: 1,25: 1", value);
	free(value);
	return 0;
}

int test_cchoose() {
	char *value;

	value = ctemplate_executeTemplate("cchoose.txt", "{\"var\":\"value1\"}");
	ASSERTSTR("value1 is set", value);
	value = ctemplate_executeTemplate("cchoose.txt", "{\"var\":\"value2\"}");
	ASSERTSTR("value2 is set", value);
	value = ctemplate_executeTemplate("cchoose.txt", NULL);
	ASSERTSTR("nothing is set", value);

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
	TESTCALL("test_cif", test_cif);
	TESTCALL("test_expression", test_expression);
	TESTCALL("test_cchoose", test_cchoose);

	ctemplate_unload();
	return -1;
}