#include "lexer.h"
#ifndef SYMTABLE_H
#define SYMTABLE_H

typedef struct symbol symbol_t;

typedef struct symbol {
    int line;
    char *name;
    token_type_t type;
    symbol_t *next;
}symbol;

typedef struct hash_table hash_table_t;
struct hash_table {
    int size;
    int count;
    symbol_t **symbols;
};

unsigned int hash(char *name, int size);

symbol_t *token_to_symbol(token_t *token);
hash_table_t *hash_table_init(int size);
void hash_table_free(hash_table_t *table);
hash_table_t* resize(hash_table_t *table);
void hash_table_insert(hash_table_t *table, symbol_t *symbol);
symbol_t *hash_table_lookup(hash_table_t *table, char *name);
void hash_table_print(hash_table_t *table);

#endif
