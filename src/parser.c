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
#include "../include/error.h"
#include "../include/stack.h"
#include "../include/symtable.h"
#include "../include/parse_tree.h"
#include "../include/exp_parser.h"

int pass;

void print_line_till_end(token_list_t *tokens){
int next = 0;
        while(strcmp(ACTIVE_DATA, "\n"))
        {
            printf("%s ", ACTIVE_DATA);
            ACTIVE_NEXT_WS;
            next++;
        }
        for(int i = 0; i < next; i++)
        {
            ACTIVE_PREV_WS;
        }
}

int rule_Prog(token_list_t *tokens, Symtables* symtables);
int rule_ParamsCont(token_list_t *tokens, Symtables* symtables);
int rule_Params(token_list_t *tokens, Symtables* symtables);
int rule_ArgsCont(token_list_t *tokens, token_t* arg_value, stack* arg_stack );
int rule_Val(token_list_t *tokens);
int rule_Args(token_list_t *tokens);
int rule_Stat(token_list_t *tokens, Symtables* symtables);
int rule_StList(token_list_t *tokens, Symtables* symtables);
int rule_Assign(token_list_t *tokens, Symtables* symtables);
int rule_Term(token_list_t *tokens);
int rule_Expr(token_list_t *tokens, Symtables* symtables);
int argCount = 0;
int paramCount =0;
char* functionName = NULL; //saves function name for sharing between rules
bool hasReturn = false; //saves if function has return statement

 //pomocne funkce 
 char* make_random_label(){
    char const abeceda[]= "abcdefghijklmnopqrstuvwxyz0123456789";
    char* output = malloc(sizeof(char)*8);
    if(output == NULL){
        error_exit(ERR_INTERNAL, NULL);
    }  
    int random;
    output[0] = 'L';
    for(int i = 1; i<8; i++){
        random = rand()%35;
        output[i] = abeceda [random];
    }
    output[8] = '\0';
    return output;
}


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
    if (ACTIVE_TYPE == T_Line_comment || ACTIVE_TYPE == T_Block_comment || ACTIVE_TYPE == T_Whitespace)
    {
        HANDLE_ERROR = parseProlog(tokens, ACTIVE_TYPE);
    }
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
    printf("DEFVAR GF@assignedVal\n"); // univerzalni promenna pro predavani hodnoty
    printf("DEFVAR GF@ret\n"); // return val pro funkce
    printf("MOVE GF@assignedVal bool@true\n"); // je pro debug bez assignu

    printf("DEFVAR GF@op1\n"); // pro operace v expressionu
    printf("DEFVAR GF@op2\n"); // pro operace v expressionu
    printf("DEFVAR GF@op3\n"); // pro operace v expressionu
    printf("DEFVAR GF@op4\n"); // pro operace v expressionu
    printf("DEFVAR GF@retType\n"); //for checking return type

    printf("DEFVAR GF@realRetType\n");
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");

    symtable_defvar_print(symtables->vars_table_array[symtables -> active_table_index]);
    //END CODEGEN HEADER

    if(error != 0)
    {
        error_exit(error, ACTIVE_TOKEN);
        exit(error);
    }
    return error;
}

// <params-cont> -> , type $id <params-cont>
// <params-cont> ->
 
int rule_ParamsCont(token_list_t *tokens, Symtables* symtables)
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

        //priradime defvary parametru do vars_table funkce
        if(symtable_lookup(symtables -> vars_table_array[symtables->active_table_index], ACTIVE_DATA) == NULL){
            symtable_insert(symtables -> vars_table_array[symtables->active_table_index], token_to_symbol(ACTIVE_TOKEN));
        }

        printf("POPS LF@%s\n", ACTIVE_DATA);

        // $id
        paramCount++;
        HANDLE_ERROR = parseTerminal(tokens, T_Var_id);
        // <params-cont>
        HANDLE_ERROR = rule_ParamsCont(tokens, symtables);
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

        if(error != 0)
    {
        error_exit(error, ACTIVE_TOKEN);
        exit(error);
    }
    return error;

}

// <params> -> type $id <params-cont>
// <params> ->
int rule_Params(token_list_t *tokens, Symtables* symtables)
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

        //priradime defvary parametru do vars_table funkce
        if(symtable_lookup(symtables -> vars_table_array[symtables->active_table_index], ACTIVE_DATA) == NULL){
            symtable_insert(symtables -> vars_table_array[symtables->active_table_index], token_to_symbol(ACTIVE_TOKEN));
        }

        printf("POPS LF@%s\n", ACTIVE_DATA);
        argCount = 0; // nevim proc tohle ovlivnuje argCount, kdyz jsme v parametrech, ale bez tohohle to nefunguje
        paramCount++; //saves number of params

        HANDLE_ERROR = parseTerminal(tokens, T_Var_id);
        // <params-cont>
        HANDLE_ERROR = rule_ParamsCont(tokens, symtables);
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

        if(error != 0)
    {
        error_exit(error, ACTIVE_TOKEN);
        exit(error);
    }
    return error;

}

