#include "lispo.h"

#define DATA_STACK(pos) ctx->dataStack[pos]

#define PUSH_INT(i) do {\
	sp++;\
	DATA_STACK(sp).dtype = D_INT;\
	DATA_STACK(sp).value = (i);\
} while(0)

#define PUSH_BOOL(b) do {\
	sp++;\
	DATA_STACK(sp).dtype = D_BOOL;\
	DATA_STACK(sp).value = (b);\
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

#define POP_INT(i) do {\
	i = DATA_STACK(sp).value;\
	sp--;\
} while(0)

#define POP_BOOL(b) do {\
	b = DATA_STACK(sp).value;\
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
		&&OpLT, &&OpGT,
		&&OpCMP,
		&&OpFRAME, &&OpLOADA, &&OpCALL, &&OpPOPR, &&OpRET
	};

	if(!START_OF_VM_CODE) {
		ctx->VMOpTable = VMOpTable;
		return NULL;
	}

	int sp = -1, fp = -1, i;
	register int r1, r2, r3;
	VMCode *code = START_OF_VM_CODE;

	goto *code->VMOp;

OpPUSH:
	printf("%d\n", sp);
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
	PUSH_INT(r2 - r1);
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
	PUSH_INT(r2 / r1);
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

OpFRAME:
	PUSH_FP(fp);
	fp = sp;
	code = code->next;
	goto *code->VMOp;

OpLOADA:
	PUSH_INT(DATA_STACK(fp - 2 - code->index).value);
	code = code->next;
	goto *code->VMOp;

OpCALL:
	PUSH_CODE(code->next);
	code = code->jumpTo->func;
	goto *code->VMOp;

OpPOPR:
	switch(DATA_STACK(sp).dtype) {
		case D_INT:
			POP_INT(r1);
			for(i = 0; i < code->numOfArgs; i++) {
				POP_INT(r2);
			}
			PUSH_INT(r1);
			break;
		case D_BOOL:
			POP_BOOL(r1);
			for(i = 0; i < code->numOfArgs; i++) {
				POP_INT(r2);
			}
			PUSH_BOOL(r1);
			break;
	}
	code = code->next;
	goto *code->VMOp;

OpRET:
	if(fp <= 0) return &(DATA_STACK(sp));
	switch(DATA_STACK(sp).dtype) {
		case D_INT:
			POP_INT(r1);
			POP_FP(fp);
			POP_CODE(code);
			PUSH_INT(r1);
			break;
		case D_BOOL:
			POP_BOOL(r1);
			POP_FP(fp);
			POP_CODE(code);
			PUSH_BOOL(r1);
			break;
	}
	goto *code->VMOp;
}

void init_VMOpTable(lcontext_t *ctx)
{
	run_VM(ctx);
}
