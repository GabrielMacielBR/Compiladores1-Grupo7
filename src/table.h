#ifndef TABLE_H
#define TABLE_H

typedef struct symbol {
    char name[32];
    char type[16];
    struct symbol *next;
} Symbol;

void insertSymbol(char *name, char *type);
Symbol *searchSymbol(char *name);
void printTable();

#endif
