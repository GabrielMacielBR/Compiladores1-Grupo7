#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "table.h"


NodeAST *createNode(NodeType type) {
    NodeAST *newNode = malloc(sizeof(NodeAST));
    
    newNode->type = type;
    newNode->child_count = 0;

    for (int i = 0; i < MAX_CHILDREN; i++) {
        newNode->children[i] = NULL;
    }
    return newNode;
}

NodeAST *createNodeNum(int value) {
    NodeAST *newNode = createNode(AST_NUM);
    newNode->value = value;
    return newNode;
}

NodeAST *createNodeId(char *name) {
    NodeAST *newNode = createNode(AST_ID);
    strcpy(newNode->name, name);
    return newNode;
}

NodeAST *createNodeBinOp(char* op, NodeAST *left, NodeAST *right) {
    NodeAST *newNode = createNode(AST_BINOP);
    strcpy(newNode->op, op);

    addChild(newNode, left);
    addChild(newNode, right);

    return newNode;
}

NodeAST *createNodeUnOp(char* op, NodeAST *left) {
    NodeAST *newNode = createNode(AST_UNOP);
    strcpy(newNode->op, op);

    addChild(newNode, left);

    return newNode;
}

NodeAST *createNodeSeq(NodeAST *first, NodeAST *second) {
   if (!first)
        return second;

    NodeAST *newNode = createNode(AST_SEQ);

    addChild(newNode, first);
    addChild(newNode, second);

    return newNode;
}

NodeAST *createNodeIf(NodeAST *cond, NodeAST *if_body, NodeAST *else_body) {
    NodeAST *newNode = createNode(AST_IF);

    addChild(newNode, cond);
    addChild(newNode, if_body);

    if (else_body)
        addChild(newNode, else_body);

    return newNode;
}

NodeAST *createNodeFor(NodeAST *init, NodeAST *cond, NodeAST *step, NodeAST *body) {
    NodeAST *newNode = createNode(AST_FOR);

    addChild(newNode, init);
    addChild(newNode, cond);
    addChild(newNode, step);
    addChild(newNode, body);

    return newNode;
}

NodeAST *createNodeWhile(NodeAST *cond, NodeAST *body) {
    NodeAST *newNode = createNode(AST_WHILE);

    addChild(newNode, cond);
    addChild(newNode, body);

    return newNode;
}

NodeAST *createNodeDoWhile(NodeAST *body, NodeAST *cond) {
    NodeAST *newNode = createNode(AST_DO_WHILE);

    addChild(newNode, body);
    addChild(newNode, cond);

    return newNode;
}

NodeAST *createNodeAssign(NodeAST *id, NodeAST *value) {
    NodeAST *newNode = createNode(AST_ASSIGN);

    addChild(newNode, id);
    addChild(newNode, value);

    return newNode;
}

NodeAST *createNodeDecl(char *type, NodeAST *id, NodeAST *value) {
    NodeAST *newNode = createNode(AST_DECL);
    strcpy(newNode->op, type);

    addChild(newNode, id);

    if (value)
        addChild(newNode, value);

    return newNode;
}

void addChild(NodeAST *parent, NodeAST *child) {
    if (!parent || !child)
        return;

    if (parent->child_count < MAX_CHILDREN) {
        parent->children[parent->child_count++] = child;
    }
}

void printAST(NodeAST *root, int level) {
    if (!root)
        return;

    switch (root->type) {
        case AST_NUM:
            printf("%d", root->value);
            break;
        case AST_ID:
            printf("%s", root->name);
            break;
        case AST_BINOP:
            printf("(");
            printAST(root->children[0], level);
            printf(" %s ", root->op);
            printAST(root->children[1], level);
            printf(")");
            break;
        case AST_UNOP:
            printf("(%s", root->op);
            printAST(root->children[0], level);
            printf(")");
            break;
        case AST_SEQ:
            printAST(root->children[0], level);

            if (root->children[0] &&
                root->children[0]->type != AST_IF &&
                root->children[0]->type != AST_WHILE &&
                root->children[0]->type != AST_DO_WHILE &&
                root->children[0]->type != AST_FOR) {
                printf(";\n");
            } else {
                printf("\n");
            }

            printAST(root->children[1], level);
            break;
        case AST_IF:
            printf("if (");
            printAST(root->children[0], level);
            printf(") {\n");
            printAST(root->children[1], level);
            printf("\n}");

            if (root->child_count == 3) {
                printf(" else {\n");
                printAST(root->children[2], level);
                printf("\n}");
            }
            break;
        case AST_WHILE:
            printf("while (");
            printAST(root->children[0], level);
            printf(") {\n");
            printAST(root->children[1], level);
            printf("\n}");
            break;
        case AST_DO_WHILE:
            printf("do {\n ");
            printAST(root->children[0], level);
            printf("\n} while (");
            printAST(root->children[1], level);
            printf(")");
            break;
        case AST_FOR:
            printf("for (");

            if (root->children[0])
                printAST(root->children[0], level);

            printf("; ");

            if (root->children[1])
                printAST(root->children[1], level);

            printf("; ");

            if (root->children[2])
                printAST(root->children[2], level);

            printf(") {\n");

            if (root->children[3])
                printAST(root->children[3], level);

            printf("\n}");
            break;
        case AST_ASSIGN:
            printf("(ASSIGN ");
            printAST(root->children[0], level);
            printf(" = ");
            printAST(root->children[1], level);
            printf(")");
            break;
        case AST_DECL:
            printf("(DECL %s ", root->op);
            printAST(root->children[0], level);

            if (root->child_count == 2) {
                printf(" = ");
                printAST(root->children[1], level);
            }

            printf(")");
            break;
        default:
            printf("Unknown");
            break;
    }
}

const char *inferType(NodeAST *node) {
    if (!node) return NULL;
    switch (node->type) {
        case AST_NUM:
            return "int";
        case AST_ID: {
            const char *t = getSymbolType(node->name);
            return t; /* may be NULL */
        }
        case AST_BINOP: {
            const char *lt = inferType(node->children[0]);
            const char *rt = inferType(node->children[1]);
            if (!lt || !rt) return NULL;
            if (strcmp(lt, "float") == 0 || strcmp(rt, "float") == 0) return "float";
            return "int";
        }
        case AST_UNOP: {
            return inferType(node->children[0]);
        }
        case AST_ASSIGN:
            return inferType(node->children[1]);
        case AST_DECL:
            return node->op[0] ? node->op : NULL;
        default:
            return NULL;
    }
}