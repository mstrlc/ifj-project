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

#define MAX_HTAB_LD 0.75f

int main()
{
    // token_t *tokens[1000];

    // for (int i = 0; i < 1000; i++)
    // {
    //     tokens[i] = malloc(sizeof(token_t));
    // }
    // // Go through tokens until End of file token type
    // int i = 0;
    // while (true)
    // {
    //     getNextToken(tokens[i]);
    //     printf("%d:\t", tokens[i]->line)    ;
    //     printf("%s\t", typeToString(tokens[i]->type));
    //     if(strlen(typeToString(tokens[i]->type))>7)
    //         printf("\t");
    //     switch (tokens[i]->type)
    //     {
    //         case T_Identifier:
    //             printf("Identifier: %s\n", tokens[i]->data);
    //             break;
    //         case T_Int:
    //             printf("Int: %s\n", tokens[i]->data);
    //             break;
    //         case T_Float:
    //             printf("Float: %s\n", tokens[i]->data);
    //             break;
    //         case T_Exp:
    //             printf("Exp: %s\n", tokens[i]->data);
    //             break;
    //         case T_String:
    //             printf("String: %s\n", tokens[i]->data);
    //             break;
    //         default:
    //             printf("\n");
    //             break;
    //     }

    //     if (tokens[i]->type == T_File_end)
    //     {
    //         i++;
    //         break;
    //     }
    //     i++;
    // }

    // //PARSE DEBUG
    // rewind(stdin);

    // b_stack* stack = (b_stack *) malloc(sizeof(struct b_stack)) ;
    // b_stack_init(stack);

    // token_t *tokens_p[1000];
    // for (int a = 0; a < 1000; a++)
    // {
    //     tokens_p[a] = malloc(sizeof(token_t));
    // }
    // // Go through tokens until End of file token type
    // int a = 0;
    // while(true){
    //     getNextToken(tokens_p[a]);

    //     if(prog(tokens_p[a], stack) == 1){
    //         printf("\n\nERROR ON LINE: %d", tokens_p[a] -> line ); //nefunguje , protoze iteruju po druhe, ale kdyz zakomentuju lexer debug tak funguje
    //         printf("\n\nTHE ERROR IS NEAR: %s\n", tokens_p[a] -> data );
    //         break;
    //     }

    //     if (tokens_p[a] -> type == T_File_end){
    //         a++;
    //         break;
    //     }
    //     a++;
    // }
    // //SYMBOL TABLE DEBUG
    // //tabulka zatím insertuje úplně všechno a nekontroluje, jestli tam už je
    // printf("\nImprovised symbol table:\n");
    // //ALWAYS USE PRIME NUMBERS FOR HASH TABLE SIZE
    // symtable_t *table = symtable_init(11);
    // for (int j = 0; j < i; j++)
    // {
    //     //pouzivejte vzdycky check a pak insert [Seidly]
    //     table = symtable_check_size(table);
    //     symbol_t *symbol = token_to_symbol(tokens[j]);
    //     symtable_insert(table, symbol);
    // }

    // for (int i = 0; i < 1000; i++)
    // {
    //     free(tokens[i]);
    // }
    // symtable_print(table);
    // free_symbols(table);
    // symtable_dispose(table);

    // Initialize token list and fill it with tokens from stdin
    token_list_t *tokens = malloc(sizeof(token_list_t));
    int exitCode = fillTokenList(tokens);
    // If there was an error, print it and exit
    if (exitCode != EXIT_SUCCESS)
    {
        exit(exitCode);
    }

    printTokenList(tokens);

    freeTokenList(tokens);
    exit(0);
}
