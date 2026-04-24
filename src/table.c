#include "table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Symbol *table = NULL;

void insertSymbol(char *name, char *type) {
    // Verifica se o símbolo já foi inserido
    Symbol *s = table;
    while (s) {
        if (strcmp(s->name, name) == 0)
            return;
        s = s->next;
    }
    Symbol *new = malloc(sizeof(Symbol));
    strcpy(new->name, name);
    strcpy(new->type, type);
    new->next = NULL;
    if (table == NULL) {
        table = new;
    } else {
        Symbol *last = table;
        while (last->next)
            last = last->next;
        last->next = new;
    }
}

Symbol *searchSymbol(char *name) {
    for (Symbol *s = table; s; s = s->next)
        if (strcmp(s->name, name) == 0)
            return s;
    return NULL;
}

void printTable() {
    printf("\nTabela de Símbolos:\n");
    for (Symbol *s = table; s; s = s->next)
        printf("Nome: %s, Tipo: %s\n", s->name, s->type);
}
