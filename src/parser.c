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

int rule_ParamsCont(token_list_t *tokens);
int rule_Params(token_list_t *tokens);
int rule_ArgsCont(token_list_t *tokens);
int rule_Val(token_list_t *tokens);
int rule_Expr(token_list_t *tokens);
int rule_Args(token_list_t *tokens);
int rule_Assign(token_list_t *tokens);
int rule_Stat(token_list_t *tokens);
int rule_Prog(token_list_t *tokens);

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
    printf("PARSE EPSILON\n");
    if (ACTIVE_TYPE == T_Whitespace || ACTIVE_TYPE == T_Block_comment || ACTIVE_TYPE == T_Line_comment)
    {
        ACTIVE_NEXT;
    }
    return 0;
}

// <params-cont> -> , type $id <params-cont> .
// <params-cont> -> eps .
int rule_ParamsCont(token_list_t *tokens)
{
    printf("BEGIN PARAMSCONT\n");
    int error = 0;

    // <params-cont> -> , type $id <params-cont> .
    if (ACTIVE_TYPE == T_Comma)
    {
        error = error || parseTerminal(tokens, T_Comma);
        // type
        if (ACTIVE_TYPE == T_Keyword_Int)
            error = error || parseTerminal(tokens, T_Keyword_Int);
        else if (ACTIVE_TYPE == T_Keyword_Float)
            error = error || parseTerminal(tokens, T_Keyword_Float);
        else if (ACTIVE_TYPE == T_Keyword_String)
            error = error || parseTerminal(tokens, T_Keyword_String);
        else if (ACTIVE_TYPE == T_Keyword_Null)
            error = error || parseTerminal(tokens, T_Keyword_Null);
        else
            error = 1;
        // $id
        error = error || parseTerminal(tokens, T_Var_id);
        // <params-cont>
        error = error || rule_ParamsCont(tokens);
    }
    // <params-cont> -> eps .
    else
    {
        error = error || parseEpsilon(tokens);
    }

    return error;
}

// <params> -> type $id <params-cont> .
// <params> -> eps .
int rule_Params(token_list_t *tokens)
{
    printf("BEGIN PARAMS\n");
    int error = 0;

    // <params> -> type $id <params-cont> .
    if (ACTIVE_TYPE == T_Keyword_Int || ACTIVE_TYPE == T_Keyword_Float || ACTIVE_TYPE == T_Keyword_String || ACTIVE_TYPE == T_Keyword_Null)
    {
        // type
        if (ACTIVE_TYPE == T_Keyword_Int)
            error = error || parseTerminal(tokens, T_Keyword_Int);
        else if (ACTIVE_TYPE == T_Keyword_Float)
            error = error || parseTerminal(tokens, T_Keyword_Float);
        else if (ACTIVE_TYPE == T_Keyword_String)
            error = error || parseTerminal(tokens, T_Keyword_String);
        else if (ACTIVE_TYPE == T_Keyword_Null)
            error = error || parseTerminal(tokens, T_Keyword_Null);
        else
            error = 1;
        // $id
        error = error || parseTerminal(tokens, T_Var_id);
        // <params-cont>
        error = error || rule_ParamsCont(tokens);
    }
    // <params> -> eps .
    else
    {
        error = error || parseEpsilon(tokens);
    }

    return error;
}

// <args-cont> -> , <val> <args-cont> .
// <args-cont> -> eps .
int rule_ArgsCont(token_list_t *tokens)
{
    printf("BEGIN ARGSCONT\n");
    int error = 0;

    // <args-cont> -> , <val> <args-cont> .
    if (ACTIVE_TYPE == T_Comma)
    {
        error = error || parseTerminal(tokens, T_Comma);
        error = error || rule_Val(tokens);
        error = error || rule_ArgsCont(tokens);
    }
    else if (ACTIVE_TYPE == T_R_r_par)
    {
        error = error || parseEpsilon(tokens);
    }
    else
    {
        error = 1;
    }
    printf("END ARGSCONT\n");
    return error;
}

