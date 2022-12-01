#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/parse_tree.h"
#include "../include/exp_parser.h"
#include "../include/error.h"

/**
 * @brief Returns the precedence of the given operator
 *
 * Higher precedence means the operator is evaluated first
 *
 * @param int precedence
 * @return int
 */
int precedence(token_type_t operator)
{
    switch (operator)
    {
    case T_Mul:
    case T_Div:
        return 4;
    case T_Plus:
    case T_Minus:
        return 3;
    case T_Larger:
    case T_Larger_eq:
    case T_Smaller:
    case T_Smaller_eq:
        return 2;
    case T_Concat:
    case T_Not_equal:
    case T_Equal:
        return 1;
    default:
        return -1;
    }
}

/**
 * @brief Builds a expresion parse tree from the given token list
 *
 *
 *
 * @param tokens list of tokens
 * @return int EXIT_SUCCESS or EXIT_FAILURE
 */
int exp_parser(token_list_t *tokens)
{
    int error = 0;
    PTreeNode_t *PTree = initPtree();
    if (ACTIVE_TYPE == T_Int || ACTIVE_TYPE == T_Exp || ACTIVE_TYPE == T_Float || ACTIVE_TYPE == T_String || ACTIVE_TYPE == T_L_r_par || ACTIVE_TYPE == T_Var_id)
    {
        PTree = parse_expression(tokens, 1, PTree);
        printPtree(PTree);
        return 0;
    }
}

/**
 * @brief Parses an expression
 *
 * Goes through the token list and builds a parse tree from it
 *
 * @param tokens list of tokens
 * @param min_precedence minimum precedence of the operator
 * @param PTree parse tree
 * @return PTreeNode_t*
 */
PTreeNode_t *parse_expression(token_list_t *tokens, int min_precedence, PTreeNode_t *PTree)
{
    insertLeftPtreeNode(PTree, ACTIVE_TOKEN);
    ACTIVE_NEXT;
    while (precedence(ACTIVE_TYPE) >= min_precedence)
    {
        token_type_t op = ACTIVE_TYPE;
        ACTIVE_NEXT;
        PTreeNode_t *tmp = initPtree();
        insertRightPtreeNode(PTree, tmp);
        while (precedence(ACTIVE_TYPE) > precedence(op))
        {
            tmp = parse_expression(tokens, precedence(op) + 1, tmp);
        }
        insertLeftPtreeNode(tmp, ACTIVE_TOKEN);
        ACTIVE_NEXT;
    }
    return PTree;
}
