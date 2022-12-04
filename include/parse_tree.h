#include "../include/lexer.h"
#ifndef PARSE_TREE_H
#define PARSER_TREE_H

typedef struct PTree PTreeNode_t; 

struct PTree {
    token_t *token;
    PTreeNode_t *parent;
    PTreeNode_t *left;
    PTreeNode_t *right;
} PTree;

PTreeNode_t *initPtree();

void disposePtree(PTreeNode_t *ptree);

void insertLeftPtreeNode(PTreeNode_t *ptree, token_t *token);

void insertRightPtreeNode(PTreeNode_t *ptree, token_t *token);

void deletePtreeNode(PTreeNode_t *ptree, token_t *token);

void printPtreeNode(PTreeNode_t *ptree);

void printPtree(PTreeNode_t *ptree);

#endif
