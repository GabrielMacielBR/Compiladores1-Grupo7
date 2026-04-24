#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

NodeAST *createNodeNum(int value) {
    NodeAST *new = malloc(sizeof(NodeAST));
    new->op = ' ';
    new->value = value;
    new->left = new->right = NULL;
    return new;
}

NodeAST *createNodeId(char *name) {
    NodeAST *new = malloc(sizeof(NodeAST));
    new->op = 'i';
    strcpy(new->name, name);
    new->left = new->right = NULL;
    return new;
}

NodeAST *createNodeOp(char op, NodeAST *left, NodeAST *right) {
    NodeAST *new = malloc(sizeof(NodeAST));
    new->op = op;
    new->left = left;
    new->right = right;
    return new;
}

void printAST(NodeAST *root) {
    if (root) {
        if (root->op == ' ')
            printf("%d", root->value);
        else if (root->op == 'i')
            printf("%s", root->name);
        else {
            printf("(");
            printAST(root->left);
            printf(" %c ", root->op);
            printAST(root->right);
            printf(")");
        }
    }
}
