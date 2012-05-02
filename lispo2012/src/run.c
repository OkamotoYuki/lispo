#include "lispo.h"

#define DATA_STACK(pos) ctx->dataStack[pos]

#define PUSH(i, d) do {\
	sp++;\
	DATA_STACK(sp).dtype = d;\
	DATA_STACK(sp).value = (i);\
} while(0)

#define PUSH_FP(fp) do {\
	sp++;\
	DATA_STACK(sp).dtype = D_FP;\
	DATA_STACK(sp).fp = (fp);\
} while(0)

#define PUSH_CODE(vmcode) do {\
	sp++;\
	DATA_STACK(sp).dtype = D_CODE;\
	DATA_STACK(sp).code = (vmcode);\
} while(0)

#define POP(i) do {\
	i = DATA_STACK(sp).value;\
	sp--;\
} while(0)

#define POP_FP(fp) do {\
	fp = DATA_STACK(sp).fp;\
	sp--;\
} while(0)

#define POP_CODE(code) do {\
	code = DATA_STACK(sp).code;\
	sp--;\
} while(0)

data_t *run_VM(lcontext_t *ctx)
{
	static void *VMOpTable[] = {
		&&OpPUSH, &&OpPOP,
		&&OpADD, &&OpSUB, &&OpMUL, &&OpDIV,
		&&OpLT, &&OpGT, &&OpLE, &&OpGE, &&OpEQ,
		&&OpCMP, &&OpJMP,
		&&OpFRAME, &&OpLOADA, &&OpCALL, &&OpPOPR, &&OpRET
	};

	if(!START_OF_VM_CODE) {
		ctx->VMOpTable = VMOpTable;
		return NULL;
	}

	int sp = -1, fp = -1, i;
	register int r1, r2;
	DataType dtype;
	VMCode *code = START_OF_VM_CODE;

	goto *code->VMOp;

OpPUSH:
	PUSH(code->ivalue, D_INT);
	code = code->next;
	goto *code->VMOp;

OpPOP:

OpADD:
	POP(r1);
	POP(r2);
	PUSH(r1 + r2, D_INT);
	code = code->next;
	goto *code->VMOp;

OpSUB:
	POP(r1);
	POP(r2);
	PUSH(r2 - r1, D_INT);
	code = code->next;
	goto *code->VMOp;

OpMUL:
	POP(r1);
	POP(r2);
	PUSH(r1 * r2, D_INT);
	code = code->next;
	goto *code->VMOp;

OpDIV:
	POP(r1);
	POP(r2);
	PUSH(r2 / r1, D_INT);
	code = code->next;
	goto *code->VMOp;

OpLT:
	POP(r1);
	POP(r2);
	PUSH((r2 < r1)? T : NIL, D_BOOL);
	code = code->next;
	goto *code->VMOp;

OpGT:
	POP(r1);
	POP(r2);
	PUSH((r2 > r1)? T : NIL, D_BOOL);
	code = code->next;
	goto *code->VMOp;

OpLE:
	POP(r1);
	POP(r2);
	PUSH((r2 <= r1)? T : NIL, D_BOOL);
	code = code->next;
	goto *code->VMOp;

OpGE:
	POP(r1);
	POP(r2);
	PUSH((r2 >= r1)? T : NIL, D_BOOL);
	code = code->next;
	goto *code->VMOp;

OpEQ:
	POP(r1);
	POP(r2);
	PUSH((r2 == r1)? T : NIL, D_BOOL);
	code = code->next;
	goto *code->VMOp;

OpCMP:
	POP(r1);
	if(r1) code = code->next;
	else code = code->jumpTo;
	goto *code->VMOp;

OpJMP:
	code = code->jumpTo;
	code = code->next;
	goto *code->VMOp;

OpFRAME:
	PUSH_FP(fp);
	fp = sp;
	code = code->next;
	goto *code->VMOp;

OpLOADA:
	PUSH(DATA_STACK(fp - 2 - code->index).value, D_INT);
	code = code->next;
	goto *code->VMOp;

OpCALL:
	PUSH_CODE(code->next);
	code = code->funcTable->func;
	goto *code->VMOp;

OpPOPR:
	dtype = DATA_STACK(sp).dtype;
	POP(r1);
	for(i = 0; i < code->numOfArgs; i++) {
		POP(r2);
	}
	PUSH(r1, dtype);
	code = code->next;
	goto *code->VMOp;

OpRET:
	if(fp <= 0) return &(DATA_STACK(sp));
	dtype = DATA_STACK(sp).dtype;
			POP(r1);
			POP_FP(fp);
			POP_CODE(code);
	PUSH(r1, dtype);
	goto *code->VMOp;
}

void init_VMOpTable(lcontext_t *ctx)
{
	run_VM(ctx);
}
