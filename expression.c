#include <string.h>
#include <stdlib.h>
#include <regex.h>
#include "expression.h"
#include "builder.h"

static char regexInitiated = 0;
static regex_t regexInt;
static regex_t regexFloat;
static regex_t regexString;
static regex_t regexVariable;
static regex_t regexEmpty;
static regex_t regexEquation;
static regex_t regexOperator;
static regex_t regexPopen;
static regex_t regexPclose;
static regex_t regexNot;

void expression_free(pattern_match_t *matches);
pattern_match_t *expression_extract(char *string, pattern_analyse_t *analysation);

void expression_init() {
	if ( !regexInitiated ) {
		regcomp(&regexFloat, "^[ |\t]*([0-9]+\\.[0-9]+)", REG_EXTENDED);
		regcomp(&regexInt, "^[ |\t]*([0-9]+)", REG_EXTENDED);
		regcomp(&regexString, "^[ |\t]*[\"|\'](.*)[\"|\']", REG_EXTENDED);
		regcomp(&regexVariable, "^[ |\t]*([a-zA-Z][a-zA-Z0-9]*)", REG_EXTENDED);
		regcomp(&regexEquation, "^[ |\t]*(eq|ne|==|!=|<=|>=|>|<)", REG_EXTENDED);
		regcomp(&regexOperator, "^[ |\t]*([\\+|*|-|/])", REG_EXTENDED);
		regcomp(&regexEmpty, "^[ |\t]*$", REG_EXTENDED);
		regcomp(&regexPopen, "^[ |\t]*(\\()", REG_EXTENDED);
		regcomp(&regexPclose, "^[ |\t]*(\\))", REG_EXTENDED);
		regcomp(&regexNot, "^[ |\t]*(not|!)", REG_EXTENDED);
	}
	regexInitiated = 1;
}

void expression_destroy() {
	regfree(&regexInt);
	regfree(&regexFloat);
	regfree(&regexString);
	regfree(&regexVariable);
	regfree(&regexPopen);
	regfree(&regexPclose);
	regfree(&regexNot);
	regfree(&regexEmpty);
}

void expression_eval(char *valueIn, FILE *out, bool returnString) {
	expression_init();

	pattern_analyse_t analysation;
	analysation.hasEquation = false;
	analysation.hasFloat = false;
	analysation.hasInt = false;
	analysation.hasOperator = false;
	analysation.hasString = false;
	analysation.hasVariable = false;

	char *tmp;
	if ( !strncmp(valueIn, "${", 2) ) {
		char *dup = strdup(valueIn);
		char *expr = dup;
		expr += 2;
		tmp = strchr(expr, '}');
		*tmp = '\0';

		pattern_match_t *matches = expression_extract(expr, &analysation);
		builder_generateCode(out, matches, &analysation, returnString);

		free(dup);
		expression_free(matches);
	}
}

void expression_free(pattern_match_t *matches) {
	if ( matches == NULL ) {
		return;
	}
	expression_free(matches->next);
	free(matches->string);
	free(matches);
}

pattern_match_t *expression_extract(char *string, pattern_analyse_t *analysation) {
	regmatch_t match[2];
	pattern_match_t *matches = NULL;
	pattern_match_t *lastMatch = NULL;
	pattern_match_t *matched = NULL;
	pattern_type_t type;

	while ( 1 ) {
		if ( !regexec(&regexFloat, string, 2, match, 0) ) {
			type = FLOAT;
			analysation->hasFloat = true;
		} else if ( !regexec(&regexInt, string, 2, match, 0) ) {
			type = INT;
			analysation->hasInt = true;
		} else if ( !regexec(&regexString, string, 2, match, 0) ) {
			type = STRING;
			analysation->hasString = true;
		} else if ( !regexec(&regexEquation, string, 2, match, 0) ) {
			type = EQUATION;
			analysation->hasEquation = true;
		} else if ( !regexec(&regexOperator, string, 2, match, 0) ) {
			type = OPERATOR;
			analysation->hasOperator = true;
		} else if ( !regexec(&regexVariable, string, 2, match, 0) ) {
			type = VARIABLE;
			analysation->hasVariable = true;
		} else if ( !regexec(&regexPopen, string, 2, match, 0) ) {
			type = POPEN;
		} else if ( !regexec(&regexPclose, string, 2, match, 0) ) {
			type = PCLOSE;
		} else if ( !regexec(&regexNot, string, 2, match, 0) ) {
			type = NOT;
		} else if ( !regexec(&regexEmpty, string, 2, match, 0) ) {
			break;
		} else {
			if ( matches != NULL ) {
				free(matches);
			}
			return NULL;
		}

		matched = (pattern_match_t *) malloc(sizeof(pattern_match_t));
		matched->string = (char *) calloc(sizeof(char), (int64_t) match[1].rm_eo - match[1].rm_so + 1);
		strncpy(matched->string, &string[match[1].rm_so], (int64_t) match[1].rm_eo - match[1].rm_so);
		string += (int64_t) match[1].rm_eo;

		if ( type == STRING ) {
			string++;
		}
		matched->type = type;
		matched->next = NULL;

		if ( matches == NULL ) {
			matches = matched;
			lastMatch = matches;
		} else {
			lastMatch->next = matched;
			lastMatch = matched;
		}

	}
	return matches;
}

