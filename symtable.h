#include "lexer.h"
#ifndef SYMTABLE_H
#define SYMTABLE_H

/* In hashing scheme, two tables are maintained – a hash table and symbol table and are the most commonly used method to implement symbol tables.
A hash table is an array with an index range: 0 to table size – 1. These entries are pointers pointing to the names of the symbol table.
To search for a name we use a hash function that will result in an integer between 0 to table size – 1.
Insertion and lookup can be made very fast – O(1).
The advantage is quick to search is possible and the disadvantage is that hashing is complicated to implement. */


typedef struct symbol symbol_t;

typedef struct symbol {
    int line;
    char *name;
    char *type;
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