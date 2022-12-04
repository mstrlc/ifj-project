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
#include "../include/stack.h"
#include "../include/symtable.h"
#include "../include/parse_tree.h"
#include "../include/exp_parser.h"

int rule_Prog(token_list_t *tokens, Symtables* symtables);
int rule_ParamsCont(token_list_t *tokens);
int rule_Params(token_list_t *tokens);
int rule_ArgsCont(token_list_t *tokens, int* argCount, token_t* arg_value, stack* arg_stack );
int rule_Val(token_list_t *tokens);
int rule_Args(token_list_t *tokens);
int rule_Stat(token_list_t *tokens, Symtables* symtables);
int rule_StList(token_list_t *tokens, Symtables* symtables);
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
int checkProlog(token_list_t *tokens, Symtables* symtables){
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

    //CODEGEN HEADER
    //printf(".IFJcode22\n");
    printf("DEFVAR GF@assignedValue\n");
    printf("DEFVAR GF@ret\n");
    printf("MOVE GF@assignedValue bool@true\n");
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");

    symtable_defvar_print(symtables->vars_table_array[symtables -> actual_table_index]);
    //END CODEGEN HEADER

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
int rule_ArgsCont(token_list_t *tokens, int* argCount, token_t* arg_value, stack* arg_stack )
{
    int error = 0;

    *argCount = *argCount + 1;
    char* push_arg_command = malloc(sizeof(char)*100);
    if(arg_value->type == T_String){
        sprintf(push_arg_command, "PUSHS string@%s",arg_value->data);
    }
    if(arg_value->type == T_Int){
        sprintf(push_arg_command, "PUSHS int@%s",arg_value->data);
    }
    if(arg_value->type == T_Float){
        sprintf(push_arg_command, "PUSHS float@%s",arg_value->data);
    }
    if(arg_value->type == T_Var_id){
        sprintf(push_arg_command, "PUSHS LF@%s",arg_value->data);
    }

    stack_push(arg_stack, push_arg_command);

    // printf("PUSHS string@%s\n",arg_value->data);


    // <args-cont> -> , <term> <args-cont>
    if (ACTIVE_TYPE == T_Comma)
    {
        HANDLE_ERROR = parseTerminal(tokens, T_Comma);
        arg_value = ACTIVE_TOKEN;
        HANDLE_ERROR = rule_Term(tokens);
        
       
        HANDLE_ERROR = rule_ArgsCont(tokens, argCount, arg_value, arg_stack );
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
        //exp_parser(tokens);
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
int argCount = 0; // 
int rule_Args(token_list_t *tokens)
{
    int error = 0;
    
    
    token_t* arg_value = ACTIVE_TOKEN;
    stack* arg_stack = stack_init();

    if (ACTIVE_TYPE == T_Int || ACTIVE_TYPE == T_Float || ACTIVE_TYPE == T_String || ACTIVE_TYPE == T_Var_id)
    {
        // <val>
        HANDLE_ERROR = rule_Term(tokens);
        // <args-cont>
        HANDLE_ERROR = rule_ArgsCont(tokens, &argCount, arg_value, arg_stack);
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

    while(!stack_is_empty(arg_stack)){
        printf("%s\n", stack_top(arg_stack)->stack_str);
        stack_pop(arg_stack);
    }

    return error;
}

// <assign> -> <expr>
// <assign> -> func-id ( <args> )
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
        char* functionName = ACTIVE_DATA;
        // func-id
        HANDLE_ERROR = parseTerminal(tokens, T_Identifier);
        // (
        HANDLE_ERROR = parseTerminal(tokens, T_L_r_par);
        // <args>
        HANDLE_ERROR = rule_Args(tokens);
        // )
        HANDLE_ERROR = parseTerminal(tokens, T_R_r_par);

        if(strcmp(functionName, "reads") == 0){
            printf("CALL reads\n");
            printf("MOVE GF@assignedValue GF@ret\n");
        }
        if(strcmp(functionName, "readi") == 0){
            printf("CALL readi\n");
            printf("MOVE GF@assignedValue GF@ret\n");
        }
        if(strcmp(functionName, "readf") == 0){
            printf("CALL readf\n");
            printf("MOVE GF@assignedValue GF@ret\n");
        }
        if(strcmp(functionName, "strlen") == 0){
            printf("CALL strlen\n");
            printf("MOVE GF@assignedValue GF@ret\n");
            argCount = 0; 
        }
        if(strcmp(functionName, "strval") == 0){
            printf("CALL strval\n");
            printf("MOVE GF@assignedValue GF@ret\n");
            argCount = 0; 
        }
        if(strcmp(functionName, "intval") == 0){
            printf("CALL intval\n");
            printf("MOVE GF@assignedValue GF@ret\n");
            argCount = 0; 
        }
        if(strcmp(functionName, "floatval") == 0){
            printf("CALL floatval\n");
            printf("MOVE GF@assignedValue GF@ret\n");
            argCount = 0; 
        }
        if(strcmp(functionName, "substring") == 0){
            printf("CALL substring\n");
            printf("MOVE GF@assignedValue GF@ret\n");
            argCount = 0; 
        }
        if(strcmp(functionName, "ord") == 0){
            printf("CALL ord\n");
            printf("MOVE GF@assignedValue GF@ret\n");
            argCount = 0; 
        }
        if(strcmp(functionName, "chr") == 0){
            printf("CALL chr\n");
            printf("MOVE GF@assignedValue GF@ret\n");
            argCount = 0; 
        }

    }
    else
    {
        HANDLE_ERROR = ERR_SYNTAX;
    }

    return error;
}

// <st-list> -> <stat> <st-list>
// <st-list> ->
int rule_StList(token_list_t *tokens, Symtables* symtables)
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
        HANDLE_ERROR = rule_Stat(tokens, symtables);
        // <st-list>
        HANDLE_ERROR = rule_StList(tokens, symtables);
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
int rule_Stat(token_list_t *tokens, Symtables* symtables)
{
    int error = 0;

    // <stat> -> $id = <assign> ;
    if (ACTIVE_TYPE == T_Var_id)
    {
        // $id
        token_t* var = ACTIVE_TOKEN;
        HANDLE_ERROR = parseTerminal(tokens, T_Var_id);
        // =
        HANDLE_ERROR = parseTerminal(tokens, T_Assign);
        // <assign>
        HANDLE_ERROR = rule_Assign(tokens);
        // ;
        HANDLE_ERROR = parseTerminal(tokens, T_Semicolon);
        
        //CODEGEN var init and assign
        if(symtable_lookup(symtables -> vars_table_array[symtables->actual_table_index], var->data) == NULL){
            printf("DEFVAR LF@%s\n", var->data); //pridat podminku nedefinovanosti promenne
            symtable_insert(symtables -> vars_table_array[symtables->actual_table_index], token_to_symbol(var));
        }
        //END CODEGEN var init and assign
        printf("MOVE LF@%s GF@assignedValue\n", var->data);
        
    }
    // <stat> -> while ( <expr> ) { <st-list> }
    else if (ACTIVE_TYPE == T_Keyword_While)
    {
        //CODEGEN labels init
        char* while_label_end = make_random_label();
        char* while_label_begin = make_random_label();
        //END CODEGEN labels init

        // while
        HANDLE_ERROR = parseTerminal(tokens, T_Keyword_While);
        // (
        HANDLE_ERROR = parseTerminal(tokens, T_L_r_par);
        // <expr>
        HANDLE_ERROR = rule_Expr(tokens);
        // )
        HANDLE_ERROR = parseTerminal(tokens, T_R_r_par);

        //CODEGEN WHILE -> BEGIN
        printf("JUMPIFEQ %s GF@assignedValue bool@false\n",while_label_end);
        printf("LABEL %s\n", while_label_begin);
        //END CODEGEN WHILE -> BEGIN

        // {
        HANDLE_ERROR = parseTerminal(tokens, T_L_c_par);
        // <st-list>
        HANDLE_ERROR = rule_StList(tokens, symtables);
        // }
        HANDLE_ERROR = parseTerminal(tokens, T_R_c_par);

        //CODEGEN WHILE -> END
        printf("JUMP %s\n", while_label_begin);
        printf("LABEL %s\n", while_label_end);
        free(while_label_begin);
        free(while_label_end); 
        //END CODEGEN WHILE -> END

    }
    // <stat> -> if ( <expr> ) { <st-list> } else { <st-list> }
    else if (ACTIVE_TYPE == T_Keyword_If)
    {
        //CODEGEN labels init
        char* if_label = make_random_label();
        char* else_label = make_random_label();
        //END CODEGEN

        // if
        HANDLE_ERROR = parseTerminal(tokens, T_Keyword_If);
        // (
        HANDLE_ERROR = parseTerminal(tokens, T_L_r_par);
        // <expr>
        HANDLE_ERROR = rule_Expr(tokens);
        // )
        HANDLE_ERROR = parseTerminal(tokens, T_R_r_par);

        //CODEGEN IF -> BEGIN
        printf("JUMPIFEQ %s GF@assignedValue bool@false\n", if_label); 
        //END CODEGEN IF -> BEGIN

        // {
        HANDLE_ERROR = parseTerminal(tokens, T_L_c_par);
        // <st-list>
        HANDLE_ERROR = rule_StList(tokens, symtables);
        // }
        HANDLE_ERROR = parseTerminal(tokens, T_R_c_par);

        //CODEGEN IF -> END, ELSE -> BEGIN
        printf("JUMP %s\n", else_label);
        printf("LABEL %s\n", if_label);
        //END CODEGEN IF -> END, ELSE -> BEGIN

        // else
        HANDLE_ERROR = parseTerminal(tokens, T_Keyword_Else);
        // {
        HANDLE_ERROR = parseTerminal(tokens, T_L_c_par);
        // <st-list>
        HANDLE_ERROR = rule_StList(tokens, symtables);
        // }
        HANDLE_ERROR = parseTerminal(tokens, T_R_c_par);

        //CODEGEN ELSE -> END
        printf("LABEL %s\n", else_label);
        free(else_label);
        free(if_label);
        //END CODEGEN ELSE -> END
        
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
        char* functionName = ACTIVE_DATA;
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

        //CODEGEN print argumentu
        if(strcmp(functionName, "write") == 0){
            printf("PUSHS int@%d\n", argCount);
            printf("CALL write\n");
            argCount = 0;
        }
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
int rule_Prog(token_list_t *tokens, Symtables* symtables)
{
    // printf("BEGIN PROG\n");
    int error = 0;
    symtables -> actual_table_index = 0;

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
        HANDLE_ERROR = rule_Stat(tokens, symtables);
        // <prog>
        HANDLE_ERROR = rule_Prog(tokens, symtables);
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

        //CODEGEN function body -> start
        printf("CREATEFRAME\n");
        printf("PUSHFRAME\n");

        symtables -> function_table_index++;
        symtables -> actual_table_index = symtables -> function_table_index; // chceme symtable aktualni funkce
        
        if(!symtables -> vars_table_array[symtables->actual_table_index]) //jesti je to druhy pruchod tak neinicializuj (bcs by se nam smazali data)
            symtables -> vars_table_array[symtables->actual_table_index] = symtable_init(100);
        // printf("(%d)\n",symtables->actual_table_index);
        symtable_defvar_print(symtables->vars_table_array[symtables->actual_table_index]);
        //END CODEGEN function body -> start

        // {
        HANDLE_ERROR = parseTerminal(tokens, T_L_c_par);
        // <st-list>
        HANDLE_ERROR = rule_StList(tokens, symtables);
        // }
        HANDLE_ERROR = parseTerminal(tokens, T_R_c_par);

        //CODEGEN function body -> end
        printf("POPFRAME\n");
        //END CODEGEN function body -> end

        // <prog>
        HANDLE_ERROR = rule_Prog(tokens, symtables);
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
int parser(token_list_t *tokens, Symtables* symtables)
{
    
    int error = 0;

    error = checkProlog(tokens, symtables);
    if (error != 0)
        return 1;
    // Begin parsing
    error = rule_Prog(tokens, symtables);

    return error;
}