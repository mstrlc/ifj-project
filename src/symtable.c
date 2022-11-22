#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../include/parser.h"
#include "../include/symtable.h"
#include "../include/lexer.h"


unsigned long hash(char *name) {
    unsigned long h=0;
    unsigned char *p;
    for(p=(unsigned char*)name; *p!='\0'; p++)
        h = 65599*h + *p;
    return h;
}

symbol_t *token_to_symbol(token_t *token) {

    symbol_t *symbol = malloc(sizeof(symbol_t));
    symbol->name = malloc(strlen(token->data) + 1);
    if(symbol == NULL || symbol->name == NULL) {
        return NULL;
    }

    strcpy(symbol->name, token->data);
    free(token->data);

    symbol->line = token->line;
    symbol->type = token->type;
    symbol->next = NULL;
    symbol->prev = NULL;
    
    return symbol;
}

symtable_t *symtable_init(int size) {
    symtable_t *table = malloc(sizeof(symtable_t));
    if(table == NULL) {
        return NULL;
    }
    table->symbols = malloc(sizeof(symbol_t *) * size);
    if(table->symbols == NULL) {
        return NULL;
    }
    table->size = size;
    table->count = 0;
    for (int i = 0; i < size; i++) {
        table->symbols[i] = NULL;
    }
    return table;
}

void symtable_dispose(symtable_t *table) {
    free(table->symbols);
    free(table);
}

void free_symbols(symtable_t *table) {
    for (size_t i = 0; i < table->size; i++) {
        if (table->symbols[i] != NULL) {
            symbol_t *symbol = table->symbols[i]->next;
            while (symbol != NULL) {
                symbol_t *temp = symbol;
                symbol = symbol->next;
                free(temp->name);
                free(temp);
            }
            free(table->symbols[i]->name);
            free(table->symbols[i]);

        }
    }
}

symtable_t* resize(symtable_t *table) {
    symtable_t *new_table = symtable_init(table->size * 2 + 1);
    for (size_t i = 0; i < table->size; i++) {
        if (table->symbols[i] != NULL) {
            symbol_t *symbol = table->symbols[i]->next;
            while (symbol != NULL) {
                symbol_t *temp = symbol;
                symbol = symbol->next;
                symtable_insert(new_table, temp);
            }
            symtable_insert(new_table, table->symbols[i]);
        }
    }
    symtable_dispose(table);
    return new_table;
}

void symtable_insert(symtable_t *table, symbol_t *symbol) {
    unsigned long index = hash(symbol->name) % table->size;
    symbol_t *current = table->symbols[index];
    if (current == NULL) {
        table->symbols[index] = symbol;
        table->symbols[index]->next = NULL;
        table->symbols[index]->prev = NULL;
        table->count++;
    } else {
        while (current->next != NULL) {
            current = current->next;
        }
        symbol->prev = current;
        symbol->next = NULL;
        current->next = symbol;
        table->count++;
    }
}

symtable_t* symtable_check_size(symtable_t *table) {
    float lf = (float)table->count / table->size;
    if (lf > 0.75) {
        return resize(table);
    }
    return table;
}

symbol_t *symtable_lookup(symtable_t *table, char *name) {
    unsigned long index = hash(name);
    index = index % table->size;
    symbol_t *symbol = table->symbols[index];
    while (symbol != NULL) {
        if (strcmp(symbol->name, name) == 0) {
            return symbol;
        }
        symbol = symbol->next;
    }
    return NULL;
}

void symtable_print(symtable_t *table) {
    printf("\nsize: %ld\n", table->size);
    printf("%s\t%s\n","Index", "Name");
    for (unsigned long i = 0; i < table->size; i++) {
        symbol_t *symbol = table->symbols[i];
        if (symbol != NULL) {
            printf("%ld\t%s l:%d\t", i, symbol->name, symbol->line);
            symbol = symbol->next;
            while (symbol != NULL) {
                printf("%s l:%d\t", symbol->name, symbol->line);
                symbol = symbol->next;
            }
            printf(" -> NULL \n");
        }
    }
}
