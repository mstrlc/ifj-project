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

#define MAX_HTAB_LD 0.75f

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

    // Call parser
    error = parser(tokens);
    if (error != 0)
    {
        error_exit(error, tokens->activeToken);
        freeTokenList(tokens);
        exit(error);
    }
    else
    {
        printf("Parser success\n");
    }


    freeTokenList(tokens);
    printf("PROGRAM CORRECT\n");
    exit(error);
}
