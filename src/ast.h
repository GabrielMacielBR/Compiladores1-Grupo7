#ifndef AST_H
#define AST_H

#include "types.h"

NodeAST *createNode(NodeType type);

NodeAST *createNodeNum(int value);
NodeAST *createNodeId(char *name);
NodeAST *createNodeBinOp(char *op, NodeAST *left, NodeAST *right);
NodeAST *createNodeUnOp(char *op, NodeAST *left);
NodeAST *createNodeSeq(NodeAST *first, NodeAST *second); // Encadeia statements

NodeAST *createNodeIf(NodeAST *cond, NodeAST *if_body, NodeAST *else_body);
NodeAST *createNodeFor(NodeAST *init, NodeAST *cond, NodeAST *step, NodeAST *body);
NodeAST *createNodeWhile(NodeAST *cond, NodeAST *body);
NodeAST *createNodeDoWhile(NodeAST *body, NodeAST *cond);

NodeAST *createNodeFunc(char *name, char *ret_type, NodeAST *params, NodeAST *body);
NodeAST *createNodeReturn(NodeAST *value);

NodeAST *createNodeAssign(NodeAST *id, NodeAST *value);
NodeAST *createNodeDecl(char *type, NodeAST *id, NodeAST *value);

void addChild(NodeAST *parent, NodeAST *child);

void printAST(NodeAST *root, int level);

#endif