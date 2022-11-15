#ifndef PARSER_H
#define PARSER_H

#include "symtable.h"
#include "lexer.h"

typedef enum ERR_TYPE {
    ERR_PARSE,
    ERR_LEX,
    ERR_SEM
} err_type;

err_type next_check(token_t* t);

err_type next_check_type(token_type_t* token_type, token_type_t* valid_type);

void syntax_check(token_t* token);

#endif
