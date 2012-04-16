#include "lispo.h"

#define MAX_NUMBER_OF_BRACKETS_PAIR    64

void init_rootContext(lcontext_t *ctx)
{
	/* for tokenizer */
	ctx->bracketsCounter = -1;

	/* for parser */
	ctx->cellRoot = NULL;
	ctx->cellHead = NULL;
	ctx->startBracketCellsPtrStackPos = -1;
}

lcontext_t *new_rootContext(int argc, char **argv)
{
	lcontext_t *ctx = (lcontext_t *)malloc(sizeof(lcontext_t));

	/* for file */
	if(argc == 2) {
		if((ctx->fp = fopen(argv[1], "r")) == NULL) {
			lstrerr(FILE_OPEN_ERROR);
			exit(1);
		}
	}

	/* for memory management */
	ctx->memoryArena = new_memoryArena();

	/* for parser */
	ctx->startBracketCellsPtrStack = (cons_t **)malloc(sizeof(cons_t *) * MAX_NUMBER_OF_BRACKETS_PAIR);

	init_rootContext(ctx);

	return ctx;
}

void free_rootContext(lcontext_t *ctx)
{
}
