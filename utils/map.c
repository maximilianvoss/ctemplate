#include "map.h"
#include <stdlib.h>
#include <string.h>

typedef struct s_linkedlist {
	char *key;
	char *value;
	struct s_linkedlist *next;
} linkedlist_t;

void *hash_createMap() {
	linkedlist_t *map = (linkedlist_t *) malloc(sizeof(linkedlist_t));
	map->key = NULL;
	map->value = NULL;
	map->next = NULL;
	return map;
}

void hash_destroyMap(void *data) {

	if ( data == NULL ) {
		return;
	}
	linkedlist_t *map = (linkedlist_t *) data;
	hash_destroyMap(map->next);
	if ( map->key != NULL )
		free(map->key);
	if ( map->value != NULL )
		free(map->value);
	free(map);
}

void hash_put(void *data, char *key, char *value) {
	linkedlist_t *map = (linkedlist_t *) data;

	linkedlist_t *element = (linkedlist_t *) malloc(sizeof(linkedlist_t));
	
	size_t length = strlen(key) + 1;
	element->key = (char *) malloc(sizeof(char) * length);
	strcpy(element->key, key);

	length = strlen(value) + 1;
	element->value = (char *) malloc(sizeof(char) * length);
	strcpy(element->value, value);

	element->next = map->next;
	map->next = element;
}

char *hash_get(void *data, char *key) {
	linkedlist_t *map = (linkedlist_t *) data;

	if ( key == NULL ) {
		return NULL;
	}
	while ( map != NULL ) {
		if ( map->key != NULL && !strcmp(map->key, key) ) {
			return map->value;
		}
		map = map->next;
	}
	return NULL;
}

char *hash_find(void *data, char *pattern) {
	linkedlist_t *map = (linkedlist_t *) data;
	return NULL;
}