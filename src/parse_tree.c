/**
 * @file parse_tree.c
 * 
 * Implementation of parse tree used in expression parser and also code generator
 * 
 * IFJ project 2022
 * 
 * @author Ondřej Seidl xseidl06
 * @author Dominik Klon xklond00
 * 
 */

#include <stdio.h>
#include <stdlib.h>

#include "../include/parse_tree.h"
#include "../include/symtable.h"
#include "../include/error.h"

/**
 * @brief Initializes a new parse tree node
 *
 * @return PTreeNode_t* pointer to the new parse tree node
 */
PTreeNode_t *initPtree()
{
    PTreeNode_t *ptree = malloc(sizeof(PTreeNode_t));
    ptree->token = NULL;
    ptree->left = NULL;
    ptree->right = NULL;
    ptree->active = NULL;
    return ptree;
}

/**
 * @brief frees the memory allocated for the parse tree
 *
 * @param ptree pointer to the parse tree node to be disposed of
 */
void disposePtree(PTreeNode_t *ptree)
{
    if (ptree->left != NULL)
    {
        disposePtree(ptree->left);
    }
    if (ptree->right != NULL)
    {
        disposePtree(ptree->right);
    }
    free(ptree);
}

/**
 * @brief Inserts a new node to the left of the active node
 *
 * @param ptree pointer to the current node
 * @param token pointer to the token to be inserted
 */
PTreeNode_t *insertLeftPtreeNode(PTreeNode_t *active, token_t *token)
{
    PTreeNode_t *newNode = initPtree();
    newNode->token = token;
    active->left = newNode;
    return newNode;
}

/**
 * @brief Creates a new node with the operator as root and operands as children 
 *
 * @param left pointer to the left child node
 * @param right pointer to the right child node
 * @param op pointer to the operator node
 * @return PTreeNode_t* pointer to the new node
 */
PTreeNode_t *makeOpNode(PTreeNode_t *left, PTreeNode_t *right, PTreeNode_t *op)
{
    PTreeNode_t *newNode = initPtree();
    newNode->token = op->token;
    newNode->left = left;
    newNode->right = right;
    return newNode;
}

/**
 * @brief Inserts a new node to the right of the active node
 *
 * @param ptree pointer to the current node
 * @param token pointer to the token to be inserted
 */
PTreeNode_t *insertRightPtreeNode(PTreeNode_t *active, token_t *token)
{
    PTreeNode_t *newNode = initPtree();
    newNode->token = token;
    active->right = newNode;
    return newNode;
}

/**
 * @brief Deletes a node from the parse tree
 *
 * @param ptree pointer to the node to be deleted
 * @param token pointer to the token to be deleted
 */
void deletePtreeNode(PTreeNode_t *ptree, token_t *token)
{
    if (ptree->left != NULL)
    {
        if (ptree->left->token->type == token->type)
        {
            disposePtree(ptree->left);
            ptree->left = NULL;
        }
        else
        {
            deletePtreeNode(ptree->left, token);
        }
    }
    if (ptree->right != NULL)
    {
        if (ptree->right->token->type == token->type)
        {
            disposePtree(ptree->right);
            ptree->right = NULL;
        }
        else
        {
            deletePtreeNode(ptree->right, token);
        }
    }
}

/**
 * @brief Prints a node of the parse tree
 *
 * @param ptree pointer to the node to be printed
 */
void printPtreeNode(PTreeNode_t *ptree)
{
    if (ptree->token->data != NULL)
        printf("%s", ptree->token->data);
    printToken(ptree->token);
}

/**
 * @brief Prints the parse tree
 *
 * @param ptree pointer to the root of the parse tree
 */
void printPtree(PTreeNode_t *ptree, Symtables *symtables)
{
    if (ptree->left != NULL)
    {
        printPtree(ptree->left, symtables);
    }
    if (ptree->right != NULL)
    {
        printPtree(ptree->right, symtables);
    }
    if (ptree->token != NULL)
    {
         if(ptree->token->type == T_Var_id)
        {
            //check if variable is defined
            if (symtable_lookup(symtables -> vars_table_array[symtables -> active_table_index], ptree->token->data) == NULL){
                error_exit(ERR_UNDEF_VAR, ptree->token);
                exit(ERR_UNDEF_VAR);
            }
            printf("PUSHS LF@%s\n", ptree->token->data);   
        }
        else if(ptree->token->type == T_Int)
        {
            printf("PUSHS int@%s\n", ptree->token->data);
        }
        else if(ptree->token->type == T_Float){
            printf("PUSHS float@%a\n", atof(ptree->token->data));
        }
        else if(ptree->token->type == T_String){
            printf("PUSHS string@%s\n", ptree->token->data);
        }
        else if(ptree->token->type == T_Keyword_Null){
            printf("PUSHS nil@nil\n");
        }
        else
        {
            if(ptree->token->type == T_Plus)
            {
                printf("ADDS\n");
            }
            else if(ptree->token->type == T_Minus)
            {
                printf("SUBS\n");
            }
            else if(ptree->token->type == T_Mul)
            {
                printf("MULS\n");
            }
            else if(ptree->token->type == T_Div)
            {
                // printf("INT2FLOATS\n");
                // printf("POPS GF@op1\n");
                // printf("INT2FLOATS\n");
                // printf("POPS GF@op2\n");
                // printf("DIV GF@op1 GF@op1 GF@op2\n");
                // printf("PUSHS GF@op1\n");
                printf("IDIVS\n");
            }
            else if(ptree->token->type == T_Equal)
            {
                printf("EQS\n");
            }
            else if(ptree->token->type == T_Not_equal)
            {
                printf("EQS\n");
                printf("NOTS\n");
                /*
                POP op1
                POP op2
                EQ op1 op1 op2
                PUSH op1
                */
            }
            else if(ptree->token->type == T_Smaller)
            {
                printf("LTS\n");
            }
            else if(ptree->token->type == T_Smaller_eq)
            {
                printf("GTS\n");
                printf("NOTS\n");
            }
            else if(ptree->token->type == T_Larger)
            {
                printf("GTS\n");
            }
            else if(ptree->token->type == T_Larger_eq)
            {
                printf("LTS\n");
                printf("NOTS\n");
            }
            else if(ptree->token->type == T_Concat)
            {
                printf("POPS GF@op1\n");
                printf("POPS GF@op2\n");
                printf("CONCAT GF@op1 GF@op1 GF@op2\n");
                printf("PUSHS GF@op1\n");
            }
        }
    }
}
