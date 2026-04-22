#ifndef TYPES_H
#define TYPES_H

typedef struct nodeAST {
    char op;
    int value;
    char name[32];
    struct nodeAST *left;
    struct nodeAST *right;
} NodeAST;

#endif
