%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "table.h"

/* Prototipos para evitar avisos de funcao implicita */
int yylex(void);
void yyerror(const char *s);

int yydebug = 0;
NodeAST *root = NULL;
%}

%code requires {
#include "ast.h"
}

/* UNION para valores semanticos */
%union {
  int intValue;
  char *id;
  NodeAST *ast;
}

/* Declaracao de tokens e seus tipos */
%token <intValue> NUM
%token INT FLOAT FUNC RETURN
%token <id> IDENT
%token ASSIGN SEMICOLON COMMA
%token PLUS MINUS TIMES DIVIDE LPAREN RPAREN LBRACE RBRACE
%token EQ NEQ LT GT LEQ GEQ AND OR NOT INCREMENT DECREMENT
%token WHILE FOR DO
%token IF ELSE

%type <ast> expr declaration assignment input statement conditional statements block body loop init_for cond_for step_for
%type <ast> function_definition parameter_list parameter parameter_list_opt arg_list arg_list_opt

/* Regras de precedencia e associatividade */
%left OR
%left AND
%nonassoc EQ NEQ LT GT LEQ GEQ
%left PLUS MINUS
%left TIMES DIVIDE
%right NOT
%nonassoc LOWER_THAN_ELSE

%nonassoc THEN
%nonassoc ELSE

/* Símbolo inicial */
%start input

%%

/* Regras da gramatica */
input:
    /* vazio */
      {
        $$ = NULL;
        root = NULL;
      }
  | input statement

  | input function_definition
      {
        if ($1 == NULL) {
          $$ = $2;
          root = $2;
        } else {
          $$ = createNodeSeq($1, $2);
          root = $$;
        }

        /* função também será exibida como statement composto */
        printf("\nAST (função):\n");
        printAST($2, 0);
        printf("\n");

        printTable();
        printf("\n");
      }
    //| input function_definition
  | input error SEMICOLON
      { 
        fprintf(stderr, "[ERRO SINTATICO] Erro recuperado ate ';'\n");
        yyerrok; /* reset de erro */
        yyclearin; /* limpamos o token de lookahead */
      }
  ;

statement:
    declaration SEMICOLON
      {
        $$ = $1;
        root = $1;
      }
  | assignment SEMICOLON
      {
        root = $1;
      }
  | expr SEMICOLON
      {
        $$ = $1;
        root = $1;
      }
  | loop
      {
        $$ = $1;
      }
  | conditional
      {
        $$ = $1;
      }
  //| RETURN expr SEMICOLON { printf("INFO: return com valor %d\n", $2); }
  | RETURN expr SEMICOLON
      {
        $$ = createNodeReturn($2);
      }
  | RETURN SEMICOLON
      {
        $$ = createNodeReturn(NULL);
      }
  ;

statements:
      /* vazio */ { $$ = NULL; }
  | statements statement
      { 
        if ($1 == NULL) {
          $$ = $2;
        } else {
          $$ = createNodeSeq($1, $2);
        }
      }
  ;

/* Regra para: int x; */
declaration:
    INT IDENT 
      {
        insertSymbol($2, "int");

        $$ = createNodeDecl(
          "int",
          createNodeId($2),
          NULL
        );
     }
  | FLOAT IDENT
      {
        insertSymbol($2, "float");
        $$ = createNodeDecl(
          "float",
          createNodeId($2),
          NULL
        );
      }
  | INT IDENT ASSIGN expr
      {
        insertSymbol($2, "int");
        $$ = createNodeDecl(
          "int",
          createNodeId($2),
          $4
        );
      }
  | FLOAT IDENT ASSIGN expr
      {
        insertSymbol($2, "float");
        $$ = createNodeDecl(
          "float",
          createNodeId($2),
          $4
        );
      }
  ;

/* Regra para: x = 10 + 2; */
assignment:
    IDENT ASSIGN expr
      {
        $$ = createNodeAssign(
          createNodeId($1),
          $3
        );
      }
  | IDENT INCREMENT
      {
        $$ = createNodeAssign(
          createNodeId($1),
          createNodeBinOp(
            "+",
            createNodeId($1),
            createNodeNum(1)
          )
        );
      }
  | IDENT DECREMENT
      {
        $$ = createNodeAssign(
            createNodeId($1),
            createNodeBinOp(
              "-",
              createNodeId($1),
              createNodeNum(1)
            )
        );
      }
  ;

/* Hierarquia de expressões com precedência:
   lógica -> comparação -> aritmética -> unária -> primária */
