/**
 * @file parser.c
 *
 * Implementation of syntax and semantic analysis and code generation 
 * 
 * IFJ project 2022
 *
 * @author <xstrel03> Matyáš Strelec
 *
 */

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

// Move active element in token list skipping whitespaces or comments
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

// Do not overwrite error code if it is already set
#define HANDLE_ERROR \
    if (error == 0)  \
    error

int rule_Prog(token_list_t *tokens);
int rule_ParamsCont(token_list_t *tokens);
int rule_Params(token_list_t *tokens);
int rule_ArgsCont(token_list_t *tokens);
int rule_Val(token_list_t *tokens);
int rule_Args(token_list_t *tokens);
int rule_Stat(token_list_t *tokens);
int rule_StList(token_list_t *tokens);
int rule_Assign(token_list_t *tokens);
int rule_Term(token_list_t *tokens);
int rule_Expr(token_list_t *tokens);

/**
 * @brief Parse token, used in prolog
 * 
 * If the active token matches the given type, parse it and move to the next one,
 * including whitespace and comments. Else, return error.
 * 
 * @param tokens Pointer to list of tokens to be parsed
 * @param type Type of token to be parsed
 * @return int Error code - success (EXIT_SUCCESS) or failure (ERR_SYNTAX)
 */

 //pomocne funkce 
 char* make_random_label(){
    char const abeceda[]= "abcdefghijklmnopqrstuvwxyz0123456789";
    char* output = malloc(sizeof(char)*8);
    int random;
    output[0] = 'L';
    for(int i = 1; i<8; i++){
        random = rand()%35;
        output[i] = abeceda [random];
    }
    output[8] = '\0';
    return output;
}


int parseProlog(token_list_t *tokens, token_type_t type)
{
    if (ACTIVE_TYPE == type)
    {
        ACTIVE_NEXT_WS;
        return EXIT_SUCCESS;
    }
    else
    {
        return ERR_SYNTAX;
    }
}

/**
 * @brief Parse terminal
 * 
 * If the active token matches the given type, parse it and move to the next token
 * that is not whitespace or comment. Else, return error.
 * 
 * @param tokens Pointer to list of tokens to be parsed
 * @param type Type of token to be parsed
 * @return int Error code - success (EXIT_SUCCESS) or failure (ERR_SYNTAX)
 */
int parseTerminal(token_list_t *tokens, token_type_t type)
{
    if (ACTIVE_TYPE == type)
    {
        ACTIVE_NEXT;
        return EXIT_SUCCESS;
    }
    else
    {
        return ERR_SYNTAX;
    }
}

/**
 * @brief Parse epsilon
 * 
 * If the active token is whitespace or comment, move onto the next one.
 * 
 * @param tokens Pointer to list of tokens to be parsed
 * @return int Error code - success (EXIT_SUCCESS)
 */
int parseEpsilon(token_list_t *tokens)
{
    if (ACTIVE_TYPE == T_Whitespace || ACTIVE_TYPE == T_Block_comment || ACTIVE_TYPE == T_Line_comment)
    {
        ACTIVE_NEXT;
    }
    return EXIT_SUCCESS;
}

/**
 *
 * @brief Check if prolog is correct
 *
 * @param tokens Pointer to list of tokens where prolog is going to be checked
 * @return int Error code (0 = OK, others = error)
 */
int checkProlog(token_list_t *tokens)
{
    int error = 0;

    // <?
    HANDLE_ERROR = parseProlog(tokens, T_Start_opening);
    // php
    if (strcmp(ACTIVE_DATA, "php") == 0)
    {
        HANDLE_ERROR = parseProlog(tokens, T_Identifier);
    }
    HANDLE_ERROR = parseProlog(tokens, T_Whitespace);
    // declare
    if (strcmp(ACTIVE_DATA, "declare") == 0)
    {
        HANDLE_ERROR = parseProlog(tokens, T_Identifier);
    }
    // (
    HANDLE_ERROR = parseProlog(tokens, T_L_r_par);
    // strict_types
    if (strcmp(ACTIVE_DATA, "strict_types") == 0)
    {
        HANDLE_ERROR = parseProlog(tokens, T_Identifier);
    }
    // =
    HANDLE_ERROR = parseProlog(tokens, T_Assign);
    // 1
    if (strcmp(ACTIVE_DATA, "1") == 0)
    {
        HANDLE_ERROR = parseProlog(tokens, T_Int);
    }
    // )
    HANDLE_ERROR = parseProlog(tokens, T_R_r_par);
    // ;
    HANDLE_ERROR = parseProlog(tokens, T_Semicolon);

    parseEpsilon(tokens);

    //codegen HEADER
    printf(".IFJcode22\n");
    printf("DEFVAR GF@expRes\n");
    printf("MOVE GF@expRes bool@false\n");
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");
    
    return error;
}

