#include "lispo.h"

#define DATA_STACK(pos) ctx->dataStack[pos]

#define PUSH_INT(i) do {\
	pos++;\
	DATA_STACK(pos).dtype = D_INT;\
	DATA_STACK(pos).value = (i);\
} while(0)

#define POP_INT(i) do {\
	i = DATA_STACK(pos).value;\
	pos--;\
} while(0)

data_t *run_VM(lcontext_t *ctx)
{
	static void *VMOpTable[] = {
		&&OpPUSH, &&OpPOP,
		&&OpADD, &&OpSUB, &&OpMUL, &&OpDIV,
		&&OpCMP,
		&&OpLT, &&OpGT,
		&&OpCALL, &&OpRET,
		&&OpEND
	};

	if(!START_OF_VM_CODE) {
		ctx->VMOpTable = VMOpTable;
		return NULL;
	}

	int pos = -1;
	int r1, r2;
	VMCode *code = START_OF_VM_CODE;

	goto *code->VMOp;

OpPUSH:
	PUSH_INT(code->ivalue);
	code = code->next;
	goto *code->VMOp;

OpPOP:

OpADD:
	POP_INT(r1);
	POP_INT(r2);
	PUSH_INT(r1 + r2);
	code = code->next;
	goto *code->VMOp;

OpSUB:

OpMUL:

OpDIV:

OpCMP:

OpLT:

OpGT:

OpCALL:

OpRET:

OpEND:
	return &(DATA_STACK(pos));
}

void init_VMOpTable(lcontext_t *ctx)
{
	run_VM(ctx);
}
