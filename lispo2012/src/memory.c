#include "lispo.h"

#define DEFAULT_NUMBER_OF_CELLS        1024
//#define DEFAULT_NUMBER_OF_CELLS        5

static memoryPool_t *init_memoryPool(memoryPool_t *pool)
{
	pool->pos = 0;
	pool->objs = (lObject *)malloc(sizeof(lObject) * DEFAULT_NUMBER_OF_CELLS);
	pool->next = NULL;
	return pool;
}

static void free_memoryPool(memoryPool_t *pool)
{
	int i;

	for(i = 0; i < DEFAULT_NUMBER_OF_CELLS; i++) {
		if(pool->objs[i].otype == O_STRING) {
			free(pool->objs[i].svalue);
		}
	}
}

memoryArena_t *new_memoryArena(void)
{
	memoryArena_t *arena = (memoryArena_t *)malloc(sizeof(memoryPool_t));
	arena->pool = (memoryPool_t *)malloc(sizeof(memoryPool_t) * 2);
	init_memoryPool(&(arena->pool[0]));
	init_memoryPool(&(arena->pool[1]));
	arena->whichPool = 0;
	return arena;
}

void free_memoryArena(memoryArena_t *arena)
{
	free_memoryPool(&(arena->pool[0]));
	free_memoryPool(&(arena->pool[1]));
	free(arena->pool);
	free(arena);
}

static lObject *copy_lObject(lcontext_t *ctx, lObject *head)
{
	if(!head) return NULL;

	lObject *from = head;
	memoryArena_t *arena = ctx->memoryArena;
	memoryPool_t *pool = &(arena->pool[!arena->whichPool]);
	lObject *to = &(pool->objs[pool->pos]);
	memcpy(to, from, sizeof(lObject));
	pool->pos++;

	if(from->otype == O_START_BRACKET) to->car = copy_lObject(ctx, from->car);
	to->cdr = copy_lObject(ctx, from->cdr);
	return to;
}

static memoryPool_t *gc(lcontext_t *ctx)
{
	memoryArena_t *arena = ctx->memoryArena;
	TREE_ROOT = (cons_t *)copy_lObject(ctx, (lObject *)TREE_ROOT);
	memoryPool_t *fromPool = &(arena->pool[arena->whichPool]);

	while(1) {
		fromPool->pos = 0;
		if(fromPool->next) fromPool = fromPool->next;
		else break;
	}
	arena->whichPool = (arena->whichPool + 1) % 2;
	return &(arena->pool[arena->whichPool]);
}

static void init_lObject(lObject *o)
{
	if(o->otype == O_STRING) free(o->svalue);
	o->otype = O_EMPTY;
	o->cdr = NULL;
}

static lObject *new_lObject(lcontext_t *ctx)
{
	lObject *o;
	memoryArena_t *arena = ctx->memoryArena;
	memoryPool_t *pool = &(arena->pool[arena->whichPool]);
	int pos = pool->pos;

	while(pos == DEFAULT_NUMBER_OF_CELLS) {
		if(pool->next) {
			pool = pool->next;
		}
		else {
			pool = gc(ctx);
		}
		pos = pool->pos;
	}

	o = &(pool->objs[pos]);
	init_lObject(o);
	pool->pos++;
	return o;
}

cons_t *new_consCell(lcontext_t *ctx)
{
	return (cons_t *)new_lObject(ctx);
}

VMCode *new_VMCode(lcontext_t *ctx)
{
	return (VMCode *)new_lObject(ctx);
}

hashTable_t *new_argTable(lcontext_t *ctx)
{
	return (hashTable_t *)new_lObject(ctx);
}
