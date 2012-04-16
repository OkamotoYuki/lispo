#include "lispo.h"

void lstrerr(Errno errno)
{
	char *str;
	switch(errno) {
		case ARG_ERROR:
			str = "Too many arguments!!";
			break;
		case FILE_OPEN_ERROR:
			str = "Script not found!!";
			break;
		case SYNTAX_ERROR:
			str = "Syntax error!!";
			break;
		default:
			str = "Error!!";
	}
	printf("%s\n", str);
}
