#include <stdio.h>
#include <string.h>
#include "expression_builder.h"
#include "variable_handler.h"

static pattern_match_t *builder_buildVariable(FILE *out, pattern_match_t *matches, pattern_analyse_t *analysation);
static bool builder_buildStringCompare(FILE *out, pattern_match_t *matches);

static bool builder_buildStringCompare(FILE *out, pattern_match_t *matches) {
	if ( matches->type == VARIABLE || matches->type == STRING ) {
		if ( matches->next != NULL && matches->next->type == EQUATION && matches->next->next != NULL && ( matches->next->next->type == STRING || matches->next->next->type == VARIABLE ) ) {
			handler_variable_t *varhandler = varhandler_create(matches->string);
			handler_variable_t *varhandlerNext = varhandler_create(matches->next->next->string);
			
			if ( matches->type == VARIABLE ) {
				varhandler_output(out, "__internal_mfunction->get(__internal_%sValues, \"%s\") != NULL && ", varhandler);
			}
			if ( matches->next->next->type == VARIABLE ) {
				varhandler_output(out, "__internal_mfunction->get(__internal_%sValues, \"%s\") != NULL && ", varhandlerNext);
			}
			
			if ( !strncmp(matches->next->string, "eq", 2) || !strncmp(matches->next->string, "==", 2) ) {
				fprintf(out, "!strcmp(");
			} else {
				fprintf(out, "strcmp(");
			}

			if ( matches->type == VARIABLE ) {
				varhandler_output(out, "__internal_mfunction->get(__internal_%sValues, \"%s\"), ", varhandler);
			} else {
				fprintf(out, "\"%s\", ", matches->string);
			}

			if ( matches->next->next->type == VARIABLE ) {
				varhandler_output(out, "__internal_mfunction->get(__internal_%sValues, \"%s\")", varhandlerNext);
			} else {
				fprintf(out, "\"%s\"", matches->next->next->string);
			}
			fprintf(out, ")");

			varhandler_destroy(varhandler);
			varhandler_destroy(varhandlerNext);
			return true;
		}
	}
	return false;
}

static pattern_match_t *builder_buildVariable(FILE *out, pattern_match_t *matches, pattern_analyse_t *analysation) {
	if ( matches->type == VARIABLE ) {
		handler_variable_t *varhandler = varhandler_create(matches->string);
		if ( !analysation->hasFloat && !analysation->hasInt && !analysation->hasOperator ) {
			if ( builder_buildStringCompare(out, matches) ) {
				return matches->next->next;
			} else {
				varhandler_output(out, "__internal_mfunction->get(__internal_%sValues, \"%s\")", varhandler);
			}
		} else if ( analysation->hasFloat ) {
			varhandler_output(out, "atof(__internal_mfunction->get(__internal_%sValues, \"%s\"))", varhandler);
		} else if ( analysation->hasInt ) {
			varhandler_output(out, "atol(__internal_mfunction->get(__internal_%sValues, \"%s\"))", varhandler);
		}
		varhandler_destroy(varhandler);
	}
	return matches;
}

void builder_generateCode(FILE *out, pattern_match_t *matches, pattern_analyse_t *analysation, bool returnString) {

	if ( returnString ) {
		if ( analysation->hasFloat ) {
			fprintf(out, "__internal_hfunction->floatToString(__internal_expressionString, EXPR_STRING_LENGTH, ");
		} else if ( analysation->hasInt || analysation->hasEquation ) {
			fprintf(out, "__internal_hfunction->intToString(__internal_expressionString, EXPR_STRING_LENGTH, ");
		}
	}

	while ( matches != NULL ) {
		if ( matches->type == VARIABLE ) {
			matches = builder_buildVariable(out, matches, analysation);
		} else if ( matches->type == STRING ) {
			if ( builder_buildStringCompare(out, matches) ) {
				matches = matches->next->next;
			} else {
				fprintf(out, "\"%s\"", matches->string);
			}
		} else if ( matches->type == EQUATION ) {
			if ( !strncmp(matches->string, "eq", 2) ) {
				fprintf(out, "==");
			} else if ( !strncmp(matches->string, "ne", 2) ) {
				fprintf(out, "!=");
			} else {
				fprintf(out, "%s", matches->string);
			}
		} else if ( matches->type == INT ) {
			fprintf(out, "%s", matches->string);
		} else if ( matches->type == FLOAT ) {
			fprintf(out, "%s", matches->string);
		} else if ( matches->type == OPERATOR ) {
			fprintf(out, "%s", matches->string);
		} else if ( matches->type == POPEN ) {
			fprintf(out, "(");
		} else if ( matches->type == PCLOSE ) {
			fprintf(out, ")");
		} else if ( matches->type == NOT ) {
			fprintf(out, "!");
		}

		matches = matches->next;
	}

	if ( returnString ) {
		if ( analysation->hasFloat ) {
			fprintf(out, ")");
		} else if ( analysation->hasInt || analysation->hasEquation ) {
			fprintf(out, ")");
		}
	}
}