// <val> -> literal .
int rule_Val(token_list_t *tokens)
{
    printf("BEGIN VAL\n");
    int error = 0;

    // <val> -> literal .
    if (ACTIVE_TYPE == T_Int) // TODO DEBUG CHANGE
    {
        error = error || parseTerminal(tokens, T_Int);
    }
    else if (ACTIVE_TYPE == T_Float) // TODO DEBUG CHANGE
    {
        error = error || parseTerminal(tokens, T_Float);
    }
    else if (ACTIVE_TYPE == T_String) // TODO DEBUG CHANGE
    {
        error = error || parseTerminal(tokens, T_String);
    }
    else if (ACTIVE_TYPE == T_Var_id) // TODO DEBUG CHANGE
    {
        error = error || parseTerminal(tokens, T_Var_id);
    }

    return error;
}

// <expr> -> <val> .
int rule_Expr(token_list_t *tokens)
{
    printf("BEGIN EXPR\n");
    int error = 0;

    // <expr> -> <val> .
    if (ACTIVE_TYPE == T_Int) // TODO DEBUG CHANGE
    {
        error = error || rule_Val(tokens);
    }
    else if (ACTIVE_TYPE == T_Float) // TODO DEBUG CHANGE
    {
        error = error || rule_Val(tokens);
    }
    else if (ACTIVE_TYPE == T_String) // TODO DEBUG CHANGE
    {
        error = error || rule_Val(tokens);
    }
    else if (ACTIVE_TYPE == T_Var_id) // TODO DEBUG CHANGE
    {
        error = error || rule_Val(tokens);
    }
    else
    {
        error = 1;
    }

    return error;
}

// <args> -> <val> <args-cont> .
// <args> -> eps .
int rule_Args(token_list_t *tokens)
{
    printf("BEGIN ARGS\n");
    int error = 0;

    // <args> -> <val> <args-cont> .
    if (ACTIVE_TYPE == T_Int || ACTIVE_TYPE == T_Float || ACTIVE_TYPE == T_String || ACTIVE_TYPE == T_Keyword_Null || ACTIVE_TYPE == T_Var_id)
    {
        // <val>
        error = error || rule_Val(tokens);
        // <args-cont>
        error = error || rule_ArgsCont(tokens);
    }
    // <args> -> eps .
    else if (ACTIVE_TYPE == T_R_r_par)
    {
        error = error || parseEpsilon(tokens);
    }
    else
    {
        printf("ARGS ERROR\n");
        error = 1;
    }
    {
        error = error || parseEpsilon(tokens);
    }

    return error;
}

// <assign> -> <expr> .
int rule_Assign(token_list_t *tokens)
{
    printf("BEGIN ASSIGN\n");
    int error = 0;

    // <assign> -> <expr> ; <stat> .
    if (ACTIVE_TYPE == T_Int || ACTIVE_TYPE == T_Float || ACTIVE_TYPE == T_String || ACTIVE_TYPE == T_Keyword_Null || ACTIVE_TYPE == T_Var_id)
    {
        error = error || rule_Expr(tokens);
    }
    // <assign> -> func-id ( <args> ) .
    else if (ACTIVE_TYPE == T_Identifier)
    {
        // func-id
        error = error || parseTerminal(tokens, T_Identifier);
        // (
        error = error || parseTerminal(tokens, T_L_r_par);
        // <args>
        error = error || rule_Args(tokens);
        // )
        error = error || parseTerminal(tokens, T_R_r_par);
    }
    else
    {
        error = 1;
    }

    return error;
}