expr:
    expr OR expr
      {
        $$ = createNodeBinOp("||", $1, $3);
      }
  | expr AND expr
      {
        $$ = createNodeBinOp("&&", $1, $3);
      }
  | expr EQ expr
      {
        $$ = createNodeBinOp("==", $1, $3);
      }
  | expr NEQ expr        
      {
        $$ = createNodeBinOp("!=", $1, $3);
      }
  | expr LT expr         
      {
        $$ = createNodeBinOp("<", $1, $3);
      }
  | expr LEQ expr 
      {
        $$ = createNodeBinOp("<=", $1, $3);
      }
  | expr GT expr 
      {
        $$ = createNodeBinOp(">", $1, $3);
      }
  | expr GEQ expr 
      {
        $$ = createNodeBinOp(">=", $1, $3);
      }
  | expr PLUS expr
      {
        $$ = createNodeBinOp("+", $1, $3);
      }
  | expr MINUS expr      {
        $$ = createNodeBinOp("-", $1, $3);
      }
  | expr TIMES expr      {
        $$ = createNodeBinOp("*", $1, $3);
      }
  | expr DIVIDE expr     {
        $$ = createNodeBinOp("/", $1, $3);
      }
  | NOT expr             {
        $$ = createNodeUnOp("!", $2);
      }
  | MINUS expr %prec NOT {
        $$ = createNodeUnOp("-", $2);
      }
  | LPAREN expr RPAREN
      {
        $$ = $2;          
      }
  | NUM
      {
        $$ = createNodeNum($1);
      }
  | IDENT 
      {
        $$ = createNodeId($1);
      }
  | IDENT LPAREN arg_list_opt RPAREN
      {
        $$ = createNodeFuncCall($1, $3);
      }
  ;

block:
    LBRACE statements RBRACE
      {
        $$ = $2;
      }
  ;

/* Laços WHILE e FOR e DO...WHILE */

init_for:
    declaration
      {
        $$ = $1;
      }
  | assignment
      {
        $$ = $1;
      }
  | /* vazio */
      {
        $$ = NULL;
      }
  ;

cond_for:
    expr
      {
        $$ = $1;
      }
  | /* vazio */
      {
        $$ = NULL;
      }
  ;

step_for:
    assignment
      {
        $$ = $1;
      }
  | /* vazio */
      {
        $$ = NULL;
      }
  ;

body:
    statement
  | block
  ;

loop: 
    FOR LPAREN init_for SEMICOLON cond_for SEMICOLON step_for RPAREN body
      {
        $$ = createNodeFor($3, $5, $7, $9);
      }
  | WHILE LPAREN expr RPAREN body
      {
        $$ = createNodeWhile($3, $5);
      }
  | DO body WHILE LPAREN expr RPAREN SEMICOLON
      {
        $$ = createNodeDoWhile($2, $5);
      }
  ;

/* Condicionais if-else (resolvem dangling-else) */
conditional:
    IF LPAREN expr RPAREN body %prec LOWER_THAN_ELSE
      {
        $$ = createNodeIf($3, $5, NULL);
      }
  | IF LPAREN expr RPAREN body ELSE body
      {
        $$ = createNodeIf($3, $5, $7);
      }
  ;
    
/* Funções: definição, parâmetros e lista de argumentos */
function_definition:
    FUNC INT IDENT LPAREN parameter_list_opt RPAREN block
      {
        /* Cria nó de função: nome, tipo de retorno (int por enquanto), lista de parâmetros e corpo */
        $$ = createNodeFunc($3, "int", $5, $7);
        printf("INFO: Função definida: %s\n", $3);
      }
  ;

parameter_list_opt:
    /* vazio */ { $$ = NULL; }
  | parameter_list { $$ = $1; }
  ;

parameter_list:
    parameter { $$ = $1; }
  | parameter_list COMMA parameter
      {
        if ($1 == NULL) {
          $$ = $3;
        } else {
          $$ = createNodeSeq($1, $3);
        }
      }
  ;

parameter:
    INT IDENT
      {
        /* Representa parâmetro como declaração sem valor */
        $$ = createNodeDecl("int", createNodeId($2), NULL);
      }
  ;

arg_list_opt:
    /* vazio */ { $$ = NULL; }
  | arg_list { $$ = $1; }
  ;

arg_list:
    expr { $$ = $1; }
  | arg_list COMMA expr
      {
        if ($1 == NULL) {
          $$ = $3;
        } else {
          $$ = createNodeSeq($1, $3);
        }
      }
  ;

%%

int main(void) {
  printf("Digite expressoes terminadas com ';'. Pressione Ctrl+D para encerrar.\n");
  yyparse();
  return 0;
}

void yyerror(const char *s) {
  fprintf(stderr, "Erro sintático: %s\n", s);
}