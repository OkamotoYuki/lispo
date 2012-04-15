#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*========== macros ==========*/
#define TRUE                           1
#define FALSE                          0

#define END_OF_LINE                    -1



/*========== enum ==========*/
typedef enum cellType cellType;

enum cellType {
	EMPTY,
	START_BRACKET,
	NUM,
	ADD,
	SUB,
	MUL,
	DIV
};



/*========== struct ==========*/
typedef struct cons cons_t;
typedef struct memoryArena memoryArena_t;
typedef struct memoryPool memoryPool_t;
typedef struct lcontext lcontext_t;

struct cons {
	cellType type;
	cons_t *cdr;
	union {
		cons_t *car;
		int ivalue;
		char *svalue;
	};
};

struct memoryPool {
	int pos;
	cons_t *cells;
	memoryPool_t *next;
};

struct memoryArena {
	memoryPool_t *pool;
	int whichPool;
};

struct lcontext {
	FILE *fp; // for lisp file

	memoryArena_t *memoryArena; // for memory managiment

	int bracketsCounter; // for counting brackets

	cons_t *cellRoot; // for parser
	cons_t *cellHead;
	cons_t **startBracketCellsPtrStack;
	int startBracketCellsPtrStackPos;
};



/*========== function ==========*/
/* context.c */
extern lcontext_t *new_rootContext(int, char **);
extern void init_rootContext(lcontext_t *);
extern void free_rootContext(lcontext_t *);

/* memory.c */
extern memoryArena_t *new_memoryArena(void);
extern cons_t *allocate_consCell(lcontext_t *ctx);

/* read.c */
extern void read(lcontext_t *);

/* token.c */
extern char *skip_space(char *start);
extern int tokenize(char *);

/* parser.c */
extern int parser(lcontext_t *, char *, int);
