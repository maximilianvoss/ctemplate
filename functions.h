#ifndef CTEMPLATE_FUNCTIONS_H
#define CTEMPLATE_FUNCTIONS_H

typedef struct {
	void *(*createMap)();
	void (*destroyMap)(void *map);
	char *(*get)(void *map, char *key);
	void (*set)(void *map, char *key, char *value);
	void (*unset)(void *map, char *key);
	void (*parseJson)(void (*set)(void *map, char *key, char *value), void *data, void *objects, char *json);
} ctemplate_functions_t;

#endif