%{
#include <stdio.h>
#include <stdlib.h>

/* Prototipos para evitar avisos de funcao implicita */
int yylex(void);
void yyerror(const char *s);
%}

/* UNION para valores semanticos */
%union {
    int intValue;
}

/* Declaracao de tokens e seus tipos */
%token <intValue> NUM
%token INT FLOAT IDENT ASSIGN SEMICOLON
%token PLUS MINUS TIMES DIVIDE LPAREN RPAREN
%token EQ NEQ LT GT LEQ GEQ AND OR NOT

%type <intValue> expr
%type <intValue> expr_or
%type <intValue> expr_and
%type <intValue> expr_comp
%type <intValue> expr_arit
%type <intValue> expr_unaria
%type <intValue> expr_primaria

/* Regras de precedencia e associatividade */
%left OR
%left AND
%nonassoc EQ NEQ LT GT LEQ GEQ
%left PLUS MINUS
%left TIMES DIVIDE
%right NOT

/* Símbolo inicial */
%start input

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
    | expr SEMICOLON { printf("Resultado: %d\n", $1); }
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
    expr_or { $$ = $1; }
    ;

expr_or:
      expr_or OR expr_and   { $$ = ($1 || $3); }
    | expr_and              { $$ = $1; }
    ;

expr_and:
      expr_and AND expr_comp { $$ = ($1 && $3); }
    | expr_comp              { $$ = $1; }
    ;

expr_comp:
      expr_comp EQ expr_arit   { $$ = ($1 == $3); }
    | expr_comp NEQ expr_arit { $$ = ($1 != $3); }
    | expr_comp LT expr_arit  { $$ = ($1 < $3); }
    | expr_comp LEQ expr_arit { $$ = ($1 <= $3); }
    | expr_comp GT expr_arit  { $$ = ($1 > $3); }
    | expr_comp GEQ expr_arit { $$ = ($1 >= $3); }
    | expr_arit               { $$ = $1; }
    ;

expr_arit:
      expr_arit PLUS expr_unaria  { $$ = $1 + $3; }
    | expr_arit MINUS expr_unaria { $$ = $1 - $3; }
    | expr_arit TIMES expr_unaria { $$ = $1 * $3; }
    | expr_arit DIVIDE expr_unaria {
                    if ($3 == 0) {
                         fprintf(stderr, "[ERRO SEMANTICO] Divisao por zero!\n");
                         $$ = 0;
                    } else {
                         $$ = $1 / $3;
                    }
            }
    | expr_unaria                 { $$ = $1; }
    ;

expr_unaria:
      NOT expr_unaria                    { $$ = !$2; }
    | MINUS expr_unaria                  { $$ = -$2; }
    | expr_primaria                      { $$ = $1; }
    ;

expr_primaria:
      LPAREN expr RPAREN                 { $$ = $2; }
    | NUM                                { $$ = $1; }
    ;

%%

int main(void) {
    printf("Digite expressoes terminadas com ';'. Pressione Ctrl+D para encerrar.\n");
    return yyparse();
}

void yyerror(const char *s) {
    fprintf(stderr, "Erro sintático: %s\n", s);
}