#ifndef UTILS_MAP_H
#define UTILS_MAP_H

void *hash_createMap();
void hash_destroyMap(void *map);
void hash_put(void *data, char *key, char *value);
char *hash_get(void *data, char *key);
char *hash_find(void *data, char *pattern);

#endif
