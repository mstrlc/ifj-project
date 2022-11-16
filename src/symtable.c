#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../include/parser.h"
#include "../include/symtable.h"
#include "../include/lexer.h"


unsigned int hash(char *name, int size) {
    int hash = 0;
    for (size_t i = 0; i < strlen(name); i++) {
        hash += name[i];
    }
    return hash % size;
}

symbol_t *token_to_symbol(token_t *token) {

    symbol_t *symbol = malloc(sizeof(symbol_t));
    symbol->line = token->line;
    symbol->name = malloc(strlen(token->data) + 1);
    strcpy(symbol->name, token->data);
    symbol->type = token->type;
    symbol->next = NULL;
    symbol->prev = NULL;
    free(token->data);
    return symbol;
}

hash_table_t *hash_table_init(int size) {
    hash_table_t *table = malloc(sizeof(hash_table_t));
    table->size = size;
    table->count = 0;
    table->symbols = malloc(sizeof(symbol_t *) * size);
    for (int i = 0; i < size; i++) {
        table->symbols[i] = NULL;
    }
    return table;
}

void hash_table_free(hash_table_t *table) {
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
    free(table->symbols);
    free(table);
}

hash_table_t* resize(hash_table_t *table) {
    printf("sex");
    hash_table_t *new_table = hash_table_init(table->size * 2);
    for (size_t i = 0; i < table->size; i++) {
        symbol_t *symbol = table->symbols[i];
        if (symbol != NULL) {
            hash_table_insert(new_table, symbol);
            symbol_t *next = symbol->next;
            while (next != NULL) {
                printf("sex");
                hash_table_insert(new_table, next);
                next = next->next;
            }
        }
    }
    //hash_table_free(table);
    return new_table;
}

void hash_table_insert(hash_table_t *table, symbol_t *symbol) {
    int index = hash(symbol->name, table->size);
    symbol_t *current = table->symbols[index];
    if (current == NULL) {
        table->symbols[index] = symbol;
        table->count++;
    } else {
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = symbol;
        symbol->prev = current;
        table->count++;
    }
}

symbol_t *hash_table_lookup(hash_table_t *table, char *name) {
    int index = hash(name, table->size);
    symbol_t *symbol = table->symbols[index];
    while (symbol != NULL) {
        if (strcmp(symbol->name, name) == 0) {
            return symbol;
        }
        symbol = symbol->next;
    }
    return NULL;
}

void hash_table_print(hash_table_t *table) {
    printf("size: %ld\n", table->size);
    printf("%s\t%s\n","Index", "Name");
    for (size_t i = 0; i < table->size; i++) {
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