// <st-list> -> <stat> <st-list> .
// <st-list> -> .
int rule_StList(token_list_t *tokens)
{
    printf("BEGIN STLIST\n");
    int error = 0;

    // <st-list> -> <stat> <st-list> .
    if (ACTIVE_TYPE == T_Keyword_Int ||
        ACTIVE_TYPE == T_Keyword_Float ||
        ACTIVE_TYPE == T_Keyword_String ||
        ACTIVE_TYPE == T_Keyword_Null ||
        ACTIVE_TYPE == T_Var_id ||
        ACTIVE_TYPE == T_Keyword_If ||
        ACTIVE_TYPE == T_Keyword_Return ||
        ACTIVE_TYPE == T_Identifier ||
        ACTIVE_TYPE == T_Keyword_While ||
        ACTIVE_TYPE == T_Var_id)
    {
        // <stat>
        error = error || rule_Stat(tokens);
        // <st-list>
        error = error || rule_StList(tokens);
    }
    // <st-list> -> .
    if (ACTIVE_TYPE == T_R_c_par)
    {
        error = error || parseEpsilon(tokens);
    }
    else
    {
        error = 1;
    }

    return error;
}

// <stat> -> $id = <assign> ; .
// <stat> -> while ( <expr> ) { <st-list> } .
// <stat> -> if ( <expr> ) { <st-list> } else { <st-list> } .
// <stat> -> return <expr> ; .
// <stat> -> <expr> ; .
// <stat> -> func-id ( <args> ) ; .
int rule_Stat(token_list_t *tokens)
{
    printf("BEGIN STAT\n");
    int error = 0;

    // <stat> -> $id = <assign> ; .
    if (ACTIVE_TYPE == T_Var_id)
    {
        // $id
        error = error || parseTerminal(tokens, T_Var_id);
        // =
        error = error || parseTerminal(tokens, T_Assign);
        // <assign>
        error = error || rule_Assign(tokens);
        // ;
        error = error || parseTerminal(tokens, T_Semicolon);
    }
    // <stat> -> while ( <expr> ) { <st-list> } .
    else if (ACTIVE_TYPE == T_Keyword_While)
    {
        // while
        error = error || parseTerminal(tokens, T_Keyword_While);
        // (
        error = error || parseTerminal(tokens, T_L_r_par);
        // <expr>
        error = error || rule_Expr(tokens);
        // )
        error = error || parseTerminal(tokens, T_R_r_par);
        // {
        error = error || parseTerminal(tokens, T_L_c_par);
        // <st-list>
        error = error || rule_StList(tokens);
        // }
        error = error || parseTerminal(tokens, T_R_c_par);
    }
    // <stat> -> if ( <expr> ) { <st-list> } else { <st-list> } .
    else if (ACTIVE_TYPE == T_Keyword_If)
    {
        // if
        error = error || parseTerminal(tokens, T_Keyword_If);
        // (
        error = error || parseTerminal(tokens, T_L_r_par);
        // <expr>
        error = error || rule_Expr(tokens);
        // )
        error = error || parseTerminal(tokens, T_R_r_par);
        // {
        error = error || parseTerminal(tokens, T_L_c_par);
        // <st-list>
        error = error || rule_StList(tokens);
        // }
        error = error || parseTerminal(tokens, T_R_c_par);
        // else
        error = error || parseTerminal(tokens, T_Keyword_Else);
        // {
        error = error || parseTerminal(tokens, T_L_c_par);
        // <st-list>
        error = error || rule_StList(tokens);
        // }
        error = error || parseTerminal(tokens, T_R_c_par);
    }
    // <stat> -> return <expr> ; .
    else if (ACTIVE_TYPE == T_Keyword_Return)
    {
        // return
        error = error || parseTerminal(tokens, T_Keyword_Return);
        // <expr>
        error = error || rule_Expr(tokens);
        // ;
        error = error || parseTerminal(tokens, T_Semicolon);
    }
    // <stat> -> <expr> ; .
    else if (ACTIVE_TYPE == T_Int || ACTIVE_TYPE == T_Float || ACTIVE_TYPE == T_String || ACTIVE_TYPE == T_Keyword_Null || ACTIVE_TYPE == T_Var_id)
    {
        // <expr>
        error = error || rule_Expr(tokens);
        // ;
        error = error || parseTerminal(tokens, T_Semicolon);
    }
    // <stat> -> func-id ( <args> ) ; .
    else if (ACTIVE_TYPE == T_Identifier)
    {
        // func-id
        error = error || parseTerminal(tokens, T_Identifier);
        // (
        error = error || parseTerminal(tokens, T_L_r_par);
        // <args>
        error = error || rule_Args(tokens);
        // )
        error = error || parseTerminal(tokens, T_R_r_par);
        // ;
        error = error || parseTerminal(tokens, T_Semicolon);
    }
    else
    {
        error = 1;
    }
    return error;
}

