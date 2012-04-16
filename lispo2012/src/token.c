#include "lispo.h"

char *skip_space(char *start)
{
	while(*start == ' ') {
		start++;
	}
	return start;
}

int tokenize(char *start)
{
	char *end = start;

	end = skip_space(end);

	switch(*end) {
		case '(':
		case ')':
		case '*':
		case '/':
			return 1;
		default:
			break;
	}

	while(!(*end == ' ' || *end == '\0')) {
		end++;
	}
	if(*end == '\0') return END_OF_LINE;
	return (int)(end - start);
}
