#include "table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* Tabela hash simples para símbolos */

#define TABLE_SIZE 211

static Symbol *table[TABLE_SIZE];

static unsigned long hash(const char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash;
}

void initTable() {
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

void insertSymbol(char *name, char *type) {
    unsigned long h = hash(name) % TABLE_SIZE;
    /* checa se já existe no bucket */
    for (Symbol *s = table[h]; s; s = s->next) {
        if (strcmp(s->name, name) == 0)
            return;
    }
    Symbol *new = malloc(sizeof(Symbol));
    if (!new) return;
    strncpy(new->name, name, sizeof(new->name)-1);
    new->name[sizeof(new->name)-1] = '\0';
    strncpy(new->type, type, sizeof(new->type)-1);
    new->type[sizeof(new->type)-1] = '\0';
    new->next = table[h];
    table[h] = new;
}

Symbol *searchSymbol(char *name) {
    unsigned long h = hash(name) % TABLE_SIZE;
    for (Symbol *s = table[h]; s; s = s->next)
        if (strcmp(s->name, name) == 0)
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
    printf("\nTabela de Símbolos (hash):\n");
    for (int i = 0; i < TABLE_SIZE; ++i) {
        for (Symbol *s = table[i]; s; s = s->next)
            printf("Bucket %d -> Nome: %s, Tipo: %s\n", i, s->name, s->type);
    }
}
