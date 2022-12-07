/**
 * @file parser.h
 *
 * Interface for parser and code generation
 *
 * IFJ project 2022
 *
 * @author <xstrel03> Matyáš Strelec
 * @author <xseidl06> Ondřej Seidl
 * @author <xklond00> Dominik Klon
 *
 */

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

void print_line_till_end(token_list_t *tokens);

char *make_random_label();

int parser(token_list_t *tokens, symtables_t *symtables, int whichPass);

void printBuiltIn();

#endif
