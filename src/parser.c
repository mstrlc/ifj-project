#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "../include/parser.h"

// Access active element in token list
#define ACTIVE_TOKEN (tokens->activeToken)
#define ACTIVE_DATA tokens->activeToken->data
#define ACTIVE_TYPE tokens->activeToken->type

// Move active element in token list
#define ACTIVE_NEXT_WS \
    tokens->activeToken = tokens->activeToken->next;
#define ACTIVE_PREV_WS \
    tokens->activeToken = tokens->activeToken->prev;

// Move active element in token list without whitespace and comments
#define ACTIVE_NEXT                                  \
    tokens->activeToken = tokens->activeToken->next; \
    while (ACTIVE_TYPE == T_Whitespace ||            \
           ACTIVE_TYPE == T_Block_comment ||         \
           ACTIVE_TYPE == T_Line_comment)            \
        tokens->activeToken = tokens->activeToken->next;
#define ACTIVE_PREV                                  \
    tokens->activeToken = tokens->activeToken->prev; \
    while (ACTIVE_TYPE == T_Whitespace ||            \
           ACTIVE_TYPE == T_Block_comment ||         \
           ACTIVE_TYPE == T_Line_comment)            \
        tokens->activeToken = tokens->activeToken->prev;

/**
 *
 * @brief Check if prolog is correct
 *
 * @param tokens Pointer to list of tokens
 * @return int Exit code (0 = OK, 1 = ERROR)
 *
 * @todo Implement all whitespace rules
 */
int checkProlog(token_list_t *tokens)
{
    printf("IN CHECKPROLOG\n");
    int exitCode = 0;

    if (tokens->activeToken->type == T_Start_opening)
    {
        printToken(tokens->activeToken);
        ACTIVE_NEXT_WS;

        if (tokens->activeToken->type == T_Identifier && strcmp(tokens->activeToken->data, "php") == 0)
        {
            printToken(tokens->activeToken);
            ACTIVE_NEXT_WS;

            if (tokens->activeToken->type == T_Whitespace)
            {
                printToken(tokens->activeToken);
                ACTIVE_NEXT_WS;

                if (tokens->activeToken->type == T_Identifier && strcmp(tokens->activeToken->data, "declare") == 0)
                {
                    printToken(tokens->activeToken);
                    ACTIVE_NEXT_WS;

                    if (tokens->activeToken->type == T_L_r_par)
                    {
                        printToken(tokens->activeToken);
                        ACTIVE_NEXT_WS;

                        if (tokens->activeToken->type == T_Identifier && strcmp(tokens->activeToken->data, "strict_types") == 0)
                        {
                            printToken(tokens->activeToken);
                            ACTIVE_NEXT_WS;

                            if (tokens->activeToken->type == T_Assign)
                            {
                                printToken(tokens->activeToken);
                                ACTIVE_NEXT_WS;

                                if (tokens->activeToken->type == T_Int && strcmp(tokens->activeToken->data, "1") == 0)
                                {
                                    printToken(tokens->activeToken);
                                    ACTIVE_NEXT_WS;

                                    if (tokens->activeToken->type == T_R_r_par)
                                    {
                                        printToken(tokens->activeToken);
                                        ACTIVE_NEXT_WS;

                                        if (tokens->activeToken->type == T_Semicolon)
                                        {
                                            printToken(tokens->activeToken);
                                            ACTIVE_NEXT_WS;
                                        }
                                        else
                                            exitCode = 1;
                                    }
                                    else
                                        exitCode = 1;
                                }
                                else
                                    exitCode = 1;
                            }
                            else
                                exitCode = 1;
                        }
                        else
                            exitCode = 1;
                    }
                    else
                        exitCode = 1;
                }
                else
                    exitCode = 1;
            }
            else
                exitCode = 1;
        }
        else
            exitCode = 1;
    }
    else
        exitCode = 1;

    if (exitCode == 1)
        printf("ERROR: Prolog is not correct!\n");
    else
        printf("Prolog is correct!\n");

    if (ACTIVE_TYPE == T_Whitespace)
        ACTIVE_NEXT;

    return exitCode;
}

int parseTerminal(token_list_t *tokens, token_type_t type)
{
    printToken(ACTIVE_TOKEN);
    if (ACTIVE_TYPE == type)
    {
        ACTIVE_NEXT;
        return 0;
    }
    else
        return 1;
}

int parseEpsilon(token_list_t *tokens)
{
    ACTIVE_NEXT;
    return 0;
}

