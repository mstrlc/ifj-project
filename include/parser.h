#ifndef PARSER_H
#define PARSER_H

#include "symtable.h"
#include "lexer.h"
#include "../include/lexer.h"
#include "../include/stack.h"

typedef enum err_code{
   parser_err,
   lexer_err,
   semantic_err,
   pass
}ERR_CODE;


// err_type next_check(token_t* t);

// err_type next_check_type(token_type_t* token_type, token_type_t* valid_type);

int prog(token_t* token, b_stack* stack);

int check_args(token_t *token);

int check_call_args(token_t *token);

char* make_random_string();
#endif
