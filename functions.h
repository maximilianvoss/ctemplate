#ifndef CTEMPLATE_FUNCTIONS_H
#define CTEMPLATE_FUNCTIONS_H

typedef struct {
	void *(*createMap)();
	void (*destroyMap)(void *map);
	char *(*get)(void *map, char *key);
	void (*set)(void *map, char *key, char *value);
	void (*unset)(void *map, char *key);
	char *(*find)(void *map, char *pattern);
} ctemplate_functions_t;

#endif