// <params-cont> -> , type $id <params-cont>
// <params-cont> ->
int rule_ParamsCont(token_list_t *tokens)
{
    int error = 0;

    // <params-cont> -> , type $id <params-cont>
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
    // <params-cont> ->
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

// <params> -> type $id <params-cont>
// <params> ->
int rule_Params(token_list_t *tokens)
{
    int error = 0;

    // <params> -> type $id <params-cont>
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
    // <params> ->
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

// <args> -> <term> <args-cont>
// <args> -> 
int rule_ArgsCont(token_list_t *tokens)
{
    int error = 0;

    // <args-cont> -> , <term> <args-cont>
    if (ACTIVE_TYPE == T_Comma)
    {
        HANDLE_ERROR = parseTerminal(tokens, T_Comma);
        HANDLE_ERROR = rule_Term(tokens);
        HANDLE_ERROR = rule_ArgsCont(tokens);
    }
    // <args-cont> ->
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

// <expr> -> <term>
int rule_Expr(token_list_t *tokens)
{
    int error = 0;

    // <expr> -> <term>
    if (ACTIVE_TYPE == T_Var_id || ACTIVE_TYPE == T_Int || ACTIVE_TYPE == T_Float || ACTIVE_TYPE == T_String)
    {
        HANDLE_ERROR = rule_Term(tokens);
    }
    else
    {
        HANDLE_ERROR = ERR_SYNTAX;
    }

    return error;
}

// <args> -> <term> <args-cont>
// <args> -> 
int rule_Args(token_list_t *tokens)
{
    int error = 0;

    if (ACTIVE_TYPE == T_Int || ACTIVE_TYPE == T_Float || ACTIVE_TYPE == T_String || ACTIVE_TYPE == T_Var_id)
    {
        // <val>
        HANDLE_ERROR = rule_Term(tokens);
        // <args-cont>
        HANDLE_ERROR = rule_ArgsCont(tokens);
    }
    // <args> ->
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

// <assign> -> <expr>
int rule_Assign(token_list_t *tokens)
{
    int error = 0;

    // <assign> -> <expr>
    if (ACTIVE_TYPE == T_Int || ACTIVE_TYPE == T_Float || ACTIVE_TYPE == T_String || ACTIVE_TYPE == T_Keyword_Null || ACTIVE_TYPE == T_Var_id)
    {
        error = rule_Expr(tokens);
    }
    // <assign> -> func-id ( <args> )
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

// <st-list> -> <stat> <st-list>
// <st-list> ->
int rule_StList(token_list_t *tokens)
{
    int error = 0;

    // <st-list> -> <stat> <st-list>
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

// <stat> -> $id = <assign> ;
// <stat> -> while ( <expr> ) { <st-list> }
// <stat> -> if ( <expr> ) { <st-list> } else { <st-list> }
// <stat> -> return <expr> ;
// <stat> -> <expr> ;
// <stat> -> func-id ( <args> ) ;
int rule_Stat(token_list_t *tokens)
{
    int error = 0;

    // <stat> -> $id = <assign> ;
    if (ACTIVE_TYPE == T_Var_id)
    {
        // $id
        char* var = ACTIVE_DATA;
        HANDLE_ERROR = parseTerminal(tokens, T_Var_id);
        // =
        HANDLE_ERROR = parseTerminal(tokens, T_Assign);
        // <assign>
        HANDLE_ERROR = rule_Assign(tokens);
        // ;
        HANDLE_ERROR = parseTerminal(tokens, T_Semicolon);

        printf("DEFVAR LF@%s\n", var);
        printf("MOVE LF@%s GF@expRes\n", var);
        
    }
    // <stat> -> while ( <expr> ) { <st-list> }
    else if (ACTIVE_TYPE == T_Keyword_While)
    {
        char* while_label = make_random_label();

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
    // <stat> -> if ( <expr> ) { <st-list> } else { <st-list> }
    else if (ACTIVE_TYPE == T_Keyword_If)
    {
        char* if_label = make_random_label();
        char* else_label = make_random_label();

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

        //codegen IF
        printf("JUMPIFEQ %s GF@expRes bool@false\n", if_label); // tady uz by mela byt prirazena hodnota expressionu
        printf("CREATEFRAME\n");
        printf("PUSHFRAME\n");

        // <st-list>
        HANDLE_ERROR = rule_StList(tokens);
        // }
        HANDLE_ERROR = parseTerminal(tokens, T_R_c_par);

        //codegen IF -> ELSE
        printf("POPFRAME\n");
        printf("JUMP %s\n", else_label);
        printf("LABEL %s\n", if_label);
        printf("CREATEFRAME\n");
        printf("PUSHFRAME\n");

        // else
        HANDLE_ERROR = parseTerminal(tokens, T_Keyword_Else);
        // {
        HANDLE_ERROR = parseTerminal(tokens, T_L_c_par);
        // <st-list>
        HANDLE_ERROR = rule_StList(tokens);
        // }
        HANDLE_ERROR = parseTerminal(tokens, T_R_c_par);

        //codegen ELSE -> END
        printf("LABEL %s\n", else_label);
        printf("POPFRAME\n");
        free(else_label);
        free(if_label);
        
    }
    // <stat> -> return <expr> ;
    else if (ACTIVE_TYPE == T_Keyword_Return)
    {
        // return
        HANDLE_ERROR = parseTerminal(tokens, T_Keyword_Return);
        // <expr>
        HANDLE_ERROR = rule_Expr(tokens);
        // ;
        HANDLE_ERROR = parseTerminal(tokens, T_Semicolon);
    }
    // <stat> -> <expr> ;
    else if (ACTIVE_TYPE == T_Int || ACTIVE_TYPE == T_Float || ACTIVE_TYPE == T_String || ACTIVE_TYPE == T_Keyword_Null || ACTIVE_TYPE == T_Var_id)
    {
        // <expr>
        HANDLE_ERROR = rule_Expr(tokens);
        // ;
        HANDLE_ERROR = parseTerminal(tokens, T_Semicolon);
    }
    // <stat> -> func-id ( <args> ) ;
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

// <term> -> $id
// <term> -> val
int rule_Term(token_list_t *tokens)
{
    int error = 0;

    // <term> -> $id
    if (ACTIVE_TYPE == T_Var_id)
    {
        // $id
        HANDLE_ERROR = parseTerminal(tokens, T_Var_id);
    }
    // <term> -> val
    else if (ACTIVE_TYPE == T_Int || ACTIVE_TYPE == T_Float || ACTIVE_TYPE == T_String)
    {
        // val
        HANDLE_ERROR = parseTerminal(tokens, ACTIVE_TYPE);
    }
    else
    {
        HANDLE_ERROR = ERR_SYNTAX;
    }

    return error;
}

// <eof> -> ?> <eof>
// <eof> -> EOF
int rule_EOF(token_list_t *tokens)
{
    int error = 0;

    // <eof> -> ?> <eof>
    if (ACTIVE_TYPE == T_End_closing)
    {
        HANDLE_ERROR = parseTerminal(tokens, T_End_closing);
        // Check for epilog rules (only newline permitted after epilog if not EOF)
        if (tokens->lastToken->type == T_File_end)
        {
            if (tokens->lastToken->prev->type == T_End_closing)
            {
                HANDLE_ERROR = EXIT_SUCCESS;
                printf("POPFRAME");
            }
            else if (tokens->lastToken->prev->type == T_Whitespace &&
                     tokens->lastToken->prev->prev->type == T_End_closing &&
                     strcmp(tokens->lastToken->prev->data, "\n") == 0)
            {
                HANDLE_ERROR = EXIT_SUCCESS;
            }
            else
            {
                HANDLE_ERROR = ERR_SYNTAX;
            }
        }
        else
        {
            HANDLE_ERROR = ERR_SYNTAX;
        }
        HANDLE_ERROR = rule_EOF(tokens);
    }
    // <eof> -> EOF
    else if (ACTIVE_TYPE == T_File_end)
    {
        HANDLE_ERROR = 0;
        return error;
    }
    else
    {
        HANDLE_ERROR = ERR_SYNTAX;
    }

    return error;
}

// <prog> -> <stat> <prog>
// <prog> -> function func-id ( <params> ) : type { <st-list> } <prog>
// <prog> -> <eof>
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
    // <prog> -> function func-id ( <params> ) : type { <st-list> } <prog>
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
        else if (ACTIVE_TYPE == T_Keyword_Void)
        {
            HANDLE_ERROR = parseTerminal(tokens, T_Keyword_Void);
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
    // <prog> -> <eof>
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

/**
 * @brief Parse the input file based on syntax rules in defined grammar
 * 
 * Used rule is called based on LL(1) grammar parsing table
 * 
 * @param tokens Pointer to list of tokens containing input
 * @return int Error code - success (0) or failure (1 = ERR_LEXEME or 2 = ERR_SYNTAX)
 */
int parser(token_list_t *tokens)
{
    int error = 0;

    error = checkProlog(tokens);
    if (error != 0)
        return 1;
    // Begin parsing
    error = rule_Prog(tokens);

    return error;
}
