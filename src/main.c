#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>

#include "../include/parser.h"
#include "../include/symtable.h"
#include "../include/lexer.h"
#include "../include/stack.h"
#include "../include/common.h"
#include "../include/error.h"

int main()
{
    int error = 0;
    // Initialize token list and fill it with tokens from stdin
    token_list_t *tokens = malloc(sizeof(token_list_t));

    // Call lexer
    error = fillTokenList(tokens);
    if (error != 0)
    {
        error_exit(error, tokens->activeToken);
        freeTokenList(tokens);
        exit(error);
    }
    else
    {
        printf("Lexer success\n");
    }

    // Initialize symbol table
    symtable_t *symtable = symtable_init(19);
    symtable = symtable_check_size(symtable);

    // Fill symbol table with tokens
    for (; tokens->activeToken->type != T_File_end; tokens->activeToken = tokens->activeToken->next)
    {
        if (tokens->activeToken->type == T_Identifier ||
            tokens->activeToken->type == T_Var_id ||
            tokens->activeToken->type == T_String ||
            tokens->activeToken->type == T_Int ||
            tokens->activeToken->type == T_Float)
        {
            symtable_insert(symtable, token_to_symbol(tokens->activeToken));
        }
    }
    symtable_print(symtable);

    void *aa = symtable_lookup(symtable, "$d");
    printf("aa: %p\n\n", aa);

    tokens->activeToken = tokens->firstToken;

    //Prepare symtables
    Symtables* symtables = (Symtables*)malloc(sizeof(struct symtables_type));
    symtables -> vars_table = symtable_init(100);
    symtables -> func_table = symtable_init(100);
    symtables -> vars_table_index = 0;
    symtables -> function_table_index = 0;
    symtables -> vars_table_array[symtables -> vars_table_index] = symtable_init(100);

    // Call parser
    // nejdebilnejsi ale funkci reseni, prvni pruchod nema kompletni codegen, tak ho skipneme, misto toho, abysme ho negenerovali vubec
    printf(".IFJcode22\n");
    printf("JUMP end_of_this_scuffed_codegen\n");
    error = parser(tokens, symtables);
    symtables -> vars_table_index = 0;
    symtables -> function_table_index = 0;
    printf("LABEL end_of_this_scuffed_codegen\n");
    ACTIVE_TOKEN = tokens->firstToken;


    error = parser(tokens, symtables);
    if (error != 0)
    {
        error_exit(error, tokens->activeToken);
        freeTokenList(tokens);
        exit(error);
    }
    else
    {
        printf("\n\nParser success\n");
    }


    freeTokenList(tokens);
    printf("PROGRAM CORRECT\n");
    exit(error);
}
