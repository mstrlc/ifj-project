/**
 * @file exp_parser.h
 * @author Ondřej Seidl xseidl06
 * @brief Interface of expression parser
 * IFJ project 2022
 */

#ifndef EXP_PARSER_H
#define EXP_PARSER_H

#include "../include/parser.h"
#include "../include/symtable.h"

PTreeNode_t *parse_expression_with_tree(token_list_t *tokens, int min_precedence, PTreeNode_t *PTree);
int exp_parser(token_list_t *tokens, Symtables *symtables);


#endif
