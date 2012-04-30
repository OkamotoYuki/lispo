#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*========== macros ==========*/
#define T                              1
#define NIL                            0

#define MAX_LINE_LEN                   256
#define END_OF_LINE                    -1
#define QUIT                           -2

#define DEFAULT_SYMBOL_TABLE_SIZE      26
#define DEFAULT_ARG_TABLE_SIZE         4

#define TREE_ROOT                      ctx->treeRoot
#define TREE_HEAD                      ctx->treeHead

#define START_OF_VM_CODE               ctx->startOfVMCode
#define HEAD_OF_VM_CODE                ctx->headOfVMCode



/*========== enum ==========*/
typedef enum Errno Errno;
typedef enum ObjectType ObjectType;
typedef enum VMCodeType VMCodeType;
typedef enum SymbolType SymbolType;
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
	O_LT,
	O_GT,
	O_IF,
	O_SETQ,
	O_DEFUN,
	O_STRING,

	/* VM code */
	O_OpPUSH,
	O_OpPOP,
	O_OpADD,
	O_OpSUB,
	O_OpMUL,
	O_OpDIV,
	O_OpLT,
	O_OpGT,
	O_OpCMP,
	O_OpLOADA,
	O_OpCALL,
	O_OpRET,
	O_OpEND,

	/* symbol */
	O_SymVAL,
	O_SymFUNC,
	O_SymARG
};

enum VMCodeType {
	PUSH,
	POP,
	ADD,
	SUB,
	MUL,
	DIV,
	LT,
	GT,
	CMP,
	LOADA,
	CALL,
	RET,
	END
};

//enum SymbolType {
//	S_VALUE,
//	S_FUNC,
//	S_ARG,
//};

enum DataType {
	D_INT,
	D_BOOL,
	D_POS,
	D_CODE
};



/*========== struct ==========*/
typedef struct cons cons_t;
typedef struct VMCode VMCode;
typedef struct lObject lObject;
typedef struct memoryArena memoryArena_t;
typedef struct memoryPool memoryPool_t;
typedef struct hashTable hashTable_t;
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

struct hashTable {
	ObjectType otype;
	lObject *to; // TODO
	hashTable_t *next;
	char *symbol;
	union {
		int value;
		VMCode *func;
		int index;
	};
};

struct data {
	DataType dtype;
	union {
		int value;
		int pos;
		VMCode *code;
	};
};

struct lcontext {
	FILE *fp; // for lisp file

	memoryArena_t *memoryArena; // for memory managiment

	hashTable_t **symbolTable; // for symbol table
	hashTable_t ** argTable; // for arg table

	int bracketsCounter; // for counting brackets

	cons_t *treeRoot; // for parser
	cons_t *treeHead;
	cons_t **startBracketCellsPtrStack;
	int startBracketCellsPtrStackPos;
	int isFunc;
	int isStartOfArg;

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
extern VMCode *new_VMCode(lcontext_t *);
extern hashTable_t *new_argTable(lcontext_t *);

/* hash.c */
extern hashTable_t *search_symbol(lcontext_t *, char *);
extern hashTable_t *add_symbol(lcontext_t *, char *);
extern inline void set_value(hashTable_t *, int);
extern inline void set_func(hashTable_t *, VMCode *);
extern hashTable_t *search_arg(lcontext_t *, char *);
extern hashTable_t *add_arg(lcontext_t *, char *);
extern inline void set_arg(hashTable_t *, int);

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
