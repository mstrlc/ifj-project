/**
 * @file main.c
 *
 * Main file of the compiler
 *
 * IFJ project 2022
 *
 * @author <xstrel03> Matyáš Strelec
 * @author <xseidl06> Ondřej Seidl
 * @author <xnovym00> Maxmilián Nový
 * @author <xklond00> Dominik Klon
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/parser.h"
#include "../include/symtable.h"
#include "../include/lexer.h"
#include "../include/stack.h"
#include "../include/error.h"
#include "../include/parse_tree.h"
#include "../include/exp_parser.h"

/**
 * @brief Main function of the compiler
 *
 * Calls lexical analyzer, then parser and code generation.
 *
 * @return Exit code - 0 if success, other if error -- see error.h
 */
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

    // Set active token to first token to start parsing
    ACTIVE_TOKEN = tokens->firstToken;

    // Prepare symtables for storing variables and functions in parser
    symtables_t *symtables = (symtables_t *)malloc(sizeof(struct symtables_type));
    symtables->vars_table = symtable_init(100);
    symtables->active_table_index = 0;
    symtables->function_table_index = 0;
    symtables->vars_table_array[symtables->active_table_index] = symtable_init(100);
    symtables->function_table = symtable_init(100);

    // First pass of parser, start generating code
    printf(".IFJcode22\n");
    // First pass doesn't have complete codegen, so skip it for now
    printf("JUMP first_pass_codegen_end\n");

    error = parser(tokens, symtables, 1);
    if (error != 0)
    {
        error_exit(error, tokens->activeToken);
        freeTokenList(tokens);
        exit(error);
    }

    // Set symtable indexes
    symtables->active_table_index = 0;
    symtables->function_table_index = 0;
    printf("LABEL first_pass_codegen_end\n");

    // Second pass of parser
    ACTIVE_TOKEN = tokens->firstToken;
    error = parser(tokens, symtables, 2);
    if (error != 0)
    {
        error_exit(error, tokens->activeToken);
        freeTokenList(tokens);
        exit(error);
    }

    // Print built-in functions at the end of the code
    printBuiltIn();
    free_symbols(symtables->vars_table);
    free_symbols(symtables->function_table);
    symtable_dispose(symtables->vars_table);
    symtable_dispose(symtables->function_table);
    freeTokenList(tokens);
    freeSymtables(symtables);
    exit(0);
}
