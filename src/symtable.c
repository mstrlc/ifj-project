#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../include/parser.h"
#include "../include/symtable.h"
#include "../include/lexer.h"


/**
 * @brief Hashing function
 * 
 * uses sdbm algorithm to calculate hash for given name
 * picked out from IJC project
 * 
 * @param name to be hashed
 * @return hash as unsigned long 
 */
unsigned long hash(char *name)
{
    unsigned long h = 0;
    unsigned char *p;
    for (p = (unsigned char *)name; *p != '\0'; p++)
        h = 65599 * h + *p;
    return h;
}

/**
 * @brief Converts token to symbol
 * 
 * @param token to be converted
 * @return symbol_t* pointer to symbol
 */
symbol_t *token_to_symbol(token_t *token)
{

    symbol_t *symbol = malloc(sizeof(symbol_t));
    symbol->name = malloc(strlen(token->data) + 1);
    if (symbol == NULL || symbol->name == NULL)
    {
        return NULL;
    }

    strcpy(symbol->name, token->data);

    symbol->line = token->line;
    symbol->type = token->type;
    symbol->next = NULL;
    symbol->prev = NULL;

    return symbol;
}

/**
 * @brief Initializes symbol table
 * 
 * Allocates memory for symbol table of given size
 * sets all pointers to NULL
 * 
 * @param size of table
 * 
 * @return symbol_table_t* pointer to symbol table
 */
symtable_t *symtable_init(int size)
{
    symtable_t *table = malloc(sizeof(symtable_t));
    if (table == NULL)
    {
        return NULL;
    }
    table->symbols = malloc(sizeof(symbol_t *) * size);
    if (table->symbols == NULL)
    {
        return NULL;
    }
    table->size = size;
    table->count = 0;
    for (int i = 0; i < size; i++)
    {
        table->symbols[i] = NULL;
    }
    return table;
}


/**
 * @brief Disposes symbol table
 * 
 * Frees symbol table and symbols structure
 * Not the individiual symbols
 * 
 * @param table 
 */
void symtable_dispose(symtable_t *table)
{
    free(table->symbols);
    free(table);
}


/**
 * @brief Frees all the symbols in the table
 * 
 * Individiual symbols are freed
 * 
 * @param table 
 */
void free_symbols(symtable_t *table)
{
    for (size_t i = 0; i < table->size; i++)
    {
        if (table->symbols[i] != NULL)
        {
            symbol_t *symbol = table->symbols[i]->next;
            while (symbol != NULL)
            {
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


/**
 * @brief Resizes symbol table
 * 
 * Creates new symbol table with double the size of the old one
 * Copies all the symbols from the old table to the new one
 * Frees the old table
 * 
 * @param table old table
 * @return symtable_t* new table
 */
symtable_t *resize(symtable_t *table)
{
    symtable_t *new_table = symtable_init(table->size * 2 + 1);
    for (size_t i = 0; i < table->size; i++)
    {
        if (table->symbols[i] != NULL)
        {
            symbol_t *symbol = table->symbols[i]->next;
            while (symbol != NULL)
            {
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

/**
 * @brief Inserts symbol into symbol table
 * 
 * @param symbol to be inserted
 * @param table to be inserted into 
 */
void symtable_insert(symtable_t *table, symbol_t *symbol)
{
    unsigned long index = hash(symbol->name) % table->size;
    symbol_t *current = table->symbols[index];
    if (current == NULL)
    {
        table->symbols[index] = symbol;
        table->symbols[index]->next = NULL;
        table->symbols[index]->prev = NULL;
        table->count++;
    }
    else
    {
        while (current->next != NULL)
        {
            current = current->next;
        }
        symbol->prev = current;
        symbol->next = NULL;
        current->next = symbol;
        table->count++;
    }
}

/**
 * @brief Checks the size of the table
 * 
 * If the table is more than 75% full, it is resized
 * to avoid collisions
 * 
 * @param table 
 * @return symtable_t* 
 */
symtable_t *symtable_check_size(symtable_t *table)
{
    float lf = (float)table->count / table->size;
    if (lf > 0.75)
    {
        return resize(table);
    }
    return table;
}


/**
 * @brief Finds symbol in the symbol table
 * 
 * @param table to search in
 * @param name as the key
 * @return symbol_t* or NULL if not found
 */
symbol_t *symtable_lookup(symtable_t *table, char *name)
{
    unsigned long index = hash(name);
    index = index % table->size;
    symbol_t *symbol = table->symbols[index];
    while (symbol != NULL)
    {
        if (strcmp(symbol->name, name) == 0)
        {
            return symbol;
        }
        symbol = symbol->next;
    }
    return NULL;
}

/**
 * @brief Prints symbol table
 * 
 * @param table 
 */
void symtable_print(symtable_t *table)
{
    printf("\nsize: %ld\n", table->size);
    printf("%s\t%s\n", "Index", "Name");
    for (unsigned long i = 0; i < table->size; i++)
    {
        symbol_t *symbol = table->symbols[i];
        if (symbol != NULL)
        {
            printf("%02ld | %02d: %s\t", i, symbol->line, symbol->name);
            if (strlen(symbol->name) < 8)
            {
                printf("\t\t");
            }
            else if (strlen(symbol->name) < 16)
            {
                printf("\t");
            }
            symbol = symbol->next;
            while (symbol != NULL)
            {
                printf("%02d: %s\t", symbol->line, symbol->name);
                if (strlen(symbol->name) < 8)
                {
                    printf("\t\t");
                }
                else if (strlen(symbol->name) < 16)
                {
                    printf("\t");
                }
                symbol = symbol->next;
            }
            // printf(" -> NULL \n");
            printf("\n");
        }
    }
}
