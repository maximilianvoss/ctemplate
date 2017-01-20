#ifndef CTEMPLATE_EXPRESSION_BUILDER_H
#define CTEMPLATE_EXPRESSION_BUILDER_H

#include "expression.h"
#include <stdbool.h>

void builder_generateCode(FILE *out, pattern_match_t *matches, pattern_analyse_t *analysation, bool returnString);

#endif


