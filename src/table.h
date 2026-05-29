#ifndef TABLE_H
#define TABLE_H

typedef struct symbol {
    char name[32];
    char type[16];
    char kind[16];
    int param_count;
    int line;
    int column;
    int scope;
    struct symbol *next;
} Symbol;

/* API para tabela de símbolos (implementação como tabela hash) */
void initTable();
void freeTable();

void pushScope();
void popScope();

void insertSymbol(char *name, char *type, int line, int col);
void insertFunction(char *name, char *returnType, int param_count, int line, int col);

Symbol *searchSymbol(char *name);
Symbol *searchSymbolInCurrentScope(char *name);
Symbol *searchFunction(char *name);

void printTable();

/* Retorna 1 se o símbolo existe com tipo diferente de 'type', 0 caso contrário */
int checkTypeConflict(char *name, char *type);

/* Retorna o tipo do símbolo ("int"/"float") ou NULL se não existir */
const char *getSymbolType(char *name);

#endif
