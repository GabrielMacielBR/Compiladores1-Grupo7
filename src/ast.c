#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

NodeAST *createNodeNum(int value) {
    NodeAST *newNode = malloc(sizeof(NodeAST));
    newNode->type = AST_NUM;
    newNode->value = value;
    newNode->left = newNode->right = newNode->next = NULL;
    return newNode;
}

NodeAST *createNodeId(char *name) {
    NodeAST *newNode = malloc(sizeof(NodeAST));
    newNode->type = AST_ID;
    strcpy(newNode->name, name);
    newNode->left = newNode->right = newNode->next = NULL;
    return newNode;
}

NodeAST *createNodeBinOp(char* op, NodeAST *left, NodeAST *right) {
    NodeAST *newNode = malloc(sizeof(NodeAST));
    newNode->type = AST_BINOP;
    strcpy(newNode->op, op);
    newNode->left = left;
    newNode->right = right;
    newNode->next = NULL;
    return newNode;
}

NodeAST *createNodeUnOp(char* op, NodeAST *left) {
    NodeAST *newNode = malloc(sizeof(NodeAST));
    newNode->type = AST_UNOP;
    strcpy(newNode->op, op);
    newNode->left = left;
    newNode->right = newNode->next = NULL;
    return newNode;
}

NodeAST *createNodeSeq(NodeAST *first, NodeAST *second) {
    if (!first) return second;
    NodeAST *newNode = malloc(sizeof(NodeAST));
    newNode->type = AST_SEQ;
    newNode->left = first;
    newNode->right = second;
    newNode->next = NULL;
    return newNode;
}

NodeAST *createNodeIf(NodeAST *cond, NodeAST *if_body, NodeAST *else_body) {
    NodeAST *newNode = malloc(sizeof(NodeAST));
    newNode->type = AST_IF;
    newNode->left = cond;
    newNode->right = createNodeSeq(if_body, else_body); // Um truque para guardar os 2
    newNode->next = NULL;
    return newNode;
}

void printAST(NodeAST *root, int level) {
    if (root) {
        switch (root->type) {
            case AST_NUM:
                printf("%d", root->value);
                break;
            case AST_ID:
                printf("%s", root->name);
                break;
            case AST_BINOP:
                printf("(");
                printAST(root->left, level);
                printf(" %s ", root->op);
                printAST(root->right, level);
                printf(")");
                break;
            case AST_UNOP:
                printf("(");
                printf("%s", root->op);
                printAST(root->left, level);
                printf(")");
                break;
            case AST_SEQ:
                printAST(root->left, level);
                if (root->left) printf(";\n");
                printAST(root->right, level);
                break;
            case AST_IF:
                printf("if (");
                printAST(root->left, level); // Cond
                printf(") { ");
                printAST(root->right, level); // Body e Else
                printf(" }");
                break;
            default:
                printf("Unknown");
                break;
        }
    }
}
