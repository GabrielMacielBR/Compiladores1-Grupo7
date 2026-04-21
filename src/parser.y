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
%token PLUS MINUS TIMES DIVIDE LPAREN RPAREN LBRACE RBRACE
%token EQ NEQ LT GT LEQ GEQ AND OR NOT INCREMENT DECREMENT
%token WHILE FOR DO
%token IF ELSE

%type <intValue> expr expr_or expr_and expr_comp expr_arit expr_unaria expr_primaria

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
    | input statement
    | input statement '\n'
    | input error SEMICOLON { 
          fprintf(stderr, "[ERRO SINTATICO] Erro recuperado ate ';'\n");
          yyerrok; /* reset de erro */
          yyclearin; /* limpamos o token de lookahead */
      }
    ;

statement:
      declaration SEMICOLON
    | assignment SEMICOLON
    | expr SEMICOLON { printf("Resultado: %d\n", $1); }
    | loop
    | conditional
    ;

statements:
      /* vazio */
    | statements statement
    ;

/* Regra para: int x; */
declaration:
      INT IDENT    { printf("INFO: Declaração de variável detectada.\n"); }
    | FLOAT IDENT  { printf("INFO: Declaração de variável float detectada.\n"); }
    | INT IDENT ASSIGN expr { printf("INFO: Declaração de variável com inicialização detectada. Valor: %d\n", $4); }
    | FLOAT IDENT ASSIGN expr { printf("INFO: Declaração de variável float com inicialização detectada. Valor: %d\n", $4); }
    ;

/* Regra para: x = 10 + 2; */
assignment:
      IDENT ASSIGN expr { printf("SUCESSO: Atribuição realizada. Resultado da expressão: %d\n", $3); }
    | IDENT INCREMENT   { printf("INFO: incremento posfixado detectado\n"); }
    | IDENT DECREMENT   { printf("INFO: decremento posfixado detectado\n"); }
    | INCREMENT IDENT   { printf("INFO: incremento prefixado detectado\n"); }
    | DECREMENT IDENT   { printf("INFO: decremento prefixado detectado\n"); }
    ;

/* Hierarquia de expressões com precedência:
   lógica -> comparação -> aritmética -> unária -> primária */
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
    | IDENT                              { $$ = 0;  } //Valor placeholder
    ;

block: 
    LBRACE statements RBRACE {printf("INFO: Bloco de codigo detectado\n");}
    ;

/* Laços WHILE e FOR e DO...WHILE */

init_for:
      declaration 
    | assignment 
    | /* vazio */
    ;

cond_for:
    expr
    | /* vazio */
    ;

step_for:
      assignment
    | /* vazio */
    ;

body:
    | statement
    | block
    ;

loop:
      WHILE LPAREN expr RPAREN body {printf("INFO: Laço WHILE detectado\n");}
    | FOR LPAREN init_for SEMICOLON cond_for SEMICOLON step_for RPAREN body {printf("INFO: Laço FOR detectado\n");}
    | DO body WHILE LPAREN expr RPAREN SEMICOLON {printf("INFO: Laço DO...WHILE detectado\n");}
    ;

/* Condicionais if-else */
conditional:
      IF LPAREN expr RPAREN body { printf("SUCESSO: Declaração if realizada.\n"); }
    | IF LPAREN expr RPAREN body ELSE body { printf("SUCESSO: Declaração if-else realizada.\n"); }
    ;
    
%%

int main(void) {
    printf("Digite expressoes terminadas com ';'. Pressione Ctrl+D para encerrar.\n");
    return yyparse();
}

void yyerror(const char *s) {
    fprintf(stderr, "Erro sintático: %s\n", s);
}