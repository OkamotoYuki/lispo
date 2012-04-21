#include <ctype.h>

#include "lispo.h"

#define SB_PTR_STACK           ctx->startBracketCellsPtrStack
#define SB_PTR_STACK_POS       ctx->startBracketCellsPtrStackPos

#define push_startBracketCellPtr(H) do {\
	SB_PTR_STACK_POS++;\
	SB_PTR_STACK[SB_PTR_STACK_POS] = H;\
} while(0)

#define pop_startBracketCellPtr(H) do {\
	H = SB_PTR_STACK[SB_PTR_STACK_POS];\
	SB_PTR_STACK[SB_PTR_STACK_POS] = NULL;\
	SB_PTR_STACK_POS--;\
} while(0)

int parse(lcontext_t *ctx, char *pos, int size)
{
	char c = *pos;
	char str[MAX_LINE_LEN];
	char *e;
	int ival;

	if(c == ')') {
		TREE_HEAD->cdr = NULL;
		pop_startBracketCellPtr(TREE_HEAD);
		ctx->bracketsCounter--;
		return T;
	}

	if(size == 1) {
		switch(c) {
			case '(':
				if(!TREE_HEAD) {
					TREE_ROOT = new_consCell(ctx);
					TREE_HEAD = TREE_ROOT;
				}
				else {
					TREE_HEAD->cdr = new_consCell(ctx);
					TREE_HEAD = TREE_HEAD->cdr;
				}
				TREE_HEAD->otype = O_START_BRACKET;
				push_startBracketCellPtr(TREE_HEAD);
				if(ctx->bracketsCounter == -1) ctx->bracketsCounter += 2;
				else ctx->bracketsCounter++;
				return T;
			case '+':
				TREE_HEAD->car = new_consCell(ctx);
				TREE_HEAD = TREE_HEAD->car;
				TREE_HEAD->otype = O_ADD;
				return T;
			case '-':
				TREE_HEAD->car = new_consCell(ctx);
				TREE_HEAD = TREE_HEAD->car;
				TREE_HEAD->otype = O_SUB;
				return T;
			case '*':
				TREE_HEAD->car = new_consCell(ctx);
				TREE_HEAD = TREE_HEAD->car;
				TREE_HEAD->otype = O_MUL;
				return T;
			case '/':
				TREE_HEAD->car = new_consCell(ctx);
				TREE_HEAD = TREE_HEAD->car;
				TREE_HEAD->otype = O_DIV;
				return T;
			case '<':
				TREE_HEAD->car = new_consCell(ctx);
				TREE_HEAD = TREE_HEAD->car;
				TREE_HEAD->otype = O_LT;
				return T;
			case '>':
				TREE_HEAD->car = new_consCell(ctx);
				TREE_HEAD = TREE_HEAD->car;
				TREE_HEAD->otype = O_GT;
				return T;
			case '=':
				break;
			default:
				break;
		}
	}

	memcpy(str, pos, size);
	str[size] = '\0';

	switch(c) {
		case '-':
			ival = (int)strtol(str, &e, 10);
			if(*e != '\0') return NIL;
			TREE_HEAD->cdr = new_consCell(ctx);
			TREE_HEAD = TREE_HEAD->cdr;
			TREE_HEAD->otype = O_NUM;
			TREE_HEAD->ivalue = ival;
			return T;
		default:
			if(isdigit(c)) {
				ival = (int)strtol(str, &e, 10);
				if(*e != '\0') return NIL;
				TREE_HEAD->cdr = new_consCell(ctx);
				TREE_HEAD = TREE_HEAD->cdr;
				TREE_HEAD->otype = O_NUM;
				TREE_HEAD->ivalue = ival;
				return T;
			}
			else if(size == 2 && !strncmp(str, "if", 2)) {
				TREE_HEAD->car = new_consCell(ctx);
				TREE_HEAD = TREE_HEAD->car;
				TREE_HEAD->otype = O_IF;
				return T;
			}
			else if(size == 4 && !strncmp(str, "setq", 4)) {
				TREE_HEAD->car = new_consCell(ctx);
				TREE_HEAD = TREE_HEAD->car;
				TREE_HEAD->otype = O_SETQ;
				return T;
			}
			else if(size == 5 && !strncmp(str, "defun", 5)) {
				TREE_HEAD->car = new_consCell(ctx);
				TREE_HEAD = TREE_HEAD->car;
				TREE_HEAD->otype = O_DEFUN;
				return T;
			}
			TREE_HEAD->cdr = new_consCell(ctx);
			TREE_HEAD = TREE_HEAD->cdr;
			TREE_HEAD->otype = O_STRING;
			TREE_HEAD->svalue = (char *)malloc(size + 1);
			memcpy(TREE_HEAD->svalue, pos, size);
			TREE_HEAD->svalue[size] = '\0';
			return T;
	}
}
