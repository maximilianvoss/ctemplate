#ifndef CTEMPLATE_FUNCTIONS_H
#define CTEMPLATE_FUNCTIONS_H

typedef struct {
	void *(*createMap)();
	void (*destroyMap)(void *map);
	char *(*get)(void *map, char *key);
	void (*set)(void *map, char *key, char *value);
	void (*unset)(void *map, char *key);
	void (*parseJson)(void (*set)(void *map, char *key, char *value), void *data, char *json);
} ctemplate_functions_t;

typedef struct {
	char *(*intToString)(char *str, size_t size, int expr);
	char *(*floatToString)(char *str, size_t size, float expr);
	char *(*strcat)(csafestring_t *, const char *);
} ctemplate_utilities_t;

#endif