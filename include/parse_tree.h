/**
 * @file parse_tree.h
 * @author Ondřej Seidl xseidl06
 * @brief Interface of parse_tree * IFJ project 2022
 */
#include "../include/lexer.h"
#include "../include/parser.h"
#include "../include/symtable.h"
#ifndef PARSE_TREE_H
#define PARSER_TREE_H

typedef struct PTree PTreeNode_t; 

struct PTree {
    token_t *token;
    PTreeNode_t *active;
    PTreeNode_t *left;
    PTreeNode_t *right;
} PTree;


PTreeNode_t *initPtree();

void disposePtree(PTreeNode_t *ptree);

PTreeNode_t * insertLeftPtreeNode(PTreeNode_t *ptree, token_t *token);

PTreeNode_t * insertRightPtreeNode(PTreeNode_t *ptree, token_t *token);

PTreeNode_t * makeOpNode(PTreeNode_t *left, PTreeNode_t *right, PTreeNode_t *op);

void deletePtreeNode(PTreeNode_t *ptree, token_t *token);

void printPtreeNode(PTreeNode_t *ptree);

void printPtree(PTreeNode_t *ptree, Symtables *symtables);

#endif
