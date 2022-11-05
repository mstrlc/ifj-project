#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "parser.h"


err_type next_check(token_t* t){
    if(!getNextToken(t)){
        return ERR_LEX;
    }
}

err_type next_check_type(token_type_t* token_type, token_type_t* valid_type){
    if (token_type != valid_type){
        return ERR_PARSE;
    }
}

void syntax_check(token_t* token){
    switch(token->type){
        case T_Identifier:
            next_check(token);
            next_check_type(token->type, T_Assign);
        default:
            return 0;
    }
}