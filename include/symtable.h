#include "lexer.h"
#ifndef SYMTABLE_H
#define SYMTABLE_H

typedef struct symbol symbol_t;
typedef struct symbol {
    int line;
    char *name;
    token_type_t type;
    symbol_t *next;
    symbol_t *prev;
}symbol;

typedef struct symtable symtable_t;
struct symtable {
    size_t size;
    size_t count;
    symbol_t **symbols;
};

typedef struct symtables_type{
    symtable_t* vars_table;
    symtable_t* vars_table_array[1000];
    int actual_table_index; // oznacuje z jake tabulky se bude cist
    int function_table_index; // prirazuje indexy funkcim
} Symtables;

unsigned long hash(char *name);

symbol_t *token_to_symbol(token_t *token);

symtable_t *symtable_init(int size);
void free_symbols(symtable_t *table);
void symtable_dispose(symtable_t *table);

void symtable_insert(symtable_t *table, symbol_t *symbol);
symbol_t *symtable_lookup(symtable_t *table, char *name);

void symtable_print(symtable_t *table);
void symtable_defvar_print(symtable_t *table);

symtable_t* symtable_check_size(symtable_t *table);
symtable_t* resize(symtable_t *table);

#endif
