#include "lispo.h"

#define push_startBracketCellPtr(H) do {\
	ctx->startBracketCellsPtrStackPos++;\
	ctx->startBracketCellsPtrStack[ctx->startBracketCellsPtrStackPos] = H;\
} while(0)

#define pop_startBracketCellPtr(H) do {\
	H = ctx->startBracketCellsPtrStack[ctx->startBracketCellsPtrStackPos];\
	ctx->startBracketCellsPtrStack[ctx->startBracketCellsPtrStackPos] = NULL;\
	ctx->startBracketCellsPtrStackPos--;\
} while(0)

int parser(lcontext_t *ctx, char *pos, int size)
{
	char c;
	cons_t *head = ctx->cellHead;
	if(size == 1) {
		c = *pos;
		if(c == ')') {
			head->cdr = NULL;
			pop_startBracketCellPtr(head);
			ctx->bracketsCounter--;
			return TRUE;
		}

		cons_t *cell = (cons_t *)malloc(sizeof(cons_t));
		switch(c) {
			case '(':
				cell->type = START_BRACKET;
				if(!head) {
					ctx->cellHead = cell;
					head = cell;
				}
				else {
					head->cdr = cell;
					head = cell;
				}
				push_startBracketCellPtr(head);
				if(ctx->bracketsCounter == -1) ctx->bracketsCounter += 2;
				else ctx->bracketsCounter++;
				return TRUE;
			case '+':
				cell->type = ADD;
				head->car == cell;
				head = cell;
				return TRUE;
			case '-':
				cell->type = SUB;
				head->car == cell;
				head = cell;
				return TRUE;
			case '*':
				cell->type = MUL;
				head->car = cell;
				head = cell;
				return TRUE;
			case '/':
				cell->type = DIV;
				head->car = cell;
				head = cell;
				return TRUE;
			case '<':
				break;
			case '>':
				break;
			case '=':
				break;
			default:
				return FALSE;
		}
	}
	return TRUE;
}
