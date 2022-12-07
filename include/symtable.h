/**
 * @file symtable.h
 * @author Ondřej Seidl xseidl06
 * @brief Interface of symbol table
 * IFJ project 2022
 */
#include <stdbool.h>
#include "lexer.h"
#ifndef SYMTABLE_H
#define SYMTABLE_H

//preparing types
typedef struct symbol symbol_t;
typedef struct symtable symtable_t;

// Symbol
typedef struct symbol {
    int line;
    char *name;
    token_type_t type;
    symbol_t *next;
    symbol_t *prev;
    token_type_t func_ret_type;
    bool func_is_defined;
    int func_param_count;
    bool var_is_initialized;
}symbol;

// Symbol table
struct symtable {
    size_t size;
    size_t count;
    symbol_t **symbols;
};

typedef struct symtables_type{
    symtable_t* vars_table;
    symtable_t* vars_table_array[1000];
    symtable_t* function_table; 
    int active_table_index; // marks active table to read from
    int function_table_index; // assigns indexes to functions in funcion table
 
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