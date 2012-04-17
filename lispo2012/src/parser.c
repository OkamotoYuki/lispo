#include <ctype.h>

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
	char c = *pos;
	char str[MAX_LINE_LEN];
	char *e;
	int ival;
	cons_t *head = ctx->cellHead;
	cons_t *cell;

	if(c == ')') {
		head->cdr = NULL;
		pop_startBracketCellPtr(head);
		ctx->bracketsCounter--;
		return TRUE;
	}

//	cell = (cons_t *)malloc(sizeof(cons_t));
	cell = allocate_consCell(ctx);

	if(size == 1) {
		switch(c) {
			case '(':
				cell->type = T_START_BRACKET;
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
				cell->type = T_ADD;
				head->car == cell;
				head = cell;
				return TRUE;
			case '-':
				cell->type = T_SUB;
				head->car == cell;
				head = cell;
				return TRUE;
			case '*':
				cell->type = T_MUL;
				head->car = cell;
				head = cell;
				return TRUE;
			case '/':
				cell->type = T_DIV;
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
	else {
		strncpy(str, pos, size);
		switch(c) {
			case '-':
				ival = (int)strtol(str, &e, 10);
				if(*e != '\0') FALSE;
				cell->type = T_NUM;
				cell->ivalue = ival;
				head->cdr = cell;
				head = cell;
				return TRUE;
			default:
				if(isdigit(c)) {
					ival = (int)strtol(str, &e, 10);
					if(*e != '\0') FALSE;
					cell->type = T_NUM;
					cell->ivalue = ival;
					head->cdr = cell;
					head = cell;
					return TRUE;
				}
				else if(size == 2 && !strncmp(str, "if", 2)) {
					cell->type = T_IF;
					head->car = cell;
					head = cell;
					return TRUE;
				}
				else if(size == 4 && !strncmp(str, "setq", 4)) {
					cell->type = T_SETQ;
					head->car = cell;
					head = cell;
					return TRUE;
				}
				else if(size == 5 && !strncmp(str, "defun", 5)) {
					cell->type = T_DEFUN;
					head->car = cell;
					head = cell;
					return TRUE;
				}
				cell->type = T_STRING;
				cell->svalue = allocate_string(ctx, size);
				strncpy(cell->svalue, pos, size);
				return TRUE;
		}
	}
}
