#ifndef TABLE_H
#define TABLE_H

typedef struct symbol {
    char name[32];
    char type[16];
    int line;
    int column;
    struct symbol *next;
} Symbol;

/* API para tabela de símbolos (implementação como tabela hash) */
void initTable();
void freeTable();
void insertSymbol(char *name, char *type, int line, int col);
Symbol *searchSymbol(char *name);
void printTable();
/* Retorna 1 se o símbolo existe com tipo diferente de 'type', 0 caso contrário */
int checkTypeConflict(char *name, char *type);
/* Retorna o tipo do símbolo ("int"/"float") ou NULL se não existir */
const char *getSymbolType(char *name);

#endif
