#ifndef CTEMPLATE_EXPRESSIONS_H
#define CTEMPLATE_EXPRESSIONS_H

#include <stdio.h>
#include <stdbool.h>
#include "modules.h"

typedef enum e_pattern_type {
	INT,
	FLOAT,
	STRING,
	VARIABLE,
	EQUATION,
	OPERATOR,
	POPEN,
	PCLOSE,
	NOT
} pattern_type_t;

typedef struct s_pattern_match {
	char *string;
	pattern_type_t type;
	struct s_pattern_match *next;
} pattern_match_t;

typedef struct {
	bool hasString;
	bool hasVariable;
	bool hasInt;
	bool hasFloat;
	bool hasEquation;
	bool hasOperator;
} pattern_analyse_t;

void expression_register(translation_module_t *modules);
void expression_unregister(translation_module_t *modules);
void expression_eval(FILE *out, char *line, bool returnString);

#endif
