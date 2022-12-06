#ifndef PARSER_H
#define PARSER_H

#include "../include/lexer.h"
#include "../include/symtable.h"

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
    error           \

typedef enum err_code
{
   parser_err,
   lexer_err,
   semantic_err,
} ERR_CODE;

// err_type next_check(token_t* t);

// err_type next_check_type(token_type_t* token_type, token_type_t* valid_type);

int parser(token_list_t *tokens, Symtables* symtables, int whichPass);

int check_args(token_t *token);

int check_call_args(token_t *token);

void printBuiltIn();
#endif
