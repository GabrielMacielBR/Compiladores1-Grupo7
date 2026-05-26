#ifndef AST_H
#define AST_H

#include "types.h"

NodeAST *createNodeNum(int value);
NodeAST *createNodeId(char *name);
NodeAST *createNodeBinOp(char* op, NodeAST *left, NodeAST *right);
NodeAST *createNodeUnOp(char* op, NodeAST *left);
NodeAST *createNodeSeq(NodeAST *first, NodeAST *second); // Encadeia statements
NodeAST *createNodeIf(NodeAST *cond, NodeAST *if_body, NodeAST *else_body);
NodeAST *createNodeAssign(NodeAST *id, NodeAST *value);
NodeAST *createNodeDecl(char *type, NodeAST *id, NodeAST *value);
void printAST(NodeAST *root, int level);

#endif