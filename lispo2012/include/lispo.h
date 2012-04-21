#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*========== macros ==========*/
#define T                              1
#define NIL                            0

#define MAX_LINE_LEN                   256
#define END_OF_LINE                    -1
#define QUIT                           -2

#define TREE_ROOT                      ctx->treeRoot
#define TREE_HEAD                      ctx->treeHead

#define START_OF_VM_CODE               ctx->startOfVMCode
#define HEAD_OF_VM_CODE                ctx->headOfVMCode



/*========== enum ==========*/
typedef enum Errno Errno;
typedef enum ObjectType ObjectType;
typedef enum VMCodeType VMCodeType;
typedef enum DataType DataType;

enum Errno {
	ARG_ERROR,
	FILE_OPEN_ERROR,
	SYNTAX_ERROR
};

enum ObjectType {
	/* cons cell */
	O_EMPTY,
	O_START_BRACKET,
	O_NUM,
	O_ADD,
	O_SUB,
	O_MUL,
	O_DIV,
	O_STRING,
	O_IF,
	O_SETQ,
	O_DEFUN,

	/* VM code */
	O_OpPUSH,
	O_OpPOP,
	O_OpADD,
	O_OpSUB,
	O_OpMUL,
	O_OpDIV,
	O_OpCONDJMP,
	O_OpLTHAN,
	O_OpGTHAN,
	O_OpCALL,
	O_OpRET,
	O_OpEND
};

enum VMCodeType {
	PUSH,
	POP,
	ADD,
	SUB,
	MUL,
	DIV,
	CONDJMP,
	LTHAN,
	GTHAN,
	CALL,
	RET,
	END
};

enum DataType {
	D_INT,
	D_BOOL
};



/*========== struct ==========*/
typedef struct cons cons_t;
typedef struct VMCode VMCode;
typedef struct lObject lObject;
typedef struct memoryArena memoryArena_t;
typedef struct memoryPool memoryPool_t;
typedef struct data data_t;
typedef struct lcontext lcontext_t;

struct cons {
	ObjectType otype;
	lObject *to; // TODO
	cons_t *cdr;
	union {
		cons_t *car;
		int ivalue;
		char *svalue;
	};
};

struct VMCode {
	ObjectType otype;
	lObject *to; // TODO
	VMCode *next;
	void *VMOp;

	DataType dtype;
	int ivalue;
};

struct lObject {
	ObjectType otype;
	lObject *to; // TODO
	lObject *cdr;
	union {
		lObject *car;
		int ivalue;
		char *svalue;
	};

	char v1[4];
	char v2[4];
};

struct memoryPool {
	int pos;
	lObject *objs;
	memoryPool_t *next;
};

struct memoryArena {
	memoryPool_t *pool;
	int whichPool;
};

struct data {
	DataType dtype;
	int value;
};

struct lcontext {
	FILE *fp; // for lisp file

	memoryArena_t *memoryArena; // for memory managiment

	int bracketsCounter; // for counting brackets

	cons_t *treeRoot; // for parser
	cons_t *treeHead;
	cons_t **startBracketCellsPtrStack;
	int startBracketCellsPtrStackPos;

	void **VMOpTable; // for VM
	VMCode *startOfVMCode;
	VMCode *headOfVMCode;

	data_t *dataStack; // for data stack
};



/*========== function ==========*/
/* error.c */
extern void lstrerr(Errno);

/* context.c */
extern lcontext_t *new_rootContext(int, char **);
extern void init_rootContext(lcontext_t *);
extern void free_rootContext(lcontext_t *);

/* memory.c */
extern memoryArena_t *new_memoryArena(void);
extern cons_t *new_consCell(lcontext_t *);
//extern char *new_string(lcontext_t *, int);
extern VMCode *new_VMCode(lcontext_t *);

/* read.c */
extern void read(lcontext_t *);

/* token.c */
extern char *skip_space(char *);
extern int tokenize(char *);

/* parser.c */
extern int parse(lcontext_t *, char *, int);

/* codegen.c */
extern void compile(lcontext_t *);

/* run.c */
extern data_t *run_VM(lcontext_t *);
extern void init_VMOpTable(lcontext_t *);

/* print.c */
extern void print_result(data_t *);

/* dbg.c */
extern void print_consTree(cons_t *);
extern void print_VMCode(VMCode *);



/*========== global valiable ==========*/
extern int depth;
