#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "symtable.h"

unsigned int hash(char *name, int size) {
    int hash = 0;
    for (size_t i = 0; i < strlen(name); i++) {
        hash += name[i];
    }
    return hash % size;
}



