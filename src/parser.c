#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "../include/parser.h"
#include "../include/common.h"
#include "../include/error.h"

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

#define HANDLE_ERROR \
    if (error == 0)  \
    error

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
    // printf("IN CHECKPROLOG\n");
    int exitCode = 0;

    if (tokens->activeToken->type == T_Start_opening)
    {
        ACTIVE_NEXT_WS;
        if (tokens->activeToken->type == T_Identifier && strcmp(tokens->activeToken->data, "php") == 0)
        {
            ACTIVE_NEXT_WS;
            if (tokens->activeToken->type == T_Whitespace)
            {
                ACTIVE_NEXT_WS;
                if (tokens->activeToken->type == T_Identifier && strcmp(tokens->activeToken->data, "declare") == 0)
                {
                    ACTIVE_NEXT_WS;
                    if (tokens->activeToken->type == T_L_r_par)
                    {
                        ACTIVE_NEXT_WS;
                        if (tokens->activeToken->type == T_Identifier && strcmp(tokens->activeToken->data, "strict_types") == 0)
                        {
                            ACTIVE_NEXT_WS;
                            if (tokens->activeToken->type == T_Assign)
                            {
                                ACTIVE_NEXT_WS;
                                if (tokens->activeToken->type == T_Int && strcmp(tokens->activeToken->data, "1") == 0)
                                {
                                    ACTIVE_NEXT_WS;
                                    if (tokens->activeToken->type == T_R_r_par)
                                    {
                                        ACTIVE_NEXT_WS;
                                        if (tokens->activeToken->type == T_Semicolon)
                                        {
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

    if (ACTIVE_TYPE == T_Whitespace)
        ACTIVE_NEXT;

    return exitCode;
}

int parseTerminal(token_list_t *tokens, token_type_t type)
{
    // printToken(ACTIVE_TOKEN);
    if (ACTIVE_TYPE == type)
    {
        ACTIVE_NEXT;
        return 0;
    }
    else
    {
        return ERR_SYNTAX;
    }
}

int parseEpsilon(token_list_t *tokens)
{
    // printf("PARSE EPSILON\n");
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
    // printf("BEGIN PARAMSCONT\n");
    int error = 0;

    // <params-cont> -> , type $id <params-cont> .
    if (ACTIVE_TYPE == T_Comma)
    {
        HANDLE_ERROR = parseTerminal(tokens, T_Comma);
        // type
        if (ACTIVE_TYPE == T_Keyword_Int)
        {
            HANDLE_ERROR = parseTerminal(tokens, T_Keyword_Int);
        }
        else if (ACTIVE_TYPE == T_Keyword_Float)
        {
            HANDLE_ERROR = parseTerminal(tokens, T_Keyword_Float);
        }
        else if (ACTIVE_TYPE == T_Keyword_String)
        {
            HANDLE_ERROR = parseTerminal(tokens, T_Keyword_String);
        }
        else if (ACTIVE_TYPE == T_Keyword_Null)
        {
            HANDLE_ERROR = parseTerminal(tokens, T_Keyword_Null);
        }
        else
        {
            HANDLE_ERROR = ERR_SYNTAX;
        }
        // $id
        HANDLE_ERROR = parseTerminal(tokens, T_Var_id);
        // <params-cont>
        HANDLE_ERROR = rule_ParamsCont(tokens);
    }
    // <params-cont> -> eps .
    else if (ACTIVE_TYPE == T_R_r_par)
    {
        HANDLE_ERROR = parseEpsilon(tokens);
    }
    else
    {
        HANDLE_ERROR = ERR_SYNTAX;
    }

    return error;
}

// <params> -> type $id <params-cont> .
// <params> -> eps .
int rule_Params(token_list_t *tokens)
{
    // printf("BEGIN PARAMS\n");
    int error = 0;

    // <params> -> type $id <params-cont> .
    if (ACTIVE_TYPE == T_Keyword_Int || ACTIVE_TYPE == T_Keyword_Float || ACTIVE_TYPE == T_Keyword_String || ACTIVE_TYPE == T_Keyword_Null)
    {
        // type
        if (ACTIVE_TYPE == T_Keyword_Int)
        {
            HANDLE_ERROR = parseTerminal(tokens, T_Keyword_Int);
        }
        else if (ACTIVE_TYPE == T_Keyword_Float)
        {
            HANDLE_ERROR = parseTerminal(tokens, T_Keyword_Float);
        }
        else if (ACTIVE_TYPE == T_Keyword_String)
        {
            HANDLE_ERROR = parseTerminal(tokens, T_Keyword_String);
        }
        else if (ACTIVE_TYPE == T_Keyword_Null)
        {
            HANDLE_ERROR = parseTerminal(tokens, T_Keyword_Null);
        }
        else
        {
            HANDLE_ERROR = ERR_SYNTAX;
        }
        // $id
        HANDLE_ERROR = parseTerminal(tokens, T_Var_id);
        // <params-cont>
        HANDLE_ERROR = rule_ParamsCont(tokens);
    }
    // <params> -> eps .
    else if (ACTIVE_TYPE == T_R_r_par)
    {
        HANDLE_ERROR = parseEpsilon(tokens);
    }
    else
    {
        HANDLE_ERROR = ERR_SYNTAX;
    }

    return error;
}

// <args-cont> -> , <val> <args-cont> .
// <args-cont> -> eps .
int rule_ArgsCont(token_list_t *tokens)
{
    // printf("BEGIN ARGSCONT\n");
    int error = 0;

    // <args-cont> -> , <val> <args-cont> .
    if (ACTIVE_TYPE == T_Comma)
    {
        HANDLE_ERROR = parseTerminal(tokens, T_Comma);
        HANDLE_ERROR = rule_Val(tokens);
        HANDLE_ERROR = rule_ArgsCont(tokens);
    }
    else if (ACTIVE_TYPE == T_R_r_par)
    {
        HANDLE_ERROR = parseEpsilon(tokens);
    }
    else
    {
        HANDLE_ERROR = ERR_SYNTAX;
    }

    return error;
}

// <val> -> literal .
int rule_Val(token_list_t *tokens)
{
    // printf("BEGIN VAL\n");
    int error = 0;

    // <val> -> literal .
    if (ACTIVE_TYPE == T_Int) // TODO DEBUG CHANGE
    {
        HANDLE_ERROR = parseTerminal(tokens, T_Int);
    }
    else if (ACTIVE_TYPE == T_Float) // TODO DEBUG CHANGE
    {
        HANDLE_ERROR = parseTerminal(tokens, T_Float);
    }
    else if (ACTIVE_TYPE == T_String) // TODO DEBUG CHANGE
    {
        HANDLE_ERROR = parseTerminal(tokens, T_String);
    }
    else if (ACTIVE_TYPE == T_Var_id) // TODO DEBUG CHANGE
    {
        HANDLE_ERROR = parseTerminal(tokens, T_Var_id);
    }
    else
    {
        HANDLE_ERROR = ERR_SYNTAX;
    }

    return error;
}

// <expr> -> <val> .
int rule_Expr(token_list_t *tokens)
{
    // printf("BEGIN EXPR\n");
    int error = 0;

    // <expr> -> <val> .
    if (ACTIVE_TYPE == T_Int) // TODO DEBUG CHANGE
    {
        HANDLE_ERROR = rule_Val(tokens);
    }
    else if (ACTIVE_TYPE == T_Float) // TODO DEBUG CHANGE
    {
        HANDLE_ERROR = rule_Val(tokens);
    }
    else if (ACTIVE_TYPE == T_String) // TODO DEBUG CHANGE
    {
        HANDLE_ERROR = rule_Val(tokens);
    }
    else if (ACTIVE_TYPE == T_Var_id) // TODO DEBUG CHANGE
    {
        HANDLE_ERROR = rule_Val(tokens);
    }
    else
    {
        HANDLE_ERROR = ERR_SYNTAX;
    }

    return error;
}

// <args> -> <val> <args-cont> .
// <args> -> eps .
int rule_Args(token_list_t *tokens)
{
    // printf("BEGIN ARGS\n");
    int error = 0;

    // <args> -> <val> <args-cont> .
    if (ACTIVE_TYPE == T_Int || ACTIVE_TYPE == T_Float || ACTIVE_TYPE == T_String || ACTIVE_TYPE == T_Keyword_Null || ACTIVE_TYPE == T_Var_id)
    {
        // <val>
        HANDLE_ERROR = rule_Val(tokens);
        // <args-cont>
        HANDLE_ERROR = rule_ArgsCont(tokens);
    }
    // <args> -> eps .
    else if (ACTIVE_TYPE == T_R_r_par)
    {
        HANDLE_ERROR = parseEpsilon(tokens);
    }
    else
    {
        HANDLE_ERROR = ERR_SYNTAX;
    }

    return error;
}

// <assign> -> <expr> .
int rule_Assign(token_list_t *tokens)
{
    // printf("BEGIN ASSIGN\n");
    int error = 0;

    // <assign> -> <expr> ; <stat> .
    if (ACTIVE_TYPE == T_Int || ACTIVE_TYPE == T_Float || ACTIVE_TYPE == T_String || ACTIVE_TYPE == T_Keyword_Null || ACTIVE_TYPE == T_Var_id)
    {
        error = rule_Expr(tokens);
    }
    // <assign> -> func-id ( <args> ) .
    else if (ACTIVE_TYPE == T_Identifier)
    {
        // func-id
        HANDLE_ERROR = parseTerminal(tokens, T_Identifier);
        // (
        HANDLE_ERROR = parseTerminal(tokens, T_L_r_par);
        // <args>
        HANDLE_ERROR = rule_Args(tokens);
        // )
        HANDLE_ERROR = parseTerminal(tokens, T_R_r_par);
    }
    else
    {
        HANDLE_ERROR = ERR_SYNTAX;
    }

    return error;
}

// <st-list> -> <stat> <st-list> .
// <st-list> -> .
int rule_StList(token_list_t *tokens)
{
    // printf("BEGIN STLIST\n");
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
        HANDLE_ERROR = rule_Stat(tokens);
        // <st-list>
        HANDLE_ERROR = rule_StList(tokens);
    }
    // <st-list> -> .
    if (ACTIVE_TYPE == T_R_c_par)
    {
        HANDLE_ERROR = parseEpsilon(tokens);
    }
    else
    {
        HANDLE_ERROR = ERR_SYNTAX;
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
    // printf("BEGIN STAT\n");
    int error = 0;

    // <stat> -> $id = <assign> ; .
    if (ACTIVE_TYPE == T_Var_id)
    {
        // $id
        HANDLE_ERROR = parseTerminal(tokens, T_Var_id);
        // =
        HANDLE_ERROR = parseTerminal(tokens, T_Assign);
        // <assign>
        HANDLE_ERROR = rule_Assign(tokens);
        // ;
        HANDLE_ERROR = parseTerminal(tokens, T_Semicolon);
    }
    // <stat> -> while ( <expr> ) { <st-list> } .
    else if (ACTIVE_TYPE == T_Keyword_While)
    {
        // while
        HANDLE_ERROR = parseTerminal(tokens, T_Keyword_While);
        // (
        HANDLE_ERROR = parseTerminal(tokens, T_L_r_par);
        // <expr>
        HANDLE_ERROR = rule_Expr(tokens);
        // )
        HANDLE_ERROR = parseTerminal(tokens, T_R_r_par);
        // {
        HANDLE_ERROR = parseTerminal(tokens, T_L_c_par);
        // <st-list>
        HANDLE_ERROR = rule_StList(tokens);
        // }
        HANDLE_ERROR = parseTerminal(tokens, T_R_c_par);
    }
    // <stat> -> if ( <expr> ) { <st-list> } else { <st-list> } .
    else if (ACTIVE_TYPE == T_Keyword_If)
    {
        // if
        HANDLE_ERROR = parseTerminal(tokens, T_Keyword_If);
        // (
        HANDLE_ERROR = parseTerminal(tokens, T_L_r_par);
        // <expr>
        HANDLE_ERROR = rule_Expr(tokens);
        // )
        HANDLE_ERROR = parseTerminal(tokens, T_R_r_par);
        // {
        HANDLE_ERROR = parseTerminal(tokens, T_L_c_par);
        // <st-list>
        HANDLE_ERROR = rule_StList(tokens);
        // }
        HANDLE_ERROR = parseTerminal(tokens, T_R_c_par);
        // else
        HANDLE_ERROR = parseTerminal(tokens, T_Keyword_Else);
        // {
        HANDLE_ERROR = parseTerminal(tokens, T_L_c_par);
        // <st-list>
        HANDLE_ERROR = rule_StList(tokens);
        // }
        HANDLE_ERROR = parseTerminal(tokens, T_R_c_par);
    }
    // <stat> -> return <expr> ; .
    else if (ACTIVE_TYPE == T_Keyword_Return)
    {
        // return
        HANDLE_ERROR = parseTerminal(tokens, T_Keyword_Return);
        // <expr>
        HANDLE_ERROR = rule_Expr(tokens);
        // ;
        HANDLE_ERROR = parseTerminal(tokens, T_Semicolon);
    }
    // <stat> -> <expr> ; .
    else if (ACTIVE_TYPE == T_Int || ACTIVE_TYPE == T_Float || ACTIVE_TYPE == T_String || ACTIVE_TYPE == T_Keyword_Null || ACTIVE_TYPE == T_Var_id)
    {
        // <expr>
        HANDLE_ERROR = rule_Expr(tokens);
        // ;
        HANDLE_ERROR = parseTerminal(tokens, T_Semicolon);
    }
    // <stat> -> func-id ( <args> ) ; .
    else if (ACTIVE_TYPE == T_Identifier)
    {
        // func-id
        HANDLE_ERROR = parseTerminal(tokens, T_Identifier);
        // (
        HANDLE_ERROR = parseTerminal(tokens, T_L_r_par);
        // <args>
        HANDLE_ERROR = rule_Args(tokens);
        // )
        HANDLE_ERROR = parseTerminal(tokens, T_R_r_par);
        // ;
        HANDLE_ERROR = parseTerminal(tokens, T_Semicolon);
    }
    else
    {
        HANDLE_ERROR = ERR_SYNTAX;
    }

    return error;
}

// <eof> -> ?> <eof> .
// <eof> -> EOF .
int rule_EOF(token_list_t *tokens)
{
    // printf("BEGIN EOF\n");
    int error = 0;

    // <eof> -> ?> <eof> .
    if (ACTIVE_TYPE == T_End_closing)
    {
        HANDLE_ERROR = parseTerminal(tokens, T_End_closing);
        HANDLE_ERROR = rule_EOF(tokens);
    }
    // <eof> -> EOF .
    else if (ACTIVE_TYPE == T_File_end)
    {
        // parseTerminal(tokens, T_File_end);
        HANDLE_ERROR = 0;
        return error;
    }
    else
    {
        HANDLE_ERROR = ERR_SYNTAX;
    }

    return error;
}

// <prog> -> <stat> <prog> .
// <prog> -> function func-id ( <params> ) : type { <st-list> } <prog> .
// <prog> -> <eof> .
int rule_Prog(token_list_t *tokens)
{
    // printf("BEGIN PROG\n");
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
        HANDLE_ERROR = rule_Stat(tokens);
        // <prog>
        HANDLE_ERROR = rule_Prog(tokens);
    }
    // <prog> -> function func-id ( <params> ) : type { <st-list> } <prog> .
    else if (ACTIVE_TYPE == T_Keyword_Function)
    {
        // function
        HANDLE_ERROR = parseTerminal(tokens, T_Keyword_Function);
        // func-id
        HANDLE_ERROR = parseTerminal(tokens, T_Identifier);
        // (
        HANDLE_ERROR = parseTerminal(tokens, T_L_r_par);
        // <params>
        HANDLE_ERROR = rule_Params(tokens);
        // )
        HANDLE_ERROR = parseTerminal(tokens, T_R_r_par);
        // :
        HANDLE_ERROR = parseTerminal(tokens, T_Colon);
        // type
        if (ACTIVE_TYPE == T_Keyword_Int)
        {
            HANDLE_ERROR = parseTerminal(tokens, T_Keyword_Int);
        }
        else if (ACTIVE_TYPE == T_Keyword_Float)
        {
            HANDLE_ERROR = parseTerminal(tokens, T_Keyword_Float);
        }
        else if (ACTIVE_TYPE == T_Keyword_String)
        {
            HANDLE_ERROR = parseTerminal(tokens, T_Keyword_String);
        }
        else if (ACTIVE_TYPE == T_Keyword_Null)
        {
            HANDLE_ERROR = parseTerminal(tokens, T_Keyword_Null);
        }
        else
        {
            HANDLE_ERROR = ERR_SYNTAX;
        }
        // {
        HANDLE_ERROR = parseTerminal(tokens, T_L_c_par);
        // <st-list>
        HANDLE_ERROR = rule_StList(tokens);
        // }
        HANDLE_ERROR = parseTerminal(tokens, T_R_c_par);
        // <prog>
        HANDLE_ERROR = rule_Prog(tokens);
    }
    // <prog> -> <eof> .
    else if (ACTIVE_TYPE == T_End_closing || ACTIVE_TYPE == T_File_end)
    {
        // <eof>
        HANDLE_ERROR = rule_EOF(tokens);
    }
    else
    {
        HANDLE_ERROR = ERR_SYNTAX;
    }

    return error;
}

int parser(token_list_t *tokens)
{
    int error = 0;

    error = checkProlog(tokens);
    if (error != 0)
        return error;
    error = rule_Prog(tokens);

    return error;
}
