#ifndef TYPES_H
#define TYPES_H

typedef enum {
    AST_NUM,
    AST_ID,
    AST_BINOP,
    AST_UNOP,
    AST_ASSIGN,
    AST_DECL,
    AST_BLOCK,
    AST_SEQ,
    AST_IF,
    AST_WHILE,
    AST_FOR,
    AST_FUNC,
    AST_RETURN,
    AST_CALL
} NodeType;

typedef struct nodeAST {
    NodeType type;
    char op[3];        // ex: "+", "==", "++"
    int value;         // Para AST_NUM
    char name[32];     // Para AST_ID e nome de funções
    
    struct nodeAST *left;
    struct nodeAST *right;
    struct nodeAST *next;
} NodeAST;

#endif
