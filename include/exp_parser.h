#ifndef EXP_PARSER_H
#define EXP_PARSER_H

#include "../include/parser.h"

PTreeNode_t *parse_expression_with_tree(token_list_t *tokens, int min_precedence, PTreeNode_t *PTree);
int exp_parser(token_list_t *tokens);

#endif
