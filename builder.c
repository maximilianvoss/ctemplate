#include <stdio.h>
#include <string.h>
#include "builder.h"

pattern_match_t *builder_buildVariable(FILE *out, pattern_match_t *matches, pattern_analyse_t *analysation);
bool builder_buildStringCompare(FILE *out, pattern_match_t *matches);

bool builder_buildStringCompare(FILE *out, pattern_match_t *matches) {
	if ( matches->type == VARIABLE || matches->type == STRING ) {
		if ( matches->next != NULL && matches->next->type == EQUATION && matches->next->next != NULL && ( matches->next->next->type == STRING || matches->next->next->type == VARIABLE ) ) {
			if ( matches->type == VARIABLE ) {
				fprintf(out, "mfunction->get(data, \"%s\") != NULL && ", matches->string);
			}
			if ( matches->next->next->type == VARIABLE ) {
				fprintf(out, "mfunction->get(data, \"%s\") != NULL && ", matches->next->next->string);
			}
			
			if ( !strncmp(matches->next->string, "eq", 2) || !strncmp(matches->next->string, "==", 2) ) {
				fprintf(out, "!strcmp(");
			} else {
				fprintf(out, "strcmp(");
			}

			if ( matches->type == VARIABLE ) {
				fprintf(out, "mfunction->get(data, \"%s\"), ", matches->string);
			} else {
				fprintf(out, "\"%s\", ", matches->string);
			}

			if ( matches->next->next->type == VARIABLE ) {
				fprintf(out, "mfunction->get(data, \"%s\")", matches->next->next->string);
			} else {
				fprintf(out, "\"%s\"", matches->next->next->string);
			}
			fprintf(out, ")");
			return true;
		}
	}
	return false;
}

pattern_match_t *builder_buildVariable(FILE *out, pattern_match_t *matches, pattern_analyse_t *analysation) {
	if ( matches->type == VARIABLE ) {
		if ( !analysation->hasFloat && !analysation->hasInt && !analysation->hasOperator ) {
			if ( builder_buildStringCompare(out, matches) ) {
				return matches->next->next;
			} else {
				fprintf(out, "mfunction->get(data, \"%s\")", matches->string);
			}
		} else if ( analysation->hasFloat ) {
			fprintf(out, "atof(mfunction->get(data, \"%s\"))", matches->string);
		} else if ( analysation->hasInt ) {
			fprintf(out, "atol(mfunction->get(data, \"%s\"))", matches->string);
		}
	}
	return matches;
}

void builder_generateCode(FILE *out, pattern_match_t *matches, pattern_analyse_t *analysation, bool returnString) {

	if ( returnString ) {
		if ( analysation->hasFloat ) {
			fprintf(out, "floatToString(expressionString, 255, ");
		} else if ( analysation->hasInt || analysation->hasEquation ) {
			fprintf(out, "intToString(expressionString, 255, ");
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