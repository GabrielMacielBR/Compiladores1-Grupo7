#include "table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* Tabela hash simples para símbolos */

#define TABLE_SIZE 211

static Symbol *table[TABLE_SIZE];
static int current_scope = 0;

static unsigned long hash(const char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash;
}

void initTable() {
    current_scope = 0;
    for (int i = 0; i < TABLE_SIZE; ++i)
        table[i] = NULL;
}

void freeTable() {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        Symbol *s = table[i];
        while (s) {
            Symbol *next = s->next;
            free(s);
            s = next;
        }
        table[i] = NULL;
    }
}

void pushScope() {
    current_scope++;
}

void popScope() {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        Symbol *s = table[i];
        Symbol *prev = NULL;
        while (s) {
            if (s->scope == current_scope) {
                Symbol *to_delete = s;
                if (prev) {
                    prev->next = s->next;
                } else {
                    table[i] = s->next;
                }
                s = s->next;
                free(to_delete);
            } else {
                prev = s;
                s = s->next;
            }
        }
    }
    if (current_scope > 0) {
        current_scope--;
    }
}

void insertSymbol(char *name, char *type, int line, int col) {
    unsigned long h = hash(name) % TABLE_SIZE;
    /* checa se já existe no bucket */
    for (Symbol *s = table[h]; s; s = s->next) {
        if (strcmp(s->name, name) == 0 &&
            strcmp(s->kind, "var") == 0 &&
            s->scope == current_scope)
        {
            /* Redeclaração no mesmo escopo -> erro semântico */
            fprintf(stderr, "Erro semântico [L%d:C%d]: símbolo já declarado no mesmo escopo: %s\n", line, col, name);
            return;
        }
    }
    Symbol *new = malloc(sizeof(Symbol));
    if (!new) return;
    strncpy(new->name, name, sizeof(new->name)-1);
    new->name[sizeof(new->name)-1] = '\0';
    strncpy(new->type, type, sizeof(new->type)-1);
    new->type[sizeof(new->type)-1] = '\0';
    strncpy(new->kind, "var", sizeof(new->kind)-1);
    new->kind[sizeof(new->kind)-1] = '\0';
    new->line = line;
    new->column = col;
    new->scope = current_scope;
    new->next = table[h];
    table[h] = new;
}

void insertFunction(char *name, char *returnType, int line, int col) {
    unsigned long h = hash(name) % TABLE_SIZE;

    for (Symbol *s = table[h]; s; s = s->next) {
        if (strcmp(s->name, name) == 0 && strcmp(s->kind, "func") == 0) {
            /* Redeclaração de função -> erro semântico */
            fprintf(stderr, "Erro semântico [L%d:C%d]: função já declarada: %s\n", line, col, name);
            return;
        }
    }

    Symbol *new = malloc(sizeof(Symbol));
    if (!new) return;

    strncpy(new->name, name, sizeof(new->name)-1);
    new->name[sizeof(new->name)-1] = '\0';

    strncpy(new->type, returnType, sizeof(new->type)-1);
    new->type[sizeof(new->type)-1] = '\0';

    strncpy(new->kind, "func", sizeof(new->kind)-1);
    new->kind[sizeof(new->kind)-1] = '\0';

    new->line = line;
    new->column = col;
    new->scope = 0;
    new->ast = NULL;
    new->next = table[h];
    table[h] = new;
}

void setFunctionAst(char *name, NodeAST *ast) {
    Symbol *s = searchFunction(name);
    if (s) {
        s->ast = ast;
    }
}

NodeAST *getFunctionAst(char *name) {
    Symbol *s = searchFunction(name);
    if (!s) return NULL;
    return s->ast;
}

Symbol *searchSymbol(char *name) {
    unsigned long h = hash(name) % TABLE_SIZE;
    for (Symbol *s = table[h]; s; s = s->next)
        if (strcmp(s->name, name) == 0 && strcmp(s->kind, "var") == 0)
            return s;
    return NULL;
}

Symbol *searchSymbolInCurrentScope(char *name) {
    unsigned long h = hash(name) % TABLE_SIZE;
    for (Symbol *s = table[h]; s; s = s->next)
        if (strcmp(s->name, name) == 0 && strcmp(s->kind, "var") == 0 && s->scope == current_scope)
            return s;
    return NULL;
}

Symbol *searchFunction(char *name) {
    unsigned long h = hash(name) % TABLE_SIZE;
    for (Symbol *s = table[h]; s; s = s->next)
        if (strcmp(s->name, name) == 0 && strcmp(s->kind, "func") == 0)
            return s;
    return NULL;
}

int checkTypeConflict(char *name, char *type) {
    Symbol *s = searchSymbol(name);
    if (!s) return 0;
    return strcmp(s->type, type) != 0;
}

const char *getSymbolType(char *name) {
    Symbol *s = searchSymbol(name);
    if (!s) return NULL;
    return s->type;
}

void printTable() {
    printf("\n=== Tabela de Símbolos ===\n");
    for (int i = 0; i < TABLE_SIZE; ++i) {
        for (Symbol *s = table[i]; s; s = s->next)
            printf("Nome: %s, Tipo: %s, Kind: %s, Scope: %d, Decl: L%d:C%d\n", s->name, s->type, s->kind, s->scope, s->line, s->column);
    }
}
