#include "lexer.h"
#ifndef SYMTABLE_H
#define SYMTABLE_H

/* In hashing scheme, two tables are maintained – a hash table and symbol table and are the most commonly used method to implement symbol tables.
A hash table is an array with an index range: 0 to table size – 1. These entries are pointers pointing to the names of the symbol table.
To search for a name we use a hash function that will result in an integer between 0 to table size – 1.
Insertion and lookup can be made very fast – O(1).
The advantage is quick to search is possible and the disadvantage is that hashing is complicated to implement. */

typedef struct symtable_t symtable_t;

typedef struct symbol_item_t {
    int line;
    char *name;
    char *type;
    size_t size;
    symtable_t *scope;
    bool declared;
    bool defined;
    bool used;
} symbol_item_t;

struct symtable_t {
    symbol_item_t *entries;
    int count;
};

symtable_t* symtable_init();
void symtable_dispose(symtable_t *table);
symbol_item_t* symtable_set(symtable_t *table, token_t *token);
symbol_item_t* symtable_get(symtable_t *table, char *name);
void symtable_print(symtable_t *table);

#endif