// <value> -> int .
int rule_Value(token_list_t *tokens)
{
    printf("BEGIN VALUE\n");
    int error = 0;

    if (ACTIVE_TYPE == T_Int)
    {
        error = error || parseTerminal(tokens, T_Int);
    }
    else
    {
        error = 1;
    }

    return error;
}

// <expr> -> <value> + <value> .
int rule_Expr(token_list_t *tokens)
{
    printf("BEGIN EXPR\n");
    int error = 0;

    // <expr> -> <value> + <value> .
    if (ACTIVE_TYPE == T_Int)
    {
        error = error || rule_Value(tokens);
        error = error || parseTerminal(tokens, T_Plus);
        error = error || rule_Value(tokens);
    }
}

// <call-params> -> $id .
// <call-params> -> <expr> .
int rule_CallParams(token_list_t *tokens)
{
    printf("BEGIN CALLPARAMS\n");
    int error = 0;

    // <call-params> -> $id .
    if (ACTIVE_TYPE == T_Var_id)
    {
        // $id
        error = error || parseTerminal(tokens, T_Var_id);
    }
    // <call-params> -> <expr> .
    else if (ACTIVE_TYPE == T_Int)
    {
        // <expr>
        error = error || rule_Expr(tokens);
    }
    else
        error = 1;
}

// <assign> -> <expr>
// <assign> -> fun-id ( <call-params> )
int rule_Assign(token_list_t *tokens)
{
    printf("BEGIN ASSIGN\n");
    int error = 0;

    // <assign> -> <expr>
    if (ACTIVE_TYPE == T_Int)
    {
        error = error || rule_Expr(tokens);
    }
    // <assign> -> fun-id ( <call-params> )
    else if (ACTIVE_TYPE == T_Identifier)
    {
        // fun-id
        error = error || parseTerminal(tokens, T_Identifier);
        // (
        error = error || parseTerminal(tokens, T_L_r_par);
        // <call-params>
        error = error || rule_CallParams(tokens);
        // )
        error = error || parseTerminal(tokens, T_R_r_par);
    }
    else
        error = 1;
}

// <stat> -> $id = <assign> .
// <stat> -> fun-id ( <call-params> ) .
int rule_Stat(token_list_t *tokens)
{
    printf("BEGIN STAT\n");
    int error = 0;

    // <stat> -> $id = <assign> .
    if (ACTIVE_TYPE == T_Var_id)
    {
        // $id
        error = error || parseTerminal(tokens, T_Var_id);
        // =
        error = error || parseTerminal(tokens, T_Assign);
        // <assign>
        error = error || rule_Assign(tokens);
    }
    // <stat> -> fun-id ( <call-params> ) .
    else if (ACTIVE_TYPE == T_Identifier)
    {
        // fun-id
        error = error || parseTerminal(tokens, T_Identifier);
        // (
        error = error || parseTerminal(tokens, T_L_r_par);
        // <call-params>
        error = error || rule_CallParams(tokens);
        // )
        error = error || parseTerminal(tokens, T_R_r_par);
    }
    else
    {
        error = 1;
    }

    return error;
}

// <prog> -> <stat> <prog>
// <prog> -> ?> EOF
// <prog> -> EOF
int rule_Prog(token_list_t *tokens)
{
    printf("BEGIN PROG\n");
    int error = 0;

    // <prog> -> <stat> ; <prog>
    if (ACTIVE_TYPE == T_Var_id || ACTIVE_TYPE == T_Identifier)
    {
        // <stat>
        error = error || rule_Stat(tokens);
        // ;
        error = error || parseTerminal(tokens, T_Semicolon);
        // <prog>
        error = error || rule_Prog(tokens);
    }
    // <prog> -> ?> EOF
    else if (ACTIVE_TYPE == T_End_closing)
    {
        // ?>
        error = error || parseTerminal(tokens, T_End_closing);
        // EOF
        if (ACTIVE_TYPE == T_File_end)
        {
            error = error || 0;
            return error;
        }
    }
    // <prog> -> EOF
    else if (ACTIVE_TYPE == T_File_end)
    {
        // EOF
        error = error || 0;
        return error;
    }

    return error;
}

int parser(token_list_t *tokens)
{
    checkProlog(tokens);
    printf("\n\n");

    int error;
    error = rule_Prog(tokens);

    if (error == 0)
        printf("Parsing was successful! :)\n");
    else
        printf("Parsing was not successful! :(\n");

    return 0;
}