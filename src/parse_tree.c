#include "../include/parse_tree.h"
#include <stdio.h>

/**
 * @brief Initializes a new parse tree node
 * 
 * @return PTreeNode_t* pointer to the new parse tree node
 */
PTreeNode_t *initPtree()
{
    PTreeNode_t *ptree = (PTreeNode_t *)malloc(sizeof(PTreeNode_t));
    ptree->token = NULL;
    ptree->left = NULL;
    ptree->right = NULL;
    ptree->parent = NULL;
    return ptree;
}


/**
 * @brief Disposes parse tree node
 * 
 * @param ptree pointer to the parse tree node to be disposed of
 */
void disposePtree(PTreeNode_t *ptree)
{
    if (ptree->left != NULL) {
        disposePtree(ptree->left);
    }
    if (ptree->right != NULL) {
        disposePtree(ptree->right);
    }
    free(ptree);
}

/**
 * @brief Inserts a new node to the left of the current node
 * 
 * @param ptree pointer to the current node
 * @param token pointer to the token to be inserted
 */
void insertLeftPtreeNode(PTreeNode_t *ptree, token_t *token)
{
    PTreeNode_t *newNode = initPtree();
    if(ptree->left == NULL) {
        newNode->parent = ptree;
        ptree->left = newNode;
    }
    else {
        insertLeftPtreeNode(ptree->left, token);
    }   
}

/**
 * @brief Inserts a new node to the right of the current node
 * 
 * @param ptree pointer to the current node
 * @param token pointer to the token to be inserted
 */
void insertRightPtreeNode(PTreeNode_t *ptree, token_t *token)
{
    PTreeNode_t *newNode = initPtree();
    if(ptree->right == NULL) {
        newNode->parent = ptree;
        ptree->right = newNode;
    }
    else {
        insertRightPtreeNode(ptree->right, token);
    }
}

/**
 * @brief Deletes a node from the parse tree
 * 
 * @param ptree pointer to the node to be deleted
 * @param token pointer to the token to be deleted
 */
void deletePtreeNode(PTreeNode_t *ptree, token_t *token)
{
    if (ptree->left != NULL) {
        if (ptree->left->token->type == token->type) {
            disposePtree(ptree->left);
            ptree->left = NULL;
        } else {
            deletePtreeNode(ptree->left, token);
        }
    }
    if (ptree->right != NULL) {
        if (ptree->right->token->type == token->type) {
            disposePtree(ptree->right);
            ptree->right = NULL;
        } else {
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
    // if(ptree->token->data != NULL)
    //     printf("%s", ptree->token->data);
    if(ptree->token != NULL)
        printToken(ptree->token);
}

/**
 * @brief Prints the parse tree
 * 
 * @param ptree pointer to the root of the parse tree
 */
void printPtree(PTreeNode_t *ptree)
{
    if (ptree->left != NULL) {
        printPtree(ptree->left);
    }
    printPtreeNode(ptree);
    if (ptree->right != NULL) {
        printPtree(ptree->right);
    }
}
