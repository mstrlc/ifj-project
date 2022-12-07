/**
 * @file exp_parser.h
 *
 * Interface of expression parser
 *
 * IFJ project 2022
 *
 * @author <xseidl06> Ondřej Seidl
 *
 */

#ifndef EXP_PARSER_H
#define EXP_PARSER_H

#include "../include/parser.h"
#include "../include/symtable.h"

PTreeNode_t *parse_expression_with_tree(token_list_t *tokens, int min_precedence, PTreeNode_t *PTree);

int exp_parser(token_list_t *tokens, symtables_t *symtables);

#endif
