#include "utils.h"

char *floatToString(char *str, size_t size, float expr) {
	snprintf(str, size, "%f", expr);
	return str;
}

char *intToString(char *str, size_t size, int expr) {
	snprintf(str, size, "%d", expr);
	return str;
}
	