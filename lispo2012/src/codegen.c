#include "lispo.h"

#define MAX_NUM_OF_ARGS 64

#define VM_OP_TABLE(op) ctx->VMOpTable[op]
#define push_arg(arg) do {\
	pos++;\
	argCellStack[pos] = arg;\
} while(0)
#define pop_arg(arg) do {\
	arg = argCellStack[pos];\
	pos--;\
} while(0)

static VMCode *add_VMCode(lcontext_t *ctx)
{
	HEAD_OF_VM_CODE->next = new_VMCode(ctx);
	return HEAD_OF_VM_CODE->next;
}

static void generate_PUSH_NUM_Code(lcontext_t *ctx, int ivalue)
{
	if(!HEAD_OF_VM_CODE) {
		HEAD_OF_VM_CODE = new_VMCode(ctx);
		START_OF_VM_CODE = HEAD_OF_VM_CODE;
	}
	else {
		HEAD_OF_VM_CODE = add_VMCode(ctx);
	}

	HEAD_OF_VM_CODE->otype = O_OpPUSH;
	HEAD_OF_VM_CODE->VMOp = VM_OP_TABLE(PUSH);
	HEAD_OF_VM_CODE->dtype = D_INT;
	HEAD_OF_VM_CODE->ivalue = ivalue;
	return;
}

static void generate_VMCode(lcontext_t *ctx, cons_t *cell);

static void compile_stringCells(lcontext_t *ctx, cons_t *cell)
{
	int pos = -1, numOfArgs = 0;
	cons_t *argCellStack[MAX_NUM_OF_ARGS];
	hashTable_t *table = search_symbol(ctx, cell->svalue);

	if(table) {
		switch(table->otype) {
			case O_SymVAL:
				cell->otype = O_NUM;
				cell->ivalue = table->value;
				generate_VMCode(ctx, cell);
				break;
			case O_SymFUNC:
				cell = cell->cdr;
				while(cell) {
					push_arg(cell);
					cell = cell->cdr;
				}
				numOfArgs = pos + 1;
				while(pos >= 0) {
					pop_arg(cell);
					generate_VMCode(ctx, cell);
				}

				HEAD_OF_VM_CODE = add_VMCode(ctx);
				HEAD_OF_VM_CODE->otype = O_OpCALL;
				HEAD_OF_VM_CODE->VMOp = VM_OP_TABLE(CALL);
				HEAD_OF_VM_CODE->funcTable = table;

				HEAD_OF_VM_CODE = add_VMCode(ctx);
				HEAD_OF_VM_CODE->otype = O_OpPOPR;
				HEAD_OF_VM_CODE->VMOp = VM_OP_TABLE(POPR);
				HEAD_OF_VM_CODE->numOfArgs = numOfArgs;
			default:
				break;
		}
	}
	else {
		table = search_arg(ctx, cell->svalue);
		if(table) {
			HEAD_OF_VM_CODE = add_VMCode(ctx);
			HEAD_OF_VM_CODE->otype = O_OpLOADA;
			HEAD_OF_VM_CODE->VMOp = VM_OP_TABLE(LOADA);
			HEAD_OF_VM_CODE->index = table->index;
		}
		else {/* TODO */}
	}
	return;
}

