#include "lispo.h"

#define DATA_STACK(pos) ctx->dataStack[pos]

#define PUSH_INT(i) do {\
	pos++;\
	DATA_STACK(pos).dtype = D_INT;\
	DATA_STACK(pos).value = (i);\
} while(0)

#define PUSH_BOOL(b) do {\
	pos++;\
	DATA_STACK(pos).dtype = D_BOOL;\
	DATA_STACK(pos).value = (b);\
} while(0)

#define POP_INT(i) do {\
	i = DATA_STACK(pos).value;\
	pos--;\
} while(0)

#define POP_BOOL(b) do {\
	b = DATA_STACK(pos).value;\
	pos--;\
} while(0)

data_t *run_VM(lcontext_t *ctx)
{
	static void *VMOpTable[] = {
		&&OpPUSH, &&OpPOP,
		&&OpADD, &&OpSUB, &&OpMUL, &&OpDIV,
		&&OpLT, &&OpGT,
		&&OpCMP,
		&&OpCALL, &&OpRET,
		&&OpEND
	};

	if(!START_OF_VM_CODE) {
		ctx->VMOpTable = VMOpTable;
		return NULL;
	}

	int pos = -1;
	register int r1, r2, r3;
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
	POP_INT(r1);
	POP_INT(r2);
	PUSH_INT(r1 - r2);
	code = code->next;
	goto *code->VMOp;

OpMUL:
	POP_INT(r1);
	POP_INT(r2);
	PUSH_INT(r1 * r2);
	code = code->next;
	goto *code->VMOp;

OpDIV:
	POP_INT(r1);
	POP_INT(r2);
	PUSH_INT(r1 / r2);
	code = code->next;
	goto *code->VMOp;

OpLT:
	POP_INT(r1);
	POP_INT(r2);
	PUSH_BOOL((r2 < r1)? T : NIL);
	code = code->next;
	goto *code->VMOp;

OpGT:
	POP_INT(r1);
	POP_INT(r2);
	PUSH_BOOL((r2 > r1)? T : NIL);
	code = code->next;
	goto *code->VMOp;

OpCMP:
	POP_INT(r1);
	POP_INT(r2);
	POP_BOOL(r3);
	PUSH_INT(r3? r2 : r1);
	code = code->next;
	goto *code->VMOp;

OpCALL:

OpRET:

OpEND:
	return &(DATA_STACK(pos));
}

void init_VMOpTable(lcontext_t *ctx)
{
	run_VM(ctx);
}
