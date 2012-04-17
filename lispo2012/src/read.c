#include "lispo.h"

#include <readline/history.h>
#include <readline/readline.h>

void read(lcontext_t *ctx)
{
	int isFirstLine = 1, isSyntaxError = 0;
	char *line, *pos;
	int size = 0;

	while(1) {
		if(ctx->fp) {
			line = (char *)malloc(MAX_LINE_LEN);
			fgets(line, MAX_LINE_LEN, ctx->fp);
		}
		else {
			if(isFirstLine) {
				line = readline(">>> ");
				isFirstLine = 0;
			}
			else line = readline("    ");
		}

		pos = line;

		if((*(pos = skip_space(pos))) != '(' && ctx->bracketsCounter == -1) {
			lstrerr(SYNTAX_ERROR);
			free(line);
			line = NULL;
			isFirstLine = 1;
			init_rootContext(ctx);
			continue;
		}

		while(*pos != '\0') {
			if(!ctx->bracketsCounter) {
				isSyntaxError = 1;
				break;
			}
			size = tokenize(pos);
			if(size == END_OF_LINE) {
				parser(ctx, pos, strlen(pos));
				break;
			}
			else if(size == QUIT) {
				free(line);
				free_rootContext(ctx);
				exit(0);
			}
			parser(ctx, pos, size);
			pos += size;
			pos = skip_space(pos);
		}


		free(line);
		line = NULL;
		if(isSyntaxError) {
			lstrerr(SYNTAX_ERROR);
			isSyntaxError = 0;
			isFirstLine = 1;
			init_rootContext(ctx);
			continue;
		}

		if(!ctx->bracketsCounter) break;
	}
	return;
}