// <eof> -> ?> <eof> .
// <eof> -> EOF .
int rule_EOF(token_list_t *tokens)
{
    printf("BEGIN EOF\n");
    int error = 0;

    // <eof> -> ?> <eof> .
    if (ACTIVE_TYPE == T_End_closing)
    {
        error = error || parseTerminal(tokens, T_End_closing);
        error = error || rule_EOF(tokens);
    }
    // <eof> -> EOF .
    else if (ACTIVE_TYPE == T_File_end)
    {
        // parseTerminal(tokens, T_File_end);
        error = error || 0;
        return error;
    }
    else
    {
        error = 1;
    }

    return error;
}

// <prog> -> <stat> <prog> .
// <prog> -> <eof> .
int rule_Prog(token_list_t *tokens)
{
    printf("BEGIN PROG\n");
    int error = 0;

    // <prog> -> <stat> <prog> .
    if (ACTIVE_TYPE == T_Var_id ||
        ACTIVE_TYPE == T_Keyword_While ||
        ACTIVE_TYPE == T_Keyword_If ||
        ACTIVE_TYPE == T_Keyword_Return ||
        ACTIVE_TYPE == T_Identifier ||
        ACTIVE_TYPE == T_Keyword_Int ||
        ACTIVE_TYPE == T_Keyword_Float ||
        ACTIVE_TYPE == T_Keyword_String ||
        ACTIVE_TYPE == T_Keyword_Null)
    {
        // <stat>
        error = error || rule_Stat(tokens);
        // <prog>
        error = error || rule_Prog(tokens);
    }
    //     // <prog> -> function func-id ( <params> ) : type { <stat> } <prog> .
    // else if (ACTIVE_TYPE == T_Keyword_Function)
    // {
    //     // function
    //     error = error || parseTerminal(tokens, T_Keyword_Function);
    //     // func-id
    //     error = error || parseTerminal(tokens, T_Identifier);
    //     // (
    //     error = error || parseTerminal(tokens, T_L_r_par);
    //     // <params>
    //     error = error || rule_Params(tokens);
    //     // )
    //     error = error || parseTerminal(tokens, T_R_r_par);
    //     // :
    //     error = error || parseTerminal(tokens, T_Colon);
    //     // type
    //     if (ACTIVE_TYPE == T_Keyword_Int)
    //         error = error || parseTerminal(tokens, T_Keyword_Int);
    //     else if (ACTIVE_TYPE == T_Keyword_Float)
    //         error = error || parseTerminal(tokens, T_Keyword_Float);
    //     else if (ACTIVE_TYPE == T_Keyword_String)
    //         error = error || parseTerminal(tokens, T_Keyword_String);
    //     else if (ACTIVE_TYPE == T_Keyword_Null)
    //         error = error || parseTerminal(tokens, T_Keyword_Null);
    //     else
    //         error = 1;
    //     // {
    //     error = error || parseTerminal(tokens, T_L_c_par);
    //     // <stat>
    //     error = error || rule_Stat(tokens);
    //     // }
    //     error = error || parseTerminal(tokens, T_R_c_par);
    //     // <prog>
    //     error = error || rule_Prog(tokens);
    // }
    // <prog> -> <eof> .
    else if (ACTIVE_TYPE == T_End_closing || ACTIVE_TYPE == T_File_end)
    {
        // <eof>
        error = error || rule_EOF(tokens);
    }
    else
    {
        error = 1;
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