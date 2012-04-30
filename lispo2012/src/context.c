#include "lispo.h"

#define MAX_NUMBER_OF_BRACKETS_PAIR    64
#define DATA_STACK_SIZE 256

void init_rootContext(lcontext_t *ctx)
{
	/* for tokenizer */
	ctx->bracketsCounter = -1;

	/* for parser */
	ctx->treeRoot = NULL;
	ctx->treeHead = NULL;
	ctx->startBracketCellsPtrStackPos = -1;
	ctx->isFunc = 0;
	ctx->isStartOfArg = 0;

	/* for code generator */
	ctx->startOfVMCode = NULL;
	ctx->headOfVMCode = NULL;
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

	/* for symbol table */
	ctx->symbolTable = (hashTable_t **)malloc(sizeof(hashTable_t *) * DEFAULT_SYMBOL_TABLE_SIZE);

	/* fot arg table */
	ctx->argTable = (hashTable_t **)malloc(sizeof(hashTable_t *) * DEFAULT_ARG_TABLE_SIZE);

	/* for parser */
	ctx->startBracketCellsPtrStack = (cons_t **)malloc(sizeof(cons_t *) * MAX_NUMBER_OF_BRACKETS_PAIR);

	/* for code generator */
	init_VMOpTable(ctx);

	/* for run VM */
	ctx->dataStack = (data_t *)malloc((sizeof(data_t)) * DATA_STACK_SIZE);

	init_rootContext(ctx);

	return ctx;
}

void free_rootContext(lcontext_t *ctx)
{
}
