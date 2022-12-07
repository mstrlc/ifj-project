/**
 * @file parse_tree.h
 *
 * Interface of parse tree used in expression parser and also code generator
 *
 * IFJ project 2022
 *
 * @author <xseidl06> Ondřej Seidl
 *
 */

#ifndef PARSE_TREE_H
#define PARSE_TREE_H

#include "../include/parser.h"
#include "../include/symtable.h"
#include "../include/lexer.h"

typedef struct PTree PTreeNode_t;

struct PTree
{
    token_t *token;
    PTreeNode_t *active;
    PTreeNode_t *left;
    PTreeNode_t *right;
} PTree;

PTreeNode_t *initPtree();

void disposePtree(PTreeNode_t *ptree);

PTreeNode_t *insertLeftPtreeNode(PTreeNode_t *ptree, token_t *token);

PTreeNode_t *insertRightPtreeNode(PTreeNode_t *ptree, token_t *token);

PTreeNode_t *makeOpNode(PTreeNode_t *left, PTreeNode_t *right, PTreeNode_t *op);

void deletePtreeNode(PTreeNode_t *ptree, token_t *token);

void printPtreeNode(PTreeNode_t *ptree);

void printPtree(PTreeNode_t *ptree, symtables_t *symtables);

#endif
