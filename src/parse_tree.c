#include "../include/parse_tree.h"

PTreeNode_t *initPtree(token_t *token)
{
    PTreeNode_t *ptree = (PTreeNode_t *)malloc(sizeof(PTreeNode_t));
    ptree->token = token;
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

void insertPtreeNode(PTreeNode_t *ptree, token_t *token)
{
    if (ptree->left == NULL) {
        ptree->left = initPtree(token);
        ptree->left->parent = ptree;
    } else if (ptree->right == NULL) {
        ptree->right = initPtree(token);
        ptree->right->parent = ptree;
    } else {
        insertPtreeNode(ptree->left, token);
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