static void generate_VMCode(lcontext_t *ctx, cons_t *cell)
{
	if(!cell) return;

	int i = 0;
	char *symbol;
	cons_t *argCell;
	VMCode *codeCMP, *codeJMP, *L1, *L2, *L3;
	hashTable_t *table;

	switch(cell->otype) {
		case O_START_BRACKET:
			generate_VMCode(ctx, cell->car);
			return;
		case O_NUM:
			generate_PUSH_NUM_Code(ctx, cell->ivalue);
			return;
		case O_ADD:
			cell = cell->cdr;
			generate_VMCode(ctx, cell);
			while(cell->cdr) {
				cell = cell->cdr;
				generate_VMCode(ctx, cell);

				HEAD_OF_VM_CODE = add_VMCode(ctx);
				HEAD_OF_VM_CODE->otype = O_OpADD;
				HEAD_OF_VM_CODE->VMOp = VM_OP_TABLE(ADD);
			}
			return;
		case O_SUB:
			cell = cell->cdr;
			generate_VMCode(ctx, cell);
			while(cell->cdr) {
				cell = cell->cdr;
				generate_VMCode(ctx, cell);

				HEAD_OF_VM_CODE = add_VMCode(ctx);
				HEAD_OF_VM_CODE->otype = O_OpSUB;
				HEAD_OF_VM_CODE->VMOp = VM_OP_TABLE(SUB);
			}
			return;
		case O_MUL:
			cell = cell->cdr;
			generate_VMCode(ctx, cell);
			while(cell->cdr) {
				cell = cell->cdr;
				generate_VMCode(ctx, cell);

				HEAD_OF_VM_CODE = add_VMCode(ctx);
				HEAD_OF_VM_CODE->otype = O_OpMUL;
				HEAD_OF_VM_CODE->VMOp = VM_OP_TABLE(MUL);
			}
			return;
		case O_DIV:
			cell = cell->cdr;
			generate_VMCode(ctx, cell);
			while(cell->cdr) {
				cell = cell->cdr;
				generate_VMCode(ctx, cell);

				HEAD_OF_VM_CODE = add_VMCode(ctx);
				HEAD_OF_VM_CODE->otype = O_OpDIV;
				HEAD_OF_VM_CODE->VMOp = VM_OP_TABLE(DIV);
			}
			return;
		case O_LT:
			for(; i < 2; i++) {
				cell = cell->cdr;
				generate_VMCode(ctx, cell);
			}

			HEAD_OF_VM_CODE = add_VMCode(ctx);
			HEAD_OF_VM_CODE->otype = O_OpLT;
			HEAD_OF_VM_CODE->VMOp = VM_OP_TABLE(LT);
			return;
		case O_GT:
			for(; i < 2; i++) {
				cell = cell->cdr;
				generate_VMCode(ctx, cell);
			}

			HEAD_OF_VM_CODE = add_VMCode(ctx);
			HEAD_OF_VM_CODE->otype = O_OpGT;
			HEAD_OF_VM_CODE->VMOp = VM_OP_TABLE(GT);
			return;
		case O_LE:
			for(; i < 2; i++) {
				cell = cell->cdr;
				generate_VMCode(ctx, cell);
			}

			HEAD_OF_VM_CODE = add_VMCode(ctx);
			HEAD_OF_VM_CODE->otype = O_OpLE;
			HEAD_OF_VM_CODE->VMOp = VM_OP_TABLE(LE);
			return;
		case O_GE:
			for(; i < 2; i++) {
				cell = cell->cdr;
				generate_VMCode(ctx, cell);
			}

			HEAD_OF_VM_CODE = add_VMCode(ctx);
			HEAD_OF_VM_CODE->otype = O_OpGE;
			HEAD_OF_VM_CODE->VMOp = VM_OP_TABLE(GE);
			return;
		case O_EQ:
			for(; i < 2; i++) {
				cell = cell->cdr;
				generate_VMCode(ctx, cell);
			}

			HEAD_OF_VM_CODE = add_VMCode(ctx);
			HEAD_OF_VM_CODE->otype = O_OpEQ;
			HEAD_OF_VM_CODE->VMOp = VM_OP_TABLE(EQ);
			return;
		case O_IF:
			cell = cell->cdr;
			generate_VMCode(ctx, cell);
			HEAD_OF_VM_CODE = add_VMCode(ctx);
			HEAD_OF_VM_CODE->otype = O_OpCMP;
			HEAD_OF_VM_CODE->VMOp = VM_OP_TABLE(CMP);
			codeCMP = HEAD_OF_VM_CODE;
			cell = cell->cdr;
			generate_VMCode(ctx, cell);
			L1 = codeCMP->next;
			HEAD_OF_VM_CODE = add_VMCode(ctx);
			HEAD_OF_VM_CODE->otype = O_OpJMP;
			HEAD_OF_VM_CODE->VMOp = VM_OP_TABLE(JMP);
			codeJMP = HEAD_OF_VM_CODE;
			cell = cell->cdr;
			generate_VMCode(ctx, cell);
			L2 = codeJMP->next;
			L3 = HEAD_OF_VM_CODE;
			codeCMP->next = L1;
			codeCMP->jumpTo = L2;
			codeJMP->jumpTo = L3;
			return;
		case O_SETQ:
			cell = cell->cdr;
			symbol = cell->svalue;
			cell = cell->cdr;
			set_value(add_symbol(ctx, symbol), cell->ivalue);
			generate_VMCode(ctx, cell);
			return;
		case O_DEFUN:
			ctx->isDefun = 1;
			HEAD_OF_VM_CODE = new_VMCode(ctx);
			START_OF_VM_CODE = HEAD_OF_VM_CODE;
			HEAD_OF_VM_CODE->otype = O_OpFRAME;
			HEAD_OF_VM_CODE->VMOp = VM_OP_TABLE(FRAME);

			cell = cell->cdr;
			table = add_func(ctx, cell->svalue);
			table->otype = O_SymFUNC;
			cell = cell->cdr;
			argCell = cell->car;
			while(argCell) {
				set_arg(add_arg(ctx, argCell->svalue), i);
				i++;
				argCell = argCell->cdr;
			}
			cell = cell->cdr;
			generate_VMCode(ctx, cell);
			set_func(table, START_OF_VM_CODE);
			return;
		case O_STRING:
			compile_stringCells(ctx, cell);
			return;
	}
}

void compile(lcontext_t *ctx)
{
	generate_VMCode(ctx, TREE_ROOT);

	HEAD_OF_VM_CODE = add_VMCode(ctx);
	HEAD_OF_VM_CODE->otype = O_OpRET;
	HEAD_OF_VM_CODE->VMOp = VM_OP_TABLE(RET);
	return;
}
