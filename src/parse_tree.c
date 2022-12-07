/**
 * @file parse_tree.c
 *
 * Implementation of parse tree used in expression parser and also code generator
 *
 * IFJ project 2022
 *
 * @author <xseidl06> Ondřej Seidl
 * @author <xklond00> Dominik Klon
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
        if (ptree->token->type == T_Var_id)
        {
            // check if variable is defined
            if (symtable_lookup(symtables->vars_table_array[symtables->active_table_index], ptree->token->data) == NULL)
            {
                error_exit(ERR_UNDEF_VAR, ptree->token);
                exit(ERR_UNDEF_VAR);
            }
            printf("PUSHS LF@%s\n", ptree->token->data);
        }
        else if (ptree->token->type == T_Int)
        {
            printf("PUSHS int@%s\n", ptree->token->data);
        }
        else if (ptree->token->type == T_Float)
        {
            printf("PUSHS float@%a\n", atof(ptree->token->data));
        }
        else if (ptree->token->type == T_String)
        {
            printf("PUSHS string@%s\n", ptree->token->data);
        }
        else if (ptree->token->type == T_Keyword_Null)
        {
            printf("PUSHS nil@nil\n");
        }
        else
        {
            if (ptree->token->type == T_Plus)
            {
                char *label1 = make_random_label();
                char *label3 = make_random_label();
                char *label4 = make_random_label();
                char *label5 = make_random_label();
                char *label6 = make_random_label();
                char *label7 = make_random_label();

                printf("POPS GF@op1\n");
                printf("TYPE GF@op3 GF@op1\n");
                printf("JUMPIFEQ %s GF@op3 string@nil\n", label3);
                printf("JUMPIFEQ %s GF@op3 string@string\n", label3);

                printf("POPS GF@op2\n");
                printf("TYPE GF@op4 GF@op2\n");
                printf("JUMPIFEQ %s GF@op4 string@nil\n", label3);
                printf("JUMPIFEQ %s GF@op4 string@string\n", label3);

                printf("JUMPIFEQ %s GF@op3 GF@op4\n", label1); // za touhle podminkou vime ze op3 a op4 nejsou stejneho typu

                printf("JUMPIFEQ %s GF@op3 string@float\n", label5);
                printf("JUMPIFEQ %s GF@op4 string@float\n", label5); // skocime na dany label pokud je jeden z operandu float

                printf("JUMP %s\n", label1);
                printf("LABEL %s\n", label5);

                printf("JUMPIFEQ %s GF@op3 string@float\n", label6);
                printf("INT2FLOAT GF@op1 GF@op1\n");
                printf("LABEL %s\n", label6);

                printf("JUMPIFEQ %s GF@op4 string@float\n", label7);
                printf("INT2FLOAT GF@op2 GF@op2\n");
                printf("LABEL %s\n", label7);

                printf("LABEL %s\n", label1);
                printf("ADD GF@op1 GF@op1 GF@op2\n");
                printf("PUSHS GF@op1\n");

                printf("JUMP %s\n", label4);
                printf("LABEL %s\n", label3);
                printf("EXIT int@7\n");
                printf("LABEL %s\n", label4);

                free(label1);
                free(label3);
                free(label4);
                free(label5);
                free(label6);
                free(label7);
            }
            else if (ptree->token->type == T_Minus)
            {
                char *label1 = make_random_label();
                char *label3 = make_random_label();
                char *label4 = make_random_label();
                char *label5 = make_random_label();
                char *label6 = make_random_label();
                char *label7 = make_random_label();

                printf("POPS GF@op1\n");
                printf("TYPE GF@op3 GF@op1\n");
                printf("JUMPIFEQ %s GF@op3 string@nil\n", label3);
                printf("JUMPIFEQ %s GF@op3 string@string\n", label3);

                printf("POPS GF@op2\n");
                printf("TYPE GF@op4 GF@op2\n");
                printf("JUMPIFEQ %s GF@op4 string@nil\n", label3);
                printf("JUMPIFEQ %s GF@op4 string@string\n", label3);

                printf("JUMPIFEQ %s GF@op3 GF@op4\n", label1); // za touhle podminkou vime ze op3 a op4 nejsou stejneho typu

                printf("JUMPIFEQ %s GF@op3 string@float\n", label5);
                printf("JUMPIFEQ %s GF@op4 string@float\n", label5); // skocime na dany label pokud je jeden z operandu float

                printf("JUMP %s\n", label1);
                printf("LABEL %s\n", label5);

                printf("JUMPIFEQ %s GF@op3 string@float\n", label6);
                printf("INT2FLOAT GF@op1 GF@op1\n");
                printf("LABEL %s\n", label6);

                printf("JUMPIFEQ %s GF@op4 string@float\n", label7);
                printf("INT2FLOAT GF@op2 GF@op2\n");
                printf("LABEL %s\n", label7);

                printf("LABEL %s\n", label1);
                printf("SUB GF@op1 GF@op2   GF@op1\n");
                printf("PUSHS GF@op1\n");

                printf("JUMP %s\n", label4);
                printf("LABEL %s\n", label3);
                printf("EXIT int@7\n");
                printf("LABEL %s\n", label4);

                free(label1);
                free(label3);
                free(label4);
                free(label5);
                free(label6);
                free(label7);
            }
            else if (ptree->token->type == T_Mul)
            {
                char *label1 = make_random_label();
                char *label3 = make_random_label();
                char *label4 = make_random_label();
                char *label5 = make_random_label();
                char *label6 = make_random_label();
                char *label7 = make_random_label();

                printf("POPS GF@op1\n");
                printf("TYPE GF@op3 GF@op1\n");
                printf("JUMPIFEQ %s GF@op3 string@nil\n", label3);
                printf("JUMPIFEQ %s GF@op3 string@string\n", label3);

                printf("POPS GF@op2\n");
                printf("TYPE GF@op4 GF@op2\n");
                printf("JUMPIFEQ %s GF@op4 string@nil\n", label3);
                printf("JUMPIFEQ %s GF@op4 string@string\n", label3);

                printf("JUMPIFEQ %s GF@op3 GF@op4\n", label1); // za touhle podminkou vime ze op3 a op4 nejsou stejneho typu

                printf("JUMPIFEQ %s GF@op3 string@float\n", label5);
                printf("JUMPIFEQ %s GF@op4 string@float\n", label5); // skocime na dany label pokud je jeden z operandu float

                printf("JUMP %s\n", label1);
                printf("LABEL %s\n", label5);

                printf("JUMPIFEQ %s GF@op3 string@float\n", label6);
                printf("INT2FLOAT GF@op1 GF@op1\n");
                printf("LABEL %s\n", label6);

                printf("JUMPIFEQ %s GF@op4 string@float\n", label7);
                printf("INT2FLOAT GF@op2 GF@op2\n");
                printf("LABEL %s\n", label7);

                printf("LABEL %s\n", label1);
                printf("MUL GF@op1 GF@op1 GF@op2\n");
                printf("PUSHS GF@op1\n");

                printf("JUMP %s\n", label4);
                printf("LABEL %s\n", label3);
                printf("EXIT int@7\n");
                printf("LABEL %s\n", label4);

                free(label1);
                free(label3);
                free(label4);
                free(label5);
                free(label6);
                free(label7);
            }
            else if (ptree->token->type == T_Div)
            {
                char *label = make_random_label();
                char *label2 = make_random_label();
                char *label3 = make_random_label();
                char *label4 = make_random_label();

                printf("POPS GF@op1\n");
                printf("TYPE GF@op3 GF@op1\n");
                printf("JUMPIFEQ %s GF@op3 string@nil\n", label3);
                printf("JUMPIFEQ %s GF@op3 string@string\n", label3);
                printf("JUMPIFEQ %s GF@op3 string@float\n", label);
                printf("INT2FLOAT GF@op1 GF@op1\n");
                printf("LABEL %s\n", label);
                printf("POPS GF@op2\n");
                printf("TYPE GF@op3 GF@op2\n");
                printf("JUMPIFEQ %s GF@op3 string@nil\n", label3);
                printf("JUMPIFEQ %s GF@op3 string@string\n", label3);
                printf("JUMPIFEQ %s GF@op3 string@float\n", label2);
                printf("INT2FLOAT GF@op2 GF@op2\n");
                printf("LABEL %s\n", label2);

                printf("DIV GF@op1 GF@op2 GF@op1\n");
                printf("PUSHS GF@op1\n");
                printf("JUMP %s\n", label4);
                printf("LABEL %s\n", label3);
                printf("EXIT int@7\n");
                printf("LABEL %s\n", label4);

                free(label);
                free(label2);
                free(label3);
                free(label4);
            }
            else if (ptree->token->type == T_Equal)
            {
                char *error_label = make_random_label();
                char *label1 = make_random_label();
                char *end_label = make_random_label();

                printf("POPS GF@op1\n");
                printf("TYPE GF@op3 GF@op1\n");
                printf("POPS GF@op2\n");
                printf("TYPE GF@op4 GF@op2\n");
                printf("JUMPIFNEQ %s GF@op3 GF@op4\n", error_label);

                printf("JUMP %s\n", label1);
                printf("LABEL %s\n", error_label);
                printf("PUSHS bool@false\n");
                printf("JUMP %s\n", end_label);
                printf("LABEL %s\n", label1);

                printf("EQ GF@op1 GF@op2 GF@op1\n");

                printf("PUSHS GF@op1\n");
                printf("LABEL %s\n", end_label);

                free(error_label);
                free(label1);
                // printf("EQS\n");
            }
            else if (ptree->token->type == T_Not_equal)
            {
                char *error_label = make_random_label();
                char *label1 = make_random_label();
                char *end_label = make_random_label();

                printf("POPS GF@op1\n");
                printf("TYPE GF@op3 GF@op1\n");
                printf("POPS GF@op2\n");
                printf("TYPE GF@op4 GF@op2\n");
                printf("JUMPIFNEQ %s GF@op3 GF@op4\n", error_label);

                printf("JUMP %s\n", label1);
                printf("LABEL %s\n", error_label);
                printf("PUSHS bool@true\n");
                printf("JUMP %s\n", end_label);
                printf("LABEL %s\n", label1);

                printf("EQ GF@op1 GF@op2 GF@op1\n");
                printf("NOT GF@op1 GF@op1\n");

                printf("PUSHS GF@op1\n");
                printf("LABEL %s\n", end_label);

                free(error_label);
                free(label1);
                // printf("EQS\n");
                // printf("NOTS");
            }
            else if (ptree->token->type == T_Smaller)
            {
                char *label1 = make_random_label();
                char *label3 = make_random_label();
                char *label4 = make_random_label();
                char *label5 = make_random_label();
                char *label6 = make_random_label();
                char *label7 = make_random_label();

                printf("POPS GF@op1\n");
                printf("TYPE GF@op3 GF@op1\n");
                printf("JUMPIFEQ %s GF@op3 string@nil\n", label3);
                printf("JUMPIFEQ %s GF@op3 string@string\n", label3);

                printf("POPS GF@op2\n");
                printf("TYPE GF@op4 GF@op2\n");
                printf("JUMPIFEQ %s GF@op3 string@nil\n", label3);
                printf("JUMPIFEQ %s GF@op3 string@string\n", label3);

                printf("JUMPIFEQ %s GF@op3 GF@op4\n", label1); // za touhle podminkou vime ze op3 a op4 nejsou stejneho typu

                printf("JUMPIFEQ %s GF@op3 string@float\n", label5);
                printf("JUMPIFEQ %s GF@op4 string@float\n", label5); // skocime na dany label pokud je jeden z operandu float

                printf("JUMP %s\n", label1);
                printf("LABEL %s\n", label5);

                printf("JUMPIFEQ %s GF@op3 string@float\n", label6);
                printf("INT2FLOAT GF@op1 GF@op1\n");
                printf("LABEL %s\n", label6);

                printf("JUMPIFEQ %s GF@op4 string@float\n", label7);
                printf("INT2FLOAT GF@op2 GF@op2\n");
                printf("LABEL %s\n", label7);

                printf("LABEL %s\n", label1);
                printf("LT GF@op1 GF@op2 GF@op1\n");
                printf("PUSHS GF@op1\n");

                printf("JUMP %s\n", label4);
                printf("LABEL %s\n", label3);
                printf("EXIT int@7\n");
                printf("LABEL %s\n", label4);

                free(label1);
                free(label3);
                free(label4);
                free(label5);
                free(label6);
                free(label7);
            }
            else if (ptree->token->type == T_Smaller_eq)
            {
                char *label1 = make_random_label();
                char *label3 = make_random_label();
                char *label4 = make_random_label();
                char *label5 = make_random_label();
                char *label6 = make_random_label();
                char *label7 = make_random_label();

                printf("POPS GF@op1\n");
                printf("TYPE GF@op3 GF@op1\n");
                printf("JUMPIFEQ %s GF@op3 string@nil\n", label3);
                printf("JUMPIFEQ %s GF@op3 string@string\n", label3);

                printf("POPS GF@op2\n");
                printf("TYPE GF@op4 GF@op2\n");
                printf("JUMPIFEQ %s GF@op3 string@nil\n", label3);
                printf("JUMPIFEQ %s GF@op3 string@string\n", label3);

                printf("JUMPIFEQ %s GF@op3 GF@op4\n", label1); // za touhle podminkou vime ze op3 a op4 nejsou stejneho typu

                printf("JUMPIFEQ %s GF@op3 string@float\n", label5);
                printf("JUMPIFEQ %s GF@op4 string@float\n", label5); // skocime na dany label pokud je jeden z operandu float

                printf("JUMP %s\n", label1);
                printf("LABEL %s\n", label5);

                printf("JUMPIFEQ %s GF@op3 string@float\n", label6);
                printf("INT2FLOAT GF@op1 GF@op1\n");
                printf("LABEL %s\n", label6);

                printf("JUMPIFEQ %s GF@op4 string@float\n", label7);
                printf("INT2FLOAT GF@op2 GF@op2\n");
                printf("LABEL %s\n", label7);

                printf("LABEL %s\n", label1);
                printf("GT GF@op1 GF@op2 GF@op1\n");
                printf("NOT GF@op1 GF@op1\n");
                printf("PUSHS GF@op1\n");

                printf("JUMP %s\n", label4);
                printf("LABEL %s\n", label3);
                printf("EXIT int@7\n");
                printf("LABEL %s\n", label4);

                free(label1);
                free(label3);
                free(label4);
                free(label5);
                free(label6);
                free(label7);
                // printf("GTS\n");
                // printf("NOTS\n");
            }
            else if (ptree->token->type == T_Larger)
            {
                char *label1 = make_random_label();
                char *label3 = make_random_label();
                char *label4 = make_random_label();
                char *label5 = make_random_label();
                char *label6 = make_random_label();
                char *label7 = make_random_label();

                printf("POPS GF@op1\n");
                printf("TYPE GF@op3 GF@op1\n");
                printf("JUMPIFEQ %s GF@op3 string@nil\n", label3);
                printf("JUMPIFEQ %s GF@op3 string@string\n", label3);

                printf("POPS GF@op2\n");
                printf("TYPE GF@op4 GF@op2\n");
                printf("JUMPIFEQ %s GF@op4 string@nil\n", label3);
                printf("JUMPIFEQ %s GF@op4 string@string\n", label3);

                printf("JUMPIFEQ %s GF@op3 GF@op4\n", label1); // za touhle podminkou vime ze op3 a op4 nejsou stejneho typu

                printf("JUMPIFEQ %s GF@op3 string@float\n", label5);
                printf("JUMPIFEQ %s GF@op4 string@float\n", label5); // skocime na dany label pokud je jeden z operandu float

                printf("JUMP %s\n", label1);
                printf("LABEL %s\n", label5);

                printf("JUMPIFEQ %s GF@op3 string@float\n", label6);
                printf("INT2FLOAT GF@op1 GF@op1\n");
                printf("LABEL %s\n", label6);

                printf("JUMPIFEQ %s GF@op4 string@float\n", label7);
                printf("INT2FLOAT GF@op2 GF@op2\n");
                printf("LABEL %s\n", label7);

                printf("LABEL %s\n", label1);
                printf("GT GF@op1 GF@op2 GF@op1\n");
                printf("PUSHS GF@op1\n");

                printf("JUMP %s\n", label4);
                printf("LABEL %s\n", label3);
                printf("EXIT int@7\n");
                printf("LABEL %s\n", label4);

                free(label1);
                free(label3);
                free(label4);
                free(label5);
                free(label6);
                free(label7);
                // printf("GTS\n");
            }
            else if (ptree->token->type == T_Larger_eq)
            {
                char *label1 = make_random_label();
                char *label3 = make_random_label();
                char *label4 = make_random_label();
                char *label5 = make_random_label();
                char *label6 = make_random_label();
                char *label7 = make_random_label();

                printf("POPS GF@op1\n");
                printf("TYPE GF@op3 GF@op1\n");
                printf("JUMPIFEQ %s GF@op3 string@nil\n", label3);
                printf("JUMPIFEQ %s GF@op3 string@string\n", label3);

                printf("POPS GF@op2\n");
                printf("TYPE GF@op4 GF@op2\n");
                printf("JUMPIFEQ %s GF@op4 string@nil\n", label3);
                printf("JUMPIFEQ %s GF@op4 string@string\n", label3);

                printf("JUMPIFEQ %s GF@op3 GF@op4\n", label1); // za touhle podminkou vime ze op3 a op4 nejsou stejneho typu

                printf("JUMPIFEQ %s GF@op3 string@float\n", label5);
                printf("JUMPIFEQ %s GF@op4 string@float\n", label5); // skocime na dany label pokud je jeden z operandu float

                printf("JUMP %s\n", label1);
                printf("LABEL %s\n", label5);

                printf("JUMPIFEQ %s GF@op3 string@float\n", label6);
                printf("INT2FLOAT GF@op1 GF@op1\n");
                printf("LABEL %s\n", label6);

                printf("JUMPIFEQ %s GF@op4 string@float\n", label7);
                printf("INT2FLOAT GF@op2 GF@op2\n");
                printf("LABEL %s\n", label7);

                printf("LABEL %s\n", label1);
                printf("LT GF@op1 GF@op2 GF@op1\n");
                printf("NOT GF@op1 GF@op1\n");
                printf("PUSHS GF@op1\n");

                printf("JUMP %s\n", label4);
                printf("LABEL %s\n", label3);
                printf("EXIT int@7\n");
                printf("LABEL %s\n", label4);

                free(label1);
                free(label3);
                free(label4);
                free(label5);
                free(label6);
                free(label7);
                // printf("LTS\n");
                // printf("NOTS\n");
            }
            else if (ptree->token->type == T_Concat)
            {
                char *err_label = make_random_label();
                char *err_skip = make_random_label();

                printf("POPS GF@op1\n");
                printf("TYPE GF@op3 GF@op1\n");
                printf("JUMPIFNEQ %s GF@op3 string@string\n", err_label);

                printf("POPS GF@op2\n");
                printf("TYPE GF@op4 GF@op2\n");
                printf("JUMPIFNEQ %s GF@op4 string@string\n", err_label);

                printf("JUMP %s\n", err_skip);
                printf("LABEL %s\n", err_label);
                printf("EXIT int@7\n");
                printf("LABEL %s\n", err_skip);

                printf("CONCAT GF@op1 GF@op2 GF@op1\n");
                printf("PUSHS GF@op1\n");

                free(err_label);
                free(err_skip);
            }
        }
    }
}
