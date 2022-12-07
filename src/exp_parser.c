/**
 * @file exp_parser.c
 * @author Ondřej Seidl xseidl06
 * @brief Implementation of expression parser
 * IFJ project 2022
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/parse_tree.h"
#include "../include/exp_parser.h"
#include "../include/error.h"
#include "../include/symtable.h"

/**
 * @brief Returns the precedence of the given operator
 *
 * Higher precedence means the operator is evaluated first
 *
 * @param operator token type of the operator
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
    case T_Keyword_Null:
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
int exp_parser(token_list_t *tokens, Symtables *symtables)
{
    int error = EXIT_SUCCESS;
    PTreeNode_t *PTree = initPtree();
    PTree = parse_expression_with_tree(tokens, 1, PTree);\
    if(PTree != NULL)
    {
        printPtree(PTree, symtables);
        printf("POPS GF@assignedVal\n"); 
        disposePtree(PTree);
    }
    else
    {
        error = 2;
    }
    return error;
    
} 

//bracelet_L_counter is used to count the number of left brackets in the expression
static int bracket_L_counter = 0;
/**
 * @brief Parses an expression and builds a parse tree simultaneously
 * 
 * @param tokens 
 * @param min_precedence 
 * @param PTree 
 * @return PTreeNode_t* 
 */
PTreeNode_t *parse_expression_with_tree(token_list_t *tokens, int min_precedence, PTreeNode_t *PTree)
{
    //Setting the precedence as the highest possible for the first operator
    int prev_prec = 4;
    int currernt_prec = 0;
    //Getting the first operand
    PTreeNode_t *a = PTree;
    if(ACTIVE_TYPE == T_L_r_par)
    {
        bracket_L_counter++;
        ACTIVE_NEXT;
        a = parse_expression_with_tree(tokens, min_precedence, a);
    }
    else
    {
        a->token = ACTIVE_TOKEN;
    }
    ACTIVE_NEXT;
    //condition for the end of the expression
    if(ACTIVE_TYPE == T_Plus || ACTIVE_TYPE == T_Minus || ACTIVE_TYPE == T_Mul || ACTIVE_TYPE == T_Div || ACTIVE_TYPE == T_Concat || ACTIVE_TYPE == T_Not_equal || ACTIVE_TYPE == T_Equal || ACTIVE_TYPE == T_Larger || ACTIVE_TYPE == T_Larger_eq || ACTIVE_TYPE == T_Smaller || ACTIVE_TYPE == T_Smaller_eq)
    {
        //while loop ended by returns
        while (1)
        {
            if(ACTIVE_TYPE == T_Plus || ACTIVE_TYPE == T_Minus || ACTIVE_TYPE == T_Mul || ACTIVE_TYPE == T_Div || ACTIVE_TYPE == T_Concat || ACTIVE_TYPE == T_Not_equal || ACTIVE_TYPE == T_Equal || ACTIVE_TYPE == T_Larger || ACTIVE_TYPE == T_Larger_eq || ACTIVE_TYPE == T_Smaller || ACTIVE_TYPE == T_Smaller_eq)
            {
                //Getting the operator
                PTreeNode_t *op = initPtree();
                op->token = ACTIVE_TOKEN;
                op->token->type = ACTIVE_TYPE;
                currernt_prec = precedence(ACTIVE_TYPE);

                ACTIVE_NEXT;

                //Getting the second operand
                PTreeNode_t *b = initPtree();
                if(ACTIVE_TYPE == T_Plus || ACTIVE_TYPE == T_Minus || ACTIVE_TYPE == T_Mul || ACTIVE_TYPE == T_Div || ACTIVE_TYPE == T_Concat || ACTIVE_TYPE == T_Not_equal || ACTIVE_TYPE == T_Equal || ACTIVE_TYPE == T_Larger || ACTIVE_TYPE == T_Larger_eq || ACTIVE_TYPE == T_Smaller || ACTIVE_TYPE == T_Smaller_eq)
                {
                    return NULL;        
                }
                //if the next token is a left bracket, the function is called recursively
                if(ACTIVE_TYPE == T_L_r_par)
                {
                    ACTIVE_NEXT;
                    bracket_L_counter++;
                    b = parse_expression_with_tree(tokens, min_precedence, b);
                    a = makeOpNode(a, b, op);
                }
                //if the current operator has higher precedence
                else if(currernt_prec > prev_prec)
                {
                    b->token = ACTIVE_TOKEN;
                    a->right = makeOpNode(a->right, b, op);
                }
                //if the current operator has lower precedence
                else
                {
                    b->token = ACTIVE_TOKEN;
                    a = makeOpNode(a, b, op);
                }
                prev_prec = currernt_prec;
                ACTIVE_NEXT;
            }
            //if the next token is a right bracket
            else if(ACTIVE_TYPE == T_R_r_par)
            {
                bracket_L_counter--;
                ACTIVE_NEXT;
                //checks if the next token is a semicolon
                if(ACTIVE_TYPE == T_Semicolon)
                {
                    //checks if it is and all of the brackets are closed, the function returns the parse tree
                    if(bracket_L_counter == 0)
                    {
                        ACTIVE_PREV;
                        return a;
                    }   
                    else
                    {
                        ACTIVE_PREV
                        return NULL;
                    }
                }
                else
                {
                    ACTIVE_PREV;
                    return a;
                }
            }
            else
            {
                return a;
            }
        }
    }
    else
    {
        return a;
    }
}
