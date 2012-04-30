#include "lispo.h"

#define VM_OP_TABLE(op) ctx->VMOpTable[op]

static void generate_VMCode(lcontext_t *ctx, cons_t *cell)
{
	if(!cell) return;

	int i = 0;
	char *symbol;
	hashTable_t *table;

	switch(cell->otype) {
		case O_START_BRACKET:
			generate_VMCode(ctx, cell->car);
			return;
		case O_NUM:
			if(!HEAD_OF_VM_CODE) {
				HEAD_OF_VM_CODE = new_VMCode(ctx);
				START_OF_VM_CODE = HEAD_OF_VM_CODE;
			}
			else {
				HEAD_OF_VM_CODE->next = new_VMCode(ctx);
				HEAD_OF_VM_CODE = HEAD_OF_VM_CODE->next;
			}
			HEAD_OF_VM_CODE->otype = O_OpPUSH;
			HEAD_OF_VM_CODE->VMOp = VM_OP_TABLE(PUSH);
			HEAD_OF_VM_CODE->dtype = D_INT;
			HEAD_OF_VM_CODE->ivalue = cell->ivalue;
			return;
		case O_ADD:
			cell = cell->cdr;
			generate_VMCode(ctx, cell);
			while(cell->cdr) {
				cell = cell->cdr;
				generate_VMCode(ctx, cell);
				HEAD_OF_VM_CODE->next = new_VMCode(ctx);
				HEAD_OF_VM_CODE = HEAD_OF_VM_CODE->next;
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
				HEAD_OF_VM_CODE->next = new_VMCode(ctx);
				HEAD_OF_VM_CODE = HEAD_OF_VM_CODE->next;
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
				HEAD_OF_VM_CODE->next = new_VMCode(ctx);
				HEAD_OF_VM_CODE = HEAD_OF_VM_CODE->next;
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
				HEAD_OF_VM_CODE->next = new_VMCode(ctx);
				HEAD_OF_VM_CODE = HEAD_OF_VM_CODE->next;
				HEAD_OF_VM_CODE->otype = O_OpDIV;
				HEAD_OF_VM_CODE->VMOp = VM_OP_TABLE(DIV);
			}
			return;
		case O_LT:
			for(; i < 2; i++) {
				cell = cell->cdr;
				generate_VMCode(ctx, cell);
			}
			HEAD_OF_VM_CODE->next = new_VMCode(ctx);
			HEAD_OF_VM_CODE = HEAD_OF_VM_CODE->next;
			HEAD_OF_VM_CODE->otype = O_OpLT;
			HEAD_OF_VM_CODE->VMOp = VM_OP_TABLE(LT);
			return;
		case O_GT:
			for(; i < 2; i++) {
				cell = cell->cdr;
				generate_VMCode(ctx, cell);
			}
			HEAD_OF_VM_CODE->next = new_VMCode(ctx);
			HEAD_OF_VM_CODE = HEAD_OF_VM_CODE->next;
			HEAD_OF_VM_CODE->otype = O_OpGT;
			HEAD_OF_VM_CODE->VMOp = VM_OP_TABLE(GT);
			return;
		case O_IF:
			for(; i < 3; i++) {
				cell = cell->cdr;
				generate_VMCode(ctx, cell);
			}
			HEAD_OF_VM_CODE->next = new_VMCode(ctx);
			HEAD_OF_VM_CODE = HEAD_OF_VM_CODE->next;
			HEAD_OF_VM_CODE->otype = O_OpCMP;
			HEAD_OF_VM_CODE->VMOp = VM_OP_TABLE(CMP);
			return;
		case O_SETQ:
			cell = cell->cdr;
			symbol = cell->svalue;
			cell = cell->cdr;
			set_value(add_symbol(ctx, symbol), cell->ivalue);
			generate_VMCode(ctx, cell);
			return;
		case O_STRING:
			table = search_symbol(ctx, cell->svalue);
			if(!table) return; // TODO
			switch(table->otype) {
				case O_SymVAL:
					cell->otype = O_NUM;
					cell->ivalue = table->value;
					generate_VMCode(ctx, cell);
					break;
				case O_SymFUNC:
				case O_SymARG:
				default:
					break;
			}
			return;
	}
}

void compile(lcontext_t *ctx)
{
	generate_VMCode(ctx, TREE_ROOT);
	HEAD_OF_VM_CODE->next = new_VMCode(ctx);
	HEAD_OF_VM_CODE = HEAD_OF_VM_CODE->next;
	HEAD_OF_VM_CODE->otype = O_OpEND;
	HEAD_OF_VM_CODE->VMOp = VM_OP_TABLE(END);
	return;
}
