#ifndef AST_H
#define AST_H

#include "types.h"

NodeAST *createNodeNum(int value);
NodeAST *createNodeId(char *name);
NodeAST *createNodeOp(char op, NodeAST *left, NodeAST *right);
void printAST(NodeAST *root);

#endif