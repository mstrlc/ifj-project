#include "../include/lexer.h"
#ifndef PARSE_TREE_H
#define PARSER_TREE_H

typedef struct PTree PTree_t; 

struct PTree {
    token_t *token;
    
    PTree_t *parent;
    PTree_t *left;
    PTree_t *right;
} PTree;

PTree_t *initPtree(token_t *token);

void disposePtree(PTree_t *ptree);

void insertPtree(PTree_t *ptree, token_t *token);

void deletePtree(PTree_t *ptree, token_t *token);

void printNode(PTree_t *ptree);

void printPtree(PTree_t *ptree);

#endif