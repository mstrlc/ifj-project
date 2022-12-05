#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/parse_tree.h"
#include "../include/exp_parser.h"
#include "../include/error.h"

PTreeNode_t *parse_expression_with_tree(token_list_t *tokens, int min_precedence, PTreeNode_t *PTree);

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
    case T_Int:
    case T_Float:
    case T_String:
    case T_Var_id:
        return 0;
    default:
        return -1;
    }
}

/**
 * @brief Parses the expression uses parse_expression() function to build parse tree
 *
 * @param tokens list of tokens
 * @return int EXIT_SUCCESS or EXIT_FAILURE
 */
int exp_parser(token_list_t *tokens)
{
    int error = EXIT_SUCCESS;
    PTreeNode_t *PTree = initPtree();
    PTree = parse_expression_with_tree(tokens, 1, PTree);\
    if(PTree != NULL)
    {
        printPtree(PTree);
        disposePtree(PTree);
    }
    else
    {
        error = EXIT_FAILURE;
    }
    return error;
    
} 

/**
 * @brief Parses an expression, builds a parse tree and returns it
 *
 * @param tokens list of tokens starting with the first token of the expression
 * @param min_precedence minimum precedence 
 * @param PTree initialized parse tree
 * @return PTreeNode_t*
 */
PTreeNode_t *parse_expression(token_list_t *tokens, int min_precedence, PTreeNode_t *PTree);

/**
 * @brief Parses an expression and builds a parse tree simultaneously
 * 
 * Use ACTIVE_NEXT to shift the active token
 * Use ACTIVE_PREV to shift the active token back
 * Use ACTIVE_TYPE to get the type of the active token
 * Use ACTIVE_TOKEN to acces the active token
 * Use insertLeftPtreeNode(Ptree, token) to insert a new node to the left of the active node
 * Use insertRightPtreeNode(Ptree, token) to insert a new node to the right of the active node
 * Use PTree->token = ACTIVE_TOKEN to set the token of the active node
 * use PTree->active = to set the active node to another node
 * use precedence(token_type_t operator) to get the precedence of an operator
 * use PTree = InitPtree() to initialize a new parse tree node
 * end if ACTIVE_TYPE != T_var_id or T_int or T_float or T_string or T_L_r_par or T_R_r_par or T_Smaller or T_Smaller_eq or T_Larger or T_Larger_eq or T_Not_equal or T_Equal or T_Concat or T_Mul or T_Div or T_Plus or T_Minus
 * 
 * @param tokens 
 * @param min_precedence 
 * @param PTree 
 * @return PTreeNode_t* 
 */


static int bracket_L_counter = 0;
PTreeNode_t *parse_expression_with_tree(token_list_t *tokens, int min_precedence, PTreeNode_t *PTree)
{
    //int error = EXIT_SUCCESS;
    PTreeNode_t *a = PTree;
    if(ACTIVE_TYPE == T_L_r_par){
        bracket_L_counter++;
    }
    else{
    a->token = ACTIVE_TOKEN;
    }
    ACTIVE_NEXT;
    if(ACTIVE_TYPE == T_Plus || ACTIVE_TYPE == T_Minus || ACTIVE_TYPE == T_Mul || ACTIVE_TYPE == T_Div || ACTIVE_TYPE == T_Concat || ACTIVE_TYPE == T_Not_equal || ACTIVE_TYPE == T_Equal || ACTIVE_TYPE == T_Larger || ACTIVE_TYPE == T_Larger_eq || ACTIVE_TYPE == T_Smaller || ACTIVE_TYPE == T_Smaller_eq){
        while (1){
            if(ACTIVE_TYPE == T_Plus || ACTIVE_TYPE == T_Minus || ACTIVE_TYPE == T_Mul || ACTIVE_TYPE == T_Div || ACTIVE_TYPE == T_Concat || ACTIVE_TYPE == T_Not_equal || ACTIVE_TYPE == T_Equal || ACTIVE_TYPE == T_Larger || ACTIVE_TYPE == T_Larger_eq || ACTIVE_TYPE == T_Smaller || ACTIVE_TYPE == T_Smaller_eq){
                PTreeNode_t *op = initPtree();
                op->token = ACTIVE_TOKEN;
                ACTIVE_NEXT;
                PTreeNode_t *b = initPtree();
                if(ACTIVE_TYPE == T_L_r_par){
                    ACTIVE_NEXT;
                    bracket_L_counter++;
                    b = parse_expression_with_tree(tokens, min_precedence, b);
                }
                else{
                    b->token = ACTIVE_TOKEN;
                }
                a = makeOpNode(a, b, op);
                ACTIVE_NEXT;
            }
            else{
                return a;
            }
        }
    }
    else if (ACTIVE_TYPE == T_R_r_par){
        bracket_L_counter--;
        if(bracket_L_counter < 0){
            printf("Missing right bracket");
            return NULL;
        }
        return a;
    }
    else{
    return a;
    }
}
