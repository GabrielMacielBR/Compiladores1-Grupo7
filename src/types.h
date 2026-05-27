#ifndef TYPES_H
#define TYPES_H

#define MAX_CHILDREN 4

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
    AST_DO_WHILE,
    AST_FOR,
    AST_FUNC,
    AST_RETURN,
    AST_CALL
} NodeType;

typedef struct NodeAST {
    NodeType type;
    char op[3];        // ex: "+", "==", "++"
    int value;         // Para AST_NUM
    char name[32];     // Para AST_ID e nome de funções
    
    struct NodeAST *children[MAX_CHILDREN];
    int child_count;
} NodeAST;

#endif
