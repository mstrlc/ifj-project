#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "symtable.h"

symtable_t* symtable_init() {
    symtable_t *table = malloc(sizeof(symtable_t));
    table->entries = NULL;
    table->count = 0;
    return table;
}

void symtable_dispose(symtable_t *table) {
    for (int i = 0; i < table->count; i++) {
        free(table->entries[i].name);
        free(table->entries[i].type);
    }
    free(table->entries);
    free(table);
}

symbol_item_t* token_to_item(token_t *token) {
    symbol_item_t *item;
    item = malloc(sizeof(symbol_item_t));
    if(item == NULL) {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        return NULL;
    }
    item->line = token->line;
    item->name = token->token;
    item->type = NULL;
    item->size = 0;
    item->scope = NULL;
    item->declared = false;
    item->defined = false;
    item->used = false;
    return item;
}

symbol_item_t* symtable_set(symtable_t *table, token_t *token) {
    symbol_item_t *item = token_to_item(token);
    if(item == NULL) {
        return NULL;
    }
    table->count++;
    table->entries = realloc(table->entries, table->count * sizeof(symbol_item_t));
    if(table->entries == NULL) {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        return NULL;
    }
    table->entries[table->count - 1] = *item;
    return &table->entries[table->count - 1];
}
void symtable_print(symtable_t *table) {
    for (int i = 0; i < table->count; i++) {
        printf("Name: %s, Line: %d, Declared: %d, Defined: %d, Used: %d\n", table->entries[i].name, table->entries[i].line, table->entries[i].declared, table->entries[i].defined, table->entries[i].used);
    }
}
