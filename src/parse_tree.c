#include "../include/parse_tree.h"

PTree_t *initPtree(token_t *token)
{
    PTree_t *ptree = (PTree_t *)malloc(sizeof(PTree_t));
    ptree->token = token;
    ptree->left = NULL;
    ptree->right = NULL;
    return ptree;
}