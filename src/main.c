#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "../include/parser.h"
#include "../include/symtable.h"
#include "../include/lexer.h"
#include "../include/stack.h"


int main()
{
    // Debug function, lol
    token_t *tokens[1000];

    for (int i = 0; i < 1000; i++)
    {
        tokens[i] = malloc(sizeof(token_t));
    }
    // Go through tokens until End of file token type
    int i = 0;
    while (true)
    {
        getNextToken(tokens[i]);
        printf("%d:\t", tokens[i]->line);
        printf("%s\t", typeToString(tokens[i]->type));
        if(strlen(typeToString(tokens[i]->type))>7)
            printf("\t");
        switch (tokens[i]->type)
        {
            case T_Identifier:
                printf("Identifier: %s\n", tokens[i]->data);
                break;
            case T_Int:
                printf("Int: %s\n", tokens[i]->data);
                break;
            case T_Float:
                printf("Float: %s\n", tokens[i]->data);
                break;
            case T_Exp:
                printf("Exp: %s\n", tokens[i]->data);
                break;
            case T_String:
                printf("String: %s\n", tokens[i]->data);
                break;
            default:
                printf("\n");
                break;
        }

        if (tokens[i]->type == T_File_end)
        {
            i++;
            break;
        }
        i++;
    }


    //PARSE DEBUG
    rewind(stdin);

    b_stack* stack = (b_stack *) malloc(sizeof(struct b_stack)) ;
    b_stack_init(stack);

    token_t *tokens_p[1000];
    for (int a = 0; a < 1000; a++)
    {
        tokens_p[a] = malloc(sizeof(token_t));
    }
    // Go through tokens until End of file token type
    int a = 0;
    while(true){
        getNextToken(tokens_p[a]);

        if(prog(tokens_p[a], stack) == 1){
            printf("\n\nERROR ON LINE: %d", tokens_p[a] -> line ); //nefunguje , protoze iteruju po druhe, ale kdyz zakomentuju lexer debug tak funguje
            printf("\n\nTHE ERROR IS NEAR: %s\n", tokens_p[a] -> data );
            break;
        }

        if (tokens_p[a] -> type == T_File_end){
            a++;
            break;
        }
        a++;
    }


    // printf("\nImprovised symbol table:\n");
    // hash_table_t *table = hash_table_init(100);
    // for (int j = 0; j < i; j++)
    // {
    //     double lf = (double)table->count / table->size;
    //     if (lf > 0.6)
    //     {
    //         // int nsize = table->size * 2;
    //         table = resize(table);
    //     }
    //     hash_table_insert(table, token_to_symbol(tokens[j]));
    // }
    // hash_table_lookup(table, "strict_types");
    //    // hash_table_print(table);
    // for (int i = 0; i < 1000; i++)
    // {
    //     free(tokens[i]);
    // }
    // hash_table_free(table);
    return 0;
}
