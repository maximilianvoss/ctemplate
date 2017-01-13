#include "map.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct s_linkedlist {
	char *key;
	char *value;
	struct s_linkedlist *next;
} linkedlist_t;

void *hash_createMap() {
	printf("hash_createMap()...\n");
	linkedlist_t *map = (linkedlist_t *) malloc(sizeof(linkedlist_t));
	map->key = NULL;
	map->value = NULL;
	map->next = NULL;
	printf("hash_createMap()... DONE\n");
	return map;
}

void hash_destroyMap(void *data) {
	printf("hash_destroyMap([void *])...\n");
	if ( data == NULL ) {
		printf("hash_destroyMap([void *])... DONE\n");
		return;
	}
	linkedlist_t *map = (linkedlist_t *) data;
	hash_destroyMap(map->next);
	if ( map->key != NULL ) {
		free(map->key);
	}
	if ( map->value != NULL ) {
		free(map->value);
	}
	free(map);
	printf("hash_destroyMap([void *])... DONE\n");
}

void hash_put(void *data, char *key, char *value) {
	printf("hash_put([void *], %s, %s)...\n", key, value);
	linkedlist_t *map = (linkedlist_t *) data;

	if ( key == NULL ) {
		printf("hash_put([void *], %s, %s)... DONE\n", key, value);
		return;
	}

	size_t length = strlen(value) + 1;
	while ( map != NULL ) {
		if ( map->key != NULL && !strcmp(map->key, key) ) {
			free(map->value);
			map->value = (char *) malloc(sizeof(char) * length);
			strcpy(map->value, value);
			return;
		}
		map = map->next;
	}

	linkedlist_t *element = (linkedlist_t *) malloc(sizeof(linkedlist_t));
	element->value = (char *) malloc(sizeof(char) * length);
	strcpy(element->value, value);

	length = strlen(key) + 1;
	element->key = (char *) malloc(sizeof(char) * length);
	strcpy(element->key, key);

	map = (linkedlist_t *) data;
	element->next = map->next;
	map->next = element;
	printf("hash_put([void *], %s, %s)... DONE\n", key, value);
}

char *hash_get(void *data, char *key) {
	printf("hash_get([void *], %s)...\n", key);
	
	linkedlist_t *map = (linkedlist_t *) data;
	if ( key == NULL ) {
		printf("hash_get([void *], %s)... DONE\n", key);
		return NULL;
	}
	while ( map != NULL ) {
		if ( map->key != NULL && !strcmp(map->key, key) ) {
			return map->value;
		}
		map = map->next;
	}
	printf("hash_get([void *], %s)... DONE\n", key);
	return NULL;
}

char *hash_find(void *data, char *pattern) {
	return NULL;
}