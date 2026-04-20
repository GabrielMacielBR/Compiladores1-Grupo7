%{
#include <stdio.h>
#include <stdlib.h>

/* Declarações para evitar avisos de função implícita */
int yylex(void);
void yyerror(const char *s);
%}

/* Define valor semântico (intValue) */
%union {
    int intValue;
}

/* Token que carrega valor semântico */
%token <intValue> NUM

/* Tokens sem valor semântico, mas com precedência */
%token PLUS MINUS TIMES DIVIDE LPAREN RPAREN

/* NOVOS TOKENS (Sincronizados com o scanner.l) */
%token INT FLOAT IDENT ASSIGN SEMICOLON

/* Define o símbolo inicial */
%start input

/* Declara precedência:
   - PLUS e MINUS têm menor precedência
   - TIMES e DIVIDE têm maior precedência */
%left PLUS MINUS
%left TIMES DIVIDE

/* Associa o não terminal expr ao tipo intValue */
%type <intValue> expr

%%

/* Regras da gramatica */
input:
      /* vazio */
    | input comando
    | input error SEMICOLON { 
          fprintf(stderr, "[ERRO SINTATICO] Erro recuperado ate ';'\n");
          yyerrok; /* reset de erro */
          yyclearin; /* limpamos o token de lookahead */
      }
    ;

comando:
      declaracao
    | atribuicao
    | expr SEMICOLON      { printf("Resultado: %d\n", $1); }
    ;

/* Regra para: int x; */
declaracao:
    INT IDENT SEMICOLON { printf("INFO: Declaração de variável detectada.\n"); }
    
    | FLOAT IDENT SEMICOLON { printf("INFO: Declaração de variável float detectada.\n"); }
    ;

/* Regra para: x = 10 + 2; */
atribuicao:
    IDENT ASSIGN expr SEMICOLON { printf("SUCESSO: Atribuição realizada. Resultado da expressão: %d\n", $3); }
    ;

expr:
      expr PLUS expr       { $$ = $1 + $3; }
    | expr MINUS expr      { $$ = $1 - $3; }
    | expr TIMES expr      { $$ = $1 * $3; }
    | expr DIVIDE expr     { 
          if ($3 == 0) {
             fprintf(stderr, "[ERRO SEMANTICO] Divisao por zero!\n");
             $$ = 0; 
          } else {
             $$ = $1 / $3;
          }
        }
    | LPAREN expr RPAREN   { $$ = $2; }
    | NUM                  { $$ = $1; }
    ;

%%

int main(void) {
    printf("Digite expressoes terminadas com ';'. Pressione Ctrl+D para encerrar.\n");
    return yyparse();
}

void yyerror(const char *s) {
    fprintf(stderr, "Erro sintático: %s\n", s);
}