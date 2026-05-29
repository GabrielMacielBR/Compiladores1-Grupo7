#ifndef AST_H
#define AST_H

#include <stddef.h>
#include "types.h"

NodeAST *createNode(NodeType type);

NodeAST *createNodeNum(int value);
NodeAST *createNodeId(char *name);
NodeAST *createNodeBinOp(char* op, NodeAST *left, NodeAST *right);
NodeAST *createNodeUnOp(char* op, NodeAST *left);
NodeAST *createNodeSeq(NodeAST *first, NodeAST *second); // Encadeia statements

NodeAST *createNodeIf(NodeAST *cond, NodeAST *if_body, NodeAST *else_body);
NodeAST *createNodeFor(NodeAST *init, NodeAST *cond, NodeAST *step, NodeAST *body);
NodeAST *createNodeWhile(NodeAST *cond, NodeAST *body);
NodeAST *createNodeDoWhile(NodeAST *body, NodeAST *cond);

NodeAST *createNodeAssign(NodeAST *id, NodeAST *value);
NodeAST *createNodeDecl(char *type, NodeAST *id, NodeAST *value);

NodeAST *createNodeFunc(char *type, char *name, NodeAST *params, NodeAST *body);
NodeAST *createNodeCall(char *name, NodeAST *args);
NodeAST *createNodeReturn(NodeAST *value);
int checkFunctionCallArgs(char *name, NodeAST *args, char *message, size_t messageSize);

void addChild(NodeAST *parent, NodeAST *child);

void printAST(NodeAST *root, int level);

#endif