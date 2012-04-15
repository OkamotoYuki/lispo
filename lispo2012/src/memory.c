#include "lispo.h"

//#define DEFAULT_NUMBER_OF_CELLS        1024
#define DEFAULT_NUMBER_OF_CELLS        5

static memoryPool_t *init_memoryPool(memoryPool_t *pool)
{
	pool->pos = 0;
	pool->cells = (cons_t *)malloc(sizeof(cons_t) * DEFAULT_NUMBER_OF_CELLS);
	pool->next = NULL;
	return pool;
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

static cons_t *copy_cell(lcontext_t *ctx, cons_t *head)
{
	if(!head) return NULL;

	cons_t *from = head;
	memoryArena_t *arena = ctx->memoryArena;
	memoryPool_t *pool = &(arena->pool[!arena->whichPool]);
	cons_t *to = &(pool->cells[pool->pos]);
	memcpy(to, from, sizeof(cons_t));
	pool->pos++;

	if(from->type == START_BRACKET) to->car = copy_cell(ctx, from->car);
	to->cdr = copy_cell(ctx, from->cdr);
	return to;
}

static memoryPool_t *gc(lcontext_t *ctx)
{
	cons_t *head = ctx->cellRoot;
	memoryArena_t *arena = ctx->memoryArena;
	head = copy_cell(ctx, head);
	memoryPool_t *fromPool = &(arena->pool[arena->whichPool]);
	while(1) {
		fromPool->pos = 0;
		if(fromPool->next) fromPool = fromPool->next;
		else break;
	}
	arena->whichPool = (arena->whichPool + 1) % 2;
	return &(arena->pool[arena->whichPool]);
}

static void init_cell(cons_t *cell)
{
	cell->type = EMPTY;
	cell->cdr = NULL;
}

cons_t *allocate_consCell(lcontext_t *ctx)
{
	cons_t *cell;
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

	cell = &(pool->cells[pos]);
	init_cell(cell);
	pool->pos++;
	return cell;
}
