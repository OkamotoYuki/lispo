#include "lispo.h"

#define HASH_TABLE(hash) ctx->hashTable[hash]
#define hash(s) (*s % MAX_NUMBER_OF_ALPHABET)

hashTable_t *search_symbol(lcontext_t *ctx, char *symbol)
{
	hashTable_t *table = HASH_TABLE(hash(symbol));

	while(table) {
		if(!strncmp(symbol, table->symbol, strlen(symbol))) return table;
		table = table->next;
	}
	return NULL;
}

hashTable_t *add_symbol(lcontext_t *ctx, char *symbol)
{
	int hash = hash(symbol);
	int size = strlen(symbol);
	hashTable_t *table = HASH_TABLE(hash);

	if(!table) {
		HASH_TABLE(hash) = (hashTable_t *)malloc(sizeof(hashTable_t));
		table = HASH_TABLE(hash);
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
	table->stype = S_VALUE;
	table->value = value;
}
