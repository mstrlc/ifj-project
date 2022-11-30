#include "../include/parse_tree.h"
#include <stdio.h>

PTreeNode_t *initPtree()
{
    PTreeNode_t *ptree = (PTreeNode_t *)malloc(sizeof(PTreeNode_t));
    ptree->token = NULL;
    ptree->left = NULL;
    ptree->right = NULL;
    ptree->parent = NULL;
    return ptree;
}

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

void insertLeftPtreeNode(PTreeNode_t *ptree, token_t *token)
{
    PTreeNode_t *newNode = initPtree(token);
    if(ptree->left == NULL) {
        newNode->parent = ptree;
        ptree->left = newNode;
    }
    else {
        insertLeftPtreeNode(ptree->left, token);
    }   
}

void insertRightPtreeNode(PTreeNode_t *ptree, token_t *token)
{
    PTreeNode_t *newNode = initPtree(token);
    if(ptree->right == NULL) {
        newNode->parent = ptree;
        ptree->right = newNode;
    }
    else {
        insertRightPtreeNode(ptree->right, token);
    }
}

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

void printPtreeNode(PTreeNode_t *ptree)
{
    printf("%s", ptree->token->data);
}

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