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
#include "../include/parse_tree.h"
#include "../include/exp_parser.h"

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
        // printf("Lexer success\n");
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
    // symtable_print(symtable);

    // void *aa = symtable_lookup(symtable, "$d");
    // printf("aa: %p\n\n", aa);

    tokens->activeToken = tokens->firstToken;

    //Prepare symtables
    Symtables* symtables = (Symtables*)malloc(sizeof(struct symtables_type));
    symtables -> vars_table = symtable_init(100);
    symtables -> active_table_index = 0;
    symtables -> function_table_index = 0;
    symtables -> vars_table_array[symtables -> active_table_index] = symtable_init(100);
    symtables -> function_table = symtable_init(100);

    // PRVNI PRUCHOD
    printf(".IFJcode22\n");
    printf("JUMP end_of_this_scuffed_codegen\n"); // nejdebilnejsi ale funkci reseni, prvni pruchod nema kompletni codegen, tak ho skipneme, misto toho, abysme ho negenerovali vubec
    error = parser(tokens, symtables, 1);
    if (error != 0)
    {
        error_exit(error, tokens->activeToken);
        freeTokenList(tokens);
        exit(error);
    }
    symtables -> active_table_index = 0;
    symtables -> function_table_index = 0;
    printf("LABEL end_of_this_scuffed_codegen\n");

    ACTIVE_TOKEN = tokens->firstToken;
    // DRUHY PRUCHOD
    error = parser(tokens, symtables, 2);
    if (error != 0)
    {
        error_exit(error, tokens->activeToken);
        freeTokenList(tokens);
        exit(error);
    }

    // PRINT VESTAVENYCH FUNKCI
    printf("JUMP end_of_program\n");
    FILE    *textfile;
    char    line[1000];
     
    textfile = fopen("src/inbuilt.ifjc22", "r");
    if(textfile == NULL)
        return 1;
     
    while(fgets(line, 1000, textfile)){
        printf(line);
    }
    printf("LABEL end_of_program\n");
     
    fclose(textfile);

    //ERROR HANDLE
    if (error != 0)
    {
        error_exit(error, tokens->activeToken);
        freeTokenList(tokens);
        exit(error);
    }
    else
    {
        // printf("\n\nParser success\n");
    }

    freeTokenList(tokens);
    // printf("\n\n\nPROGRAM CORRECT\n");
    exit(error);
}