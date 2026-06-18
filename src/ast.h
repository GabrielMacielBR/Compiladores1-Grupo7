#ifndef AST_H
#define AST_H

#include <stddef.h>
#include "types.h"

NodeAST *createNode(NodeType type);

NodeAST *createNodeNum(int value);
NodeAST *createNodeFloat(float value);
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

int isNumeric(char *type);
int isBooleanCompatible(char *type);
int isConditionValid(NodeAST *expr);
int isAssignable(const char *lhs, const char *rhs);

NodeAST *createNodeCall(char *name, NodeAST *args);

int checkFunctionCallArgs(char *name, NodeAST *args, char *message, size_t messageSize);

void addChild(NodeAST *parent, NodeAST *child);
void printAST(NodeAST *root, int level);
void generateTAC(NodeAST *root);
void generatePythonFile(NodeAST *root, char *filename);

typedef struct TAC
{
	char op[32];
	char arg1[64];
	char arg2[64];
	char result[64];
	struct TAC *next;
} TAC;

TAC *createTAC(const char *op, const char *arg1, const char *arg2, const char *result);
TAC *insertTAC(TAC *list, TAC *instr);
void printTAC(TAC *list);
void freeTAC(TAC *list);

char *newTemp();
char *newLabel();

TAC *optimizeTAC(TAC *list);

#endif