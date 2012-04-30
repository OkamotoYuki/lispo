#include "lispo.h"

#define SYMBOL_TABLE(hash) ctx->symbolTable[hash]
#define largeHash(s) (*s % DEFAULT_SYMBOL_TABLE_SIZE)

#define ARG_TABLE(hash) ctx->argTable[hash]
#define smallHash(s) (*s % DEFAULT_ARG_TABLE_SIZE)

hashTable_t *search_symbol(lcontext_t *ctx, char *symbol)
{
	hashTable_t *table = SYMBOL_TABLE(largeHash(symbol));

	while(table) {
		if(!strncmp(symbol, table->symbol, strlen(symbol))) return table;
		table = table->next;
	}
	return NULL;
}

hashTable_t *add_symbol(lcontext_t *ctx, char *symbol)
{
	int hash = largeHash(symbol);
	int size = strlen(symbol);
	hashTable_t *table = SYMBOL_TABLE(hash);

	if(!table) {
		SYMBOL_TABLE(hash) = (hashTable_t *)malloc(sizeof(hashTable_t));
		table = SYMBOL_TABLE(hash);
		table->symbol = (char *)malloc(size + 1);
		memcpy(table->symbol, (void *)symbol, size);
		table->symbol[size] = '\0';
		return table;
	}

	while(table) {
		if(!strncmp(symbol, table->symbol, strlen(symbol))) return table;
		table = table->next;
	}
	table = (hashTable_t *)malloc(sizeof(hashTable_t));
	table->symbol = (char *)malloc(size + 1);
	memcpy(table->symbol, (void *)symbol, size);
	table->symbol[size] = '\0';
	return table;
}

inline void set_value(hashTable_t *table, int value)
{
	table->otype = O_SymVAL;
	table->value = value;
}

inline void set_func(hashTable_t *table, VMCode *startOfFunc)
{
	table->otype = O_SymFUNC;
	table->func = startOfFunc;
}

hashTable_t *search_arg(lcontext_t *ctx, char *symbol)
{
	hashTable_t *table = ARG_TABLE(smallHash(symbol));

	while(table) {
		if(!strncmp(symbol, table->symbol, strlen(symbol))) return table;
		table = table->next;
	}
	return NULL;
}

hashTable_t *add_arg(lcontext_t *ctx, char *symbol)
{
	int hash = smallHash(symbol);
	int size = strlen(symbol);
	hashTable_t *table = ARG_TABLE(hash);

	if(!table) {
		ARG_TABLE(hash) = new_argTable(ctx);;
		table = ARG_TABLE(hash);
		table->symbol = (char *)malloc(size + 1);
		memcpy(table->symbol, (void *)symbol, size);
		table->symbol[size] = '\0';
		return table;
	}

	while(table) {
		if(!strncmp(symbol, table->symbol, strlen(symbol))) return table;
		table = table->next;
	}
	table = new_argTable(ctx);
	table->symbol = (char *)malloc(size + 1);
	memcpy(table->symbol, (void *)symbol, size);
	table->symbol[size] = '\0';
	return table;
}

inline void set_arg(hashTable_t *table, int index)
{
	table->otype = O_SymVAL;
	table->index = index;
}
