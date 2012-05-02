#include "lispo.h"

char *skip_space(char *start)
{
	while(*start == ' ') {
		start++;
	}
	return start;
}

int isQuit(char *pos)
{
	if(!strncmp(pos, "bye", 3)) {
		pos += 3;
		pos = skip_space(pos);
		if(*pos == ')') {
			pos++;
			skip_space(pos);
			if(*pos == '\0') return T;
			else return NIL;
		}
	}
	else if(!strncmp(pos, "quit", 4)) {
		pos += 4;
		pos = skip_space(pos);
		if(*pos == ')') {
			pos++;
			skip_space(pos);
			if(*pos == '\0') return T;
			else return NIL;
		}
	}
	return NIL;
}

int tokenize(char *start)
{
	char *end = start;

	end = skip_space(end);

	switch(*end) {
		case '(':
			end++;
			end = skip_space(end);
			if(isQuit(end)) return QUIT;
		case ')':
		case '*':
		case '/':
			return 1;
		default:
			break;
	}

	while(!(*end == ' ' || *end == ')' || *end == '\0' || *end == '\n')) {
		end++;
	}
	if(*end == '\0') return END_OF_LINE;
	return (int)(end - start);
}