// <args> -> <term> <args-cont>
// <args> -> 
int rule_ArgsCont(token_list_t *tokens, token_t* arg_value, stack* arg_stack )
{
    int error = 0;

    // pushujeme argumenty pred volanim funkce ve spravnem formatu
    argCount++;
    char* push_arg_command = malloc(sizeof(char)*100);
    if(push_arg_command == NULL){
        error_exit(ERR_INTERNAL, ACTIVE_TOKEN);
    }
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

    //pushneme volani PUSHS do staku, jelikoz popovani stack nam pak nasledne zaruci to, ze se budou PUSHS generovat ve spravnem poradi
    stack_push(arg_stack, push_arg_command);


    // <args-cont> -> , <term> <args-cont>
    if (ACTIVE_TYPE == T_Comma)
    {
        HANDLE_ERROR = parseTerminal(tokens, T_Comma);
        arg_value = ACTIVE_TOKEN;
        HANDLE_ERROR = rule_Term(tokens);
        
       
        HANDLE_ERROR = rule_ArgsCont(tokens, arg_value, arg_stack );
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
        if(error != 0)
    {
        error_exit(error, ACTIVE_TOKEN);
        exit(error);
    }
    return error;

}

// <expr> -> <term>
int rule_Expr(token_list_t *tokens, Symtables* symtables)
{
    int error = 0;

    // <expr> -> <term>
    if (ACTIVE_TYPE == T_Var_id || ACTIVE_TYPE == T_Int || ACTIVE_TYPE == T_Float || ACTIVE_TYPE == T_String || ACTIVE_TYPE == T_L_r_par || ACTIVE_TYPE == T_Keyword_Null)
    {
        HANDLE_ERROR = exp_parser(tokens, symtables);
    }
    else
    {
        HANDLE_ERROR = ERR_SYNTAX;
    }

    if(error != 0)
    {
        error_exit(error, ACTIVE_TOKEN);
        exit(error);
    }
    return error;

}

// <args> -> <term> <args-cont>
// <args> -> 

int rule_Args(token_list_t *tokens)
{
    int error = 0;
    
    token_t* arg_value = ACTIVE_TOKEN;
    
    // do pole budeme pushovat prikazy ifjcode22 na PUSHovani do stacku, jelikoz pak je budeme potrebovat vyprintovat v obracenem poradi
    stack* arg_stack = stack_init();

    if (ACTIVE_TYPE == T_Int || ACTIVE_TYPE == T_Float || ACTIVE_TYPE == T_String || ACTIVE_TYPE == T_Var_id)
    {
        // <val>
        HANDLE_ERROR = rule_Term(tokens);
        // <args-cont>
        HANDLE_ERROR = rule_ArgsCont(tokens, arg_value, arg_stack);
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

    // printujeme argumenty v obracenem poradi, nez byly predany do funkce v ifj22, jelikoz ifjcode22 stack
    while(!stack_is_empty(arg_stack)){
        printf("%s\n", stack_top(arg_stack)->stack_str);
        stack_pop(arg_stack);
    }
    stack_dispose(arg_stack);
        if(error != 0)
    {
        error_exit(error, ACTIVE_TOKEN);
        exit(error);
    }
    return error;

}

// <assign> -> <expr>
// <assign> -> func-id ( <args> )
int rule_Assign(token_list_t *tokens, Symtables *symtables)
{
    int error = 0;

    // <assign> -> <expr>
    if (ACTIVE_TYPE == T_Int || ACTIVE_TYPE == T_Float || ACTIVE_TYPE == T_String || ACTIVE_TYPE == T_Keyword_Null || ACTIVE_TYPE == T_Var_id || ACTIVE_TYPE == T_L_r_par)
    {
        HANDLE_ERROR = exp_parser(tokens, symtables);
    }
    // <assign> -> func-id ( <args> )
    else if (ACTIVE_TYPE == T_Identifier)
    {
        functionName = ACTIVE_DATA;
        //check if function exists
        if (pass == 2 && symtable_lookup(symtables->function_table, functionName) == NULL
        && strcmp(functionName, "reads") != 0
        && strcmp(functionName, "readi") != 0
        && strcmp(functionName, "readf") != 0
        && strcmp(functionName, "write") != 0
        && strcmp(functionName, "floatval") != 0
        && strcmp(functionName, "intval") != 0
        && strcmp(functionName, "strval") != 0
        && strcmp(functionName, "strlen") != 0
        && strcmp(functionName, "substring") != 0
        && strcmp(functionName, "ord") != 0
        && strcmp(functionName, "chr") != 0)
        {
            error_exit(ERR_UNDEF_REDEF_FUN, ACTIVE_TOKEN);
            exit(ERR_UNDEF_REDEF_FUN);
        }
        // func-id
        HANDLE_ERROR = parseTerminal(tokens, T_Identifier);
        // (
        HANDLE_ERROR = parseTerminal(tokens, T_L_r_par);
        // <args>
        HANDLE_ERROR = rule_Args(tokens);
        // )make
        HANDLE_ERROR = parseTerminal(tokens, T_R_r_par);

        //check if correct num of args
        if (pass == 2 && strcmp(functionName, "write") != 0){
            symbol *func = symtable_lookup(symtables->function_table, functionName);
            if (func != NULL && func -> func_param_count != argCount){
                error_exit(ERR_WRONG_PARAM_RET, ACTIVE_TOKEN);
                exit(ERR_WRONG_PARAM_RET);
            }
        }
        argCount = 0;
        //prepsat do makra kdyz zbyde cas
        //volani vestavenych funkci, ktere maji navratovou hodnotu
        if(strcmp(functionName, "reads") == 0){
            printf("CALL reads\n");
            printf("MOVE GF@assignedVal GF@ret\n");
            printf("MOVE GF@ret nil@nil\n"); // po kazdem prirazeni je treba resetovat GF@ret na NULL, protoze funkce bez navratove hodnoty musi vracet NULL
        }
        else if(strcmp(functionName, "readi") == 0){
            printf("CALL readi\n");
            printf("MOVE GF@assignedVal GF@ret\n");
            printf("MOVE GF@ret nil@nil\n");
        }
        else if(strcmp(functionName, "readf") == 0){
            printf("CALL readf\n");
            printf("MOVE GF@assignedVal GF@ret\n");
            printf("MOVE GF@ret nil@nil\n");
        }
        else if(strcmp(functionName, "strlen") == 0){
            printf("CALL strlen\n");
            printf("MOVE GF@assignedVal GF@ret\n");
            printf("MOVE GF@ret nil@nil\n");
            argCount = 0; 
        }
        else if(strcmp(functionName, "strval") == 0){
            printf("CALL strval\n");
            printf("MOVE GF@assignedVal GF@ret\n");
            printf("MOVE GF@ret nil@nil\n");
            argCount = 0; 
        }
        else if(strcmp(functionName, "intval") == 0){
            printf("CALL intval\n");
            printf("MOVE GF@assignedVal GF@ret\n");
            printf("MOVE GF@ret nil@nil\n");
            argCount = 0; 
        }
        else if(strcmp(functionName, "floatval") == 0){
            printf("CALL floatval\n");
            printf("MOVE GF@assignedVal GF@ret\n");
            printf("MOVE GF@ret nil@nil\n");
            argCount = 0; 
        }
        else if(strcmp(functionName, "substring") == 0){
            printf("CALL substring\n");
            printf("MOVE GF@assignedVal GF@ret\n");
            printf("MOVE GF@ret nil@nil\n");
            argCount = 0; 
        }
        else if(strcmp(functionName, "ord") == 0){
            printf("CALL ord\n");
            printf("MOVE GF@assignedVal GF@ret\n");
            printf("MOVE GF@ret nil@nil\n");
            argCount = 0; 
        }
        else if(strcmp(functionName, "chr") == 0){
            printf("CALL chr\n");
            printf("MOVE GF@assignedVal GF@ret\n");
            printf("MOVE GF@ret nil@nil\n");
            argCount = 0; 
        }
        else{
            printf("CALL %s\n", functionName);
            printf("MOVE GF@assignedVal GF@ret\n");
            printf("MOVE GF@ret nil@nil\n");
            argCount = 0;

            //checks return type of func
            symbol_t *function = symtable_lookup(symtables->function_table, functionName);
            if(function -> func_ret_type == T_Keyword_Int){
                printf("MOVE GF@retType string@int\n");
                printf("TYPE GF@realRetType GF@assignedVal\n");
                printf("JUMPIFNEQ wrongParamReturn GF@retType GF@realRetType\n");
            }
            else if(function -> func_ret_type == T_Keyword_Float){
                printf("MOVE GF@retType string@float\n");
                printf("TYPE GF@realRetType GF@assignedVal\n");
                printf("JUMPIFNEQ wrongParamReturn GF@retType GF@realRetType\n");
            }
            else if(function -> func_ret_type == T_Keyword_String){
                printf("MOVE GF@retType string@string\n");
                printf("TYPE GF@realRetType GF@assignedVal\n");
                printf("JUMPIFNEQ wrongParamReturn GF@retType GF@realRetType\n");
            }
            else if(function -> func_ret_type == T_Keyword_Void){
                printf("MOVE GF@retType string@nil\n");
                printf("TYPE GF@realRetType GF@assignedVal\n");
                printf("JUMPIFNEQ wrongParamReturn GF@retType GF@realRetType\n");
            }
        }

    }
    else
    {
        HANDLE_ERROR = ERR_SYNTAX;
    }

        if(error != 0)
    {
        error_exit(error, ACTIVE_TOKEN);
        exit(error);
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

        if(error != 0)
    {
        error_exit(error, ACTIVE_TOKEN);
        exit(error);
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
        //;
        if (ACTIVE_TYPE == T_Semicolon)
        {
            HANDLE_ERROR = parseTerminal(tokens, T_Semicolon);
        }
        // =
        else
        {
            HANDLE_ERROR = parseTerminal(tokens, T_Assign);
            // <assign>
            HANDLE_ERROR = rule_Assign(tokens, symtables);
            // ;
            HANDLE_ERROR = parseTerminal(tokens, T_Semicolon);
            
            //CODEGEN var init and assign
            // do aktivni tabulky indexu vloz variable
            if(symtable_lookup(symtables -> vars_table_array[symtables->active_table_index], var->data) == NULL){
                symtable_insert(symtables -> vars_table_array[symtables->active_table_index], token_to_symbol(var));
            }

            // presun vysledek z exp_parseru do promenne
            printf("MOVE LF@%s GF@assignedVal\n", var->data);
            //END CODEGEN var init and assign
        }
    }
    // <stat> -> while ( <expr> ) { <st-list> }
    else if (ACTIVE_TYPE == T_Keyword_While)
    {
        //CODEGEN labels init
        char* while_label_end = make_random_label();
        char* while_label_begin = make_random_label();
        char* label_int = make_random_label();
        char* label_string = make_random_label();
        char* str_0 = make_random_label();
        char* skip_str = make_random_label();
        char* skip_int = make_random_label();
        char* skip_float = make_random_label();
        char* label_float = make_random_label();
        char* skip_not_boolean_vals = make_random_label();

        printf("LABEL %s\n", while_label_begin);
        //END CODEGEN labels init
    
        // while
        HANDLE_ERROR = parseTerminal(tokens, T_Keyword_While);
        // (
        HANDLE_ERROR = parseTerminal(tokens, T_L_r_par);
        // <expr>
        HANDLE_ERROR = rule_Expr(tokens, symtables);
        // )

        //CODEGEN WHILE -> BEGIN
         printf("MOVE GF@op4 bool@true\n");
        printf("TYPE GF@op1 GF@assignedVal\n");

        //handle of non boolean vars
        printf("JUMPIFEQ %s GF@op1 string@int\n", label_int);
        printf("JUMPIFEQ %s GF@op1 string@string\n", label_string);
        printf("JUMPIFEQ %s GF@op1 string@float\n", label_float);
        printf("JUMPIFEQ %s GF@op1 string@bool\n", skip_not_boolean_vals);

        // 0 in while -> false, otherwise true
        printf("JUMP %s\n", skip_int);
        printf("LABEL %s\n", label_int);
        printf("JUMPIFNEQ %s GF@assignedVal int@0\n", skip_int);
        printf("MOVE GF@op4 bool@false\n");
        printf("LABEL %s\n", skip_int);

        // empty string in while -> false, otherwise true
        printf("JUMP %s\n", skip_str);
        printf("LABEL %s\n", label_string);
        printf("JUMPIFEQ %s GF@assignedVal string@0\n", str_0);
        printf("JUMPIFNEQ %s GF@assignedVal string@\n", skip_str);
        printf("LABEL %s\n", str_0);
        printf("MOVE GF@op4 bool@false\n");
        printf("LABEL %s\n", skip_str);

        // 0.0 float in while -> false, otherwise true
        printf("JUMP %s\n", skip_float);
        printf("LABEL %s\n", label_float);
        printf("JUMPIFNEQ %s GF@assignedVal float@0x0p+0\n", skip_float);
        printf("MOVE GF@op4 bool@false\n");
        printf("LABEL %s\n", skip_float);

        // priradime spravnou value do assign
        printf("MOVE GF@assignedVal GF@op4\n");
        printf("LABEL %s\n", skip_not_boolean_vals);

        // null in while -> false
        printf("JUMPIFEQ %s GF@op1 string@nil\n", while_label_end);
        printf("JUMPIFEQ %s GF@assignedVal bool@false\n",while_label_end);

        //END CODEGEN WHILE -> BEGIN
        HANDLE_ERROR = parseTerminal(tokens, T_R_r_par);
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
        free(skip_int);
        free(skip_str);
        free(label_int);
        free(label_string);
        free(label_float);
        free(skip_float);
        free(skip_not_boolean_vals);
        free(str_0);
        //END CODEGEN WHILE -> END

    }
    // <stat> -> if ( <expr> ) { <st-list> } else { <st-list> }
    else if (ACTIVE_TYPE == T_Keyword_If)
    {
        //CODEGEN labels init
        char* if_label = make_random_label();
        char* else_label = make_random_label();
        char* label_int = make_random_label();
        char* label_string = make_random_label();
        char* str_0 = make_random_label();
        char* label_float = make_random_label();
        char* skip_str = make_random_label();
        char* skip_int = make_random_label();
        char* skip_float = make_random_label();
        char* skip_not_boolean_vals = make_random_label();

        //END CODEGEN

        // if
        HANDLE_ERROR = parseTerminal(tokens, T_Keyword_If);
        // (
        HANDLE_ERROR = parseTerminal(tokens, T_L_r_par);
        // <expr>
        HANDLE_ERROR = rule_Expr(tokens, symtables);
        // )
        HANDLE_ERROR = parseTerminal(tokens, T_R_r_par);

        //CODEGEN IF -> BEGIN
        printf("MOVE GF@op4 bool@true\n");
        printf("TYPE GF@op1 GF@assignedVal\n");

        //handle of non boolean vars
        printf("JUMPIFEQ %s GF@op1 string@int\n", label_int);
        printf("JUMPIFEQ %s GF@op1 string@string\n", label_string);
        printf("JUMPIFEQ %s GF@op1 string@bool\n", skip_not_boolean_vals);
        printf("JUMPIFEQ %s GF@op1 string@float\n", label_float);

        // 0 in if -> false, otherwise true
        printf("JUMP %s\n", skip_int);
        printf("LABEL %s\n", label_int);
        printf("JUMPIFNEQ %s GF@assignedVal int@0\n", skip_int);
        printf("MOVE GF@op4 bool@false\n");
        printf("LABEL %s\n", skip_int);

        // empty string in if -> false, otherwise true
        printf("JUMP %s\n", skip_str);
        printf("LABEL %s\n", label_string);
        printf("JUMPIFEQ %s GF@assignedVal string@0\n", str_0);
        printf("JUMPIFNEQ %s GF@assignedVal string@\n", skip_str);
        printf("LABEL %s\n", str_0);
        printf("MOVE GF@op4 bool@false\n");
        printf("LABEL %s\n", skip_str);

        // 0.0 float in while -> false, otherwise true
        printf("JUMP %s\n", skip_float);
        printf("LABEL %s\n", label_float);
        printf("JUMPIFNEQ %s GF@assignedVal float@0x0p+0\n", skip_float);
        printf("MOVE GF@op4 bool@false\n");
        printf("LABEL %s\n", skip_float);

        // priradime spravnou value do assign
        printf("MOVE GF@assignedVal GF@op4\n");
        printf("LABEL %s\n", skip_not_boolean_vals);
        
        // null in if -> false
        printf("JUMPIFEQ %s GF@op1 string@nil\n", if_label);
        printf("JUMPIFEQ %s GF@assignedVal bool@false\n", if_label); 
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
        free(skip_int);
        free(skip_str);
        free(skip_float);
        free(label_int);
        free(label_string);
        free(label_float);
        free(skip_not_boolean_vals);
        free(str_0);
        //END CODEGEN ELSE -> END
        
    }
    // <stat> -> return <expr> ;
    else if (ACTIVE_TYPE == T_Keyword_Return)
    {
        //return v mainu ukoncuje program a vraci 0
        if(symtables -> active_table_index == 0){ 
                printf("EXIT int@0\n");
        }
        
        
        hasReturn = true;
        // return
        HANDLE_ERROR = parseTerminal(tokens, T_Keyword_Return);
        // <expr> or ;
        if(ACTIVE_TYPE != T_Semicolon){
            
            // <expr>
            HANDLE_ERROR = rule_Expr(tokens, symtables);

            //presun vysledek z exp_parseru do navratove hodnoty
            printf("MOVE GF@ret GF@assignedVal\n");
            printf("POPFRAME\n");
            printf("RETURN\n");
            // ;
            HANDLE_ERROR = parseTerminal(tokens, T_Semicolon);

            //controls additional expression with return in void function
            if(symtables -> active_table_index != 0){
            symbol_t* curr_func = symtable_lookup(symtables -> function_table, functionName);
                if(curr_func -> func_ret_type == T_Keyword_Void){
                    error_exit(ERR_MISS_EXCESS_RET, ACTIVE_TOKEN);
                    exit(ERR_MISS_EXCESS_RET);
                }
            }
        }
        else if (ACTIVE_TYPE == T_Semicolon){
            
            // ;
            HANDLE_ERROR = parseTerminal(tokens, T_Semicolon);
            //presun nil do navratove hodnoty
            printf("MOVE GF@ret nil@nil\n");
            printf("POPFRAME\n");
            printf("RETURN\n");
            // controls insufficient expression with return in void function
            if(symtables -> active_table_index != 0){
                symbol_t* curr_func = symtable_lookup(symtables -> function_table, functionName);
                    if(curr_func -> func_ret_type != T_Keyword_Void){
                        error_exit(ERR_MISS_EXCESS_RET, ACTIVE_TOKEN);
                        exit(ERR_MISS_EXCESS_RET);
                    }
            }
        }
        else
        {
            HANDLE_ERROR = ERR_SYNTAX;
        }

    }
    // <stat> -> <expr> ;
    else if (ACTIVE_TYPE == T_Int || ACTIVE_TYPE == T_Float || ACTIVE_TYPE == T_String || ACTIVE_TYPE == T_Keyword_Null)
    {
        // <expr>
        HANDLE_ERROR = parseTerminal(tokens, ACTIVE_TYPE);
        // ;
        HANDLE_ERROR = parseTerminal(tokens, T_Semicolon);
    }
    // <stat> -> func-id ( <args> ) ;
    else if (ACTIVE_TYPE == T_Identifier)
    {
        functionName = ACTIVE_DATA;
        // func-id
        // Check if exists
        if (pass == 2 && symtable_lookup(symtables->function_table, functionName) == NULL
        && strcmp(functionName, "reads") != 0
        && strcmp(functionName, "readi") != 0
        && strcmp(functionName, "readf") != 0
        && strcmp(functionName, "write") != 0
        && strcmp(functionName, "floatval") != 0
        && strcmp(functionName, "intval") != 0
        && strcmp(functionName, "strval") != 0
        && strcmp(functionName, "strlen") != 0
        && strcmp(functionName, "substring") != 0
        && strcmp(functionName, "ord") != 0
        && strcmp(functionName, "chr") != 0)
        {
            error_exit(ERR_UNDEF_REDEF_FUN, ACTIVE_TOKEN);
            exit(ERR_UNDEF_REDEF_FUN);
        }
        HANDLE_ERROR = parseTerminal(tokens, T_Identifier);
        // (
        HANDLE_ERROR = parseTerminal(tokens, T_L_r_par);
        // <args>
        HANDLE_ERROR = rule_Args(tokens);
        // )
        HANDLE_ERROR = parseTerminal(tokens, T_R_r_par);
        // ;
        HANDLE_ERROR = parseTerminal(tokens, T_Semicolon);

        //CODEGEN volani funkce bez navratove hodnoty
        // vestavena funkce write potrebuje znat pocet passovanych argumentu
        if(strcmp(functionName, "write") == 0){
            printf("PUSHS int@%d\n", argCount);
            printf("CALL write\n");
            argCount = 0;
        }
        else{
            //check if correct num of args
            if (pass == 2){
                symbol *func = symtable_lookup(symtables->function_table, functionName);
                if (func != NULL && func -> func_param_count != argCount){
                    error_exit(ERR_WRONG_PARAM_RET, ACTIVE_TOKEN);
                    exit(ERR_WRONG_PARAM_RET);
                }
            }
            argCount = 0;
            printf("CALL %s\n", functionName);
        }

    }
    else
    {
        HANDLE_ERROR = ERR_SYNTAX;
    }

    if(error != 0)
    {
        error_exit(error, ACTIVE_TOKEN);
        exit(error);
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

        if(error != 0)
    {
        error_exit(error, ACTIVE_TOKEN);
        exit(error);
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

        if(error != 0)
    {
        error_exit(error, ACTIVE_TOKEN);
        exit(error);
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
    // kdyz nejsme ve funkci, tak chceme aktivni nultou tabulku symbolu (main tabulka)
    symtables -> active_table_index = 0;

    // <prog> -> <stat> <prog> .
    if (ACTIVE_TYPE == T_Var_id ||
        ACTIVE_TYPE == T_Keyword_While ||
        ACTIVE_TYPE == T_Keyword_If ||
        ACTIVE_TYPE == T_Keyword_Return ||
        ACTIVE_TYPE == T_Identifier ||
        ACTIVE_TYPE == T_Keyword_Int ||
        ACTIVE_TYPE == T_Keyword_Float ||
        ACTIVE_TYPE == T_Keyword_String ||
        ACTIVE_TYPE == T_Keyword_Null ||
        ACTIVE_TYPE == T_Int ||
        ACTIVE_TYPE == T_Float ||
        ACTIVE_TYPE == T_String)
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
        
        // Check for function redefinition
        if(pass == 1 && symtable_lookup(symtables->function_table, ACTIVE_DATA) != NULL)
        {
            error = error_exit(ERR_UNDEF_REDEF_FUN, ACTIVE_TOKEN);
            exit(error);
        }
        else{
            symtable_insert(symtables->function_table, token_to_symbol(ACTIVE_TOKEN));
        }


        //CODEGEN function body -> start
        functionName = ACTIVE_DATA;
        char* end_of_function = make_random_label();

        //kontrola redefinice vestavene funkce
        if (
           strcmp(functionName, "reads") == 0
        || strcmp(functionName, "readi") == 0
        || strcmp(functionName, "readf") == 0
        || strcmp(functionName, "write") == 0
        || strcmp(functionName, "floatval") == 0
        || strcmp(functionName, "intval") == 0
        || strcmp(functionName, "strval") == 0
        || strcmp(functionName, "strlen") == 0
        || strcmp(functionName, "substring") == 0
        || strcmp(functionName, "ord") == 0
        || strcmp(functionName, "chr") == 0)
        {
            error_exit(ERR_UNDEF_REDEF_FUN, ACTIVE_TOKEN);
            exit(ERR_UNDEF_REDEF_FUN);
        }

        // funkci musime preskocit pokud ji nevolame
        printf("JUMP %s\n", end_of_function);
        
        //labely by se nepreskocili end_of_scuffed_codegen jumpem, takze se nesmi vubec generovat v prvnim pruchodu
        if(pass == 2)
        {
            printf("LABEL %s\n", functionName);
        }

        printf("CREATEFRAME\n");
        printf("PUSHFRAME\n");

        // funkce ma vlastni lokalni defvary, proto ji priradime vlastni index v poli vars_table_array
        symtables -> function_table_index++;
        symtables -> active_table_index = symtables -> function_table_index; // chceme symtable aktualni funkce
        
        //pokud symtable s timto indexem neexistuje tak ji vytvor (osetreni druheho pruchodu)
        if(!symtables -> vars_table_array[symtables->active_table_index])
            symtables -> vars_table_array[symtables->active_table_index] = symtable_init(100);

        //vyprintuj vsechny defvary z daneho indexu, tzn pri druhem pruchodu bude na zacatku kazde funkce print vsech jejich defvaru
        symtable_defvar_print(symtables->vars_table_array[symtables->active_table_index]);
        //END CODEGEN function body -> start

        HANDLE_ERROR = parseTerminal(tokens, T_Identifier);
        // (
        HANDLE_ERROR = parseTerminal(tokens, T_L_r_par);
        // <params>
        HANDLE_ERROR = rule_Params(tokens, symtables);

        // Saves num of params into symtable
        symbol_t * current_function = symtable_lookup(symtables -> function_table, functionName);
        current_function -> func_param_count = paramCount;
        paramCount = 0;
        // )
        HANDLE_ERROR = parseTerminal(tokens, T_R_r_par);
        // :
        HANDLE_ERROR = parseTerminal(tokens, T_Colon);
        // type

        //saving function return type
        current_function -> func_ret_type = ACTIVE_TYPE;

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
        
        hasReturn = false;

        // {
        HANDLE_ERROR = parseTerminal(tokens, T_L_c_par);
        // <st-list>
        HANDLE_ERROR = rule_StList(tokens, symtables);
        // }
        HANDLE_ERROR = parseTerminal(tokens, T_R_c_par);

        //checks for missing return
        if(hasReturn == false && current_function -> func_ret_type != T_Keyword_Void)
        {
            error_exit(ERR_WRONG_PARAM_RET, ACTIVE_TOKEN);
            exit(ERR_WRONG_PARAM_RET);
        }

        //resets control variable
        functionName = NULL;
        hasReturn = false;

        //CODEGEN function body -> end
        printf("POPFRAME\n");
        printf("RETURN\n");
        printf("LABEL %s\n", end_of_function);
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

        if(error != 0)
    {
        error_exit(error, ACTIVE_TOKEN);
        exit(error);
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
int parser(token_list_t *tokens, Symtables* symtables, int whichPass)
{
    int error = 0;
    pass = whichPass;

    HANDLE_ERROR = checkProlog(tokens, symtables);
    if (error != 0)
        return 1;
    // Begin parsing
    HANDLE_ERROR = rule_Prog(tokens, symtables);

    return error;
}


void printBuiltIn(){

    printf(" \
JUMP end_of_program\n\
LABEL reads\n\
READ GF@ret string\n\
RETURN \n\
LABEL readi\n\
READ GF@ret int\n\
RETURN\n\
LABEL readf\n\
READ GF@ret float\n\
RETURN\n\
LABEL write\n\
CREATEFRAME\n\
PUSHFRAME\n\
DEFVAR LF@arg\n\
DEFVAR LF@count\n\
DEFVAR LF@i\n\
POPS LF@count\n\
MOVE LF@i int@0\n\
LABEL write_loop\n\
POPS LF@arg\n\
WRITE LF@arg\n\
ADD LF@i LF@i int@1\n\
JUMPIFNEQ write_loop LF@i LF@count\n\
POPFRAME\n\
CLEARS\n\
RETURN\n\
LABEL floatval\n\
CREATEFRAME\n\
PUSHFRAME\n\
DEFVAR LF@arg\n\
DEFVAR LF@type\n\
MOVE GF@ret float@0x0p+0\n\
POPS LF@arg\n\
TYPE LF@type LF@arg\n\
JUMPIFEQ floatval_float LF@type string@float\n\
JUMPIFEQ floatval_int LF@type string@int\n\
JUMPIFEQ floatval_end LF@type string@nil\n\
JUMP floatval_end\n\
LABEL floatval_float\n\
MOVE GF@ret LF@arg\n\
JUMP floatval_end\n\
LABEL floatval_int\n\
INT2FLOAT GF@ret LF@arg\n\
JUMP floatval_end\n\
LABEL floatval_end\n\
POPFRAME\n\
CLEARS\n\
RETURN\n\
LABEL intval\n\
CREATEFRAME\n\
PUSHFRAME\n\
DEFVAR LF@arg\n\
DEFVAR LF@type\n\
MOVE GF@ret int@0\n\
POPS LF@arg\n\
TYPE LF@type LF@arg\n\
JUMPIFEQ intval_int LF@type string@int\n\
JUMPIFEQ intval_float LF@type string@float\n\
JUMPIFEQ intval_nil LF@type string@nil\n\
JUMP floatval_end\n\
LABEL intval_int\n\
MOVE GF@ret LF@arg\n\
JUMP intval_end\n\
LABEL intval_float\n\
FLOAT2INT GF@ret LF@arg\n\
JUMP intval_end\n\
LABEL intval_end\n\
POPFRAME\n\
CLEARS\n\
RETURN\n\
LABEL strval\n\
CREATEFRAME\n\
PUSHFRAME\n\
DEFVAR LF@arg\n\
DEFVAR LF@type\n\
MOVE GF@ret string@\n\
POPS LF@arg\n\
TYPE LF@type LF@arg\n\
JUMPIFEQ strval_string LF@type string@string\n\
JUMPIFEQ strval_end LF@type string@nil\n\
JUMP floatval_end\n\
LABEL strval_string\n\
MOVE GF@ret LF@arg\n\
JUMP strval_end\n\
LABEL strval_end\n\
POPFRAME\n\
CLEARS\n\
RETURN\n\
LABEL strlen\n\
CREATEFRAME\n\
PUSHFRAME\n\
DEFVAR LF@arg\n\
POPS LF@arg\n\
STRLEN GF@ret LF@arg\n\
POPFRAME\n\
CLEARS\n\
RETURN\n\
LABEL substring\n\
CREATEFRAME\n\
PUSHFRAME\n\
DEFVAR LF@arg\n\
DEFVAR LF@arg2\n\
DEFVAR LF@arg3\n\
DEFVAR LF@len\n\
DEFVAR LF@i\n\
DEFVAR LF@char\n\
DEFVAR LF@error\n\
POPS LF@arg\n\
POPS LF@arg2\n\
POPS LF@arg3\n\
MOVE LF@i LF@arg2\n\
MOVE GF@ret string@\n\
STRLEN LF@len LF@arg\n\
LT LF@error LF@arg2 int@0\n\
LT LF@error LF@arg3 int@0\n\
GT LF@error LF@arg2 LF@arg3\n\
GT LF@error LF@arg2 LF@len\n\
EQ LF@error LF@arg2 LF@len\n\
GT LF@error LF@arg3 LF@len\n\
JUMPIFEQ substring_end LF@error bool@true\n\
GETCHAR LF@char LF@arg LF@i\n\
CONCAT GF@ret GF@ret LF@char\n\
JUMPIFEQ substring_end LF@i LF@arg3\n\
JUMP substring_loop\n\
LABEL substring_loop\n\
ADD LF@i LF@i int@1\n\
GETCHAR LF@char LF@arg LF@i\n\
CONCAT GF@ret GF@ret LF@char\n\
JUMPIFNEQ substring_loop LF@i LF@arg3\n\
JUMP substring_end\n\
LABEL substring_end\n\
POPFRAME\n\
CLEARS\n\
RETURN\n\
LABEL ord\n\
CREATEFRAME\n\
PUSHFRAME\n\
DEFVAR LF@arg\n\
POPS LF@arg\n\
STRI2INT GF@ret LF@arg int@0\n\
POPFRAME\n\
CLEARS\n\
RETURN\n\
LABEL chr\n\
CREATEFRAME\n\
PUSHFRAME\n\
DEFVAR LF@arg\n\
POPS LF@arg\n\
INT2CHAR GF@ret LF@arg\n\
POPFRAME\n\
CLEARS\n\
RETURN\n\
LABEL wrongParamReturn\n\
EXIT int@4\n\
LABEL end_of_program\n\
\n\
");
}