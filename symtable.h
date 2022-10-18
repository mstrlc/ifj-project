#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

//create a symbol table as hash table
typedef struct symtable {
    int key;
    char *data;
    struct symtable *next;
} symtable;

//create a symbol table as hash table
typedef struct symtable *symtable_ptr;

