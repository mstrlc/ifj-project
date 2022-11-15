#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "../include/parser.h"
#include "../include/symtable.h"
#include "../include/lexer.h"


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

        // if (tokens[i]->type != T_Whitespace)
        // {
        //     printf("%d)\t%s", currentLine, tokens[i]->data);

        //     printf("\t%s\n", typeToString(tokens[i]->type));
        //     if (strlen(tokens[i]->data) < 8)
        //         printf("\t");
        // }
        if (tokens[i]->type == T_File_end)
        {
            i++;
            break;
        }
        i++;
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
    // // hash_table_print(table);
    // for (int i = 0; i < 1000; i++)
    // {
    //     free(tokens[i]);
    // }
    // hash_table_free(table);
    return 0;
}
