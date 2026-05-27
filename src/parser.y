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
%token COLON QUESTION
%token PLUS MINUS TIMES DIVIDE LPAREN RPAREN LBRACE RBRACE
%token EQ NEQ LT GT LEQ GEQ AND OR NOT INCREMENT DECREMENT
%token WHILE FOR DO
%token IF ELSE

%type <ast> expr declaration assignment

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
    | input statement
    //| input function_definition
    | input error SEMICOLON { 
      fprintf(stderr, "[ERRO SINTATICO] Erro recuperado ate ';'\n");
      yyerrok; /* reset de erro */
      yyclearin; /* limpamos o token de lookahead */
    }
    ;

statement:
    declaration SEMICOLON
    {
        root = $1;

        printf("\nAST:\n");
        printAST(root, 0);
        printf("\n");

        printTable();
    }
  | assignment SEMICOLON
    {
        root = $1;

        printf("\nAST:\n");
        printAST(root, 0);
        printf("\n");

        printTable();
    }
  //| loop
  //| conditional
  //| RETURN expr SEMICOLON { printf("INFO: return com valor %d\n", $2); }
  //| RETURN SEMICOLON { printf("INFO: return sem valor\n"); }
  ;

statements:
      /* vazio */
    | statements statement
    ;

/* Regra para: int x; */
declaration:
    INT IDENT
    {
      if (searchSymbol($2)) {
        if (checkTypeConflict($2, "int")) {
          char _msg[128];
          snprintf(_msg, sizeof(_msg), "Erro semântico: redeclaração com tipo diferente: %s", $2);
          yyerror(_msg);
          YYABORT;
        } else {
          fprintf(stderr, "Aviso semântico: símbolo já declarado: %s\n", $2);
        }
      } else {
        insertSymbol($2, "int");
      }

      $$ = createNodeDecl(
        "int",
        createNodeId($2),
        NULL
      );
    }
  | FLOAT IDENT
    {
      if (searchSymbol($2)) {
        if (checkTypeConflict($2, "float")) {
          char _msg[128];
          snprintf(_msg, sizeof(_msg), "Erro semântico: redeclaração com tipo diferente: %s", $2);
          yyerror(_msg);
          YYABORT;
        } else {
          fprintf(stderr, "Aviso semântico: símbolo já declarado: %s\n", $2);
        }
      } else {
        insertSymbol($2, "float");
      }

      $$ = createNodeDecl(
        "float",
        createNodeId($2),
        NULL
      );
    }
  | INT IDENT ASSIGN expr
    {
      if (searchSymbol($2)) {
        if (checkTypeConflict($2, "int")) {
          char _msg[128];
          snprintf(_msg, sizeof(_msg), "Erro semântico: redeclaração com tipo diferente: %s", $2);
          yyerror(_msg);
          YYABORT;
        } else {
          fprintf(stderr, "Aviso semântico: símbolo já declarado: %s\n", $2);
        }
      } else {
        insertSymbol($2, "int");
      }
      /* checar compatibilidade de tipos entre declaração e expressão atribuída */
      {
        const char *rhsType = inferType($4);
        if (rhsType && strcmp(rhsType, "int") != 0) {
          char _msg[128];
          snprintf(_msg, sizeof(_msg), "Erro semântico: atribuição com tipo incompatível na declaração: %s", $2);
          yyerror(_msg);
          YYABORT;
        }
      }
      $$ = createNodeDecl(
        "int",
        createNodeId($2),
        $4
      );
    }
  | FLOAT IDENT ASSIGN expr
    {
      if (searchSymbol($2)) {
        if (checkTypeConflict($2, "float")) {
          char _msg[128];
          snprintf(_msg, sizeof(_msg), "Erro semântico: redeclaração com tipo diferente: %s", $2);
          yyerror(_msg);
          YYABORT;
        } else {
          fprintf(stderr, "Aviso semântico: símbolo já declarado: %s\n", $2);
        }
      } else {
        insertSymbol($2, "float");
      }
      /* checar compatibilidade de tipos entre declaração e expressão atribuída */
      {
        const char *rhsType = inferType($4);
        if (rhsType && strcmp(rhsType, "float") != 0) {
          char _msg[128];
          snprintf(_msg, sizeof(_msg), "Erro semântico: atribuição com tipo incompatível na declaração: %s", $2);
          yyerror(_msg);
          YYABORT;
        }
      }
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
      if (!searchSymbol($1)) {
        char _msg[128];
        snprintf(_msg, sizeof(_msg), "Erro semântico: atribuição a símbolo não declarado: %s", $1);
        yyerror(_msg);
        YYABORT;
      } else {
        /* checar compatibilidade de tipos entre LHS e RHS */
        {
          const char *lhsType = getSymbolType($1);
          const char *rhsType = inferType($3);
          if (rhsType && lhsType && strcmp(lhsType, rhsType) != 0) {
            char _msg[128];
            snprintf(_msg, sizeof(_msg), "Erro semântico: incompatibilidade de tipos na atribuição: %s", $1);
            yyerror(_msg);
            YYABORT;
          }
        }
        $$ = createNodeAssign(
          createNodeId($1),
          $3
        );
      }
    }
  | IDENT INCREMENT
    {
      if (!searchSymbol($1)) {
        char _msg[128];
        snprintf(_msg, sizeof(_msg), "Erro semântico: incremento em símbolo não declarado: %s", $1);
        yyerror(_msg);
        YYABORT;
      } else {
        const char *t = getSymbolType($1);
        if (!t || strcmp(t, "int") != 0) {
          char _msg[128];
          snprintf(_msg, sizeof(_msg), "Erro semântico: incremento apenas permitido para int: %s", $1);
          yyerror(_msg);
          YYABORT;
        }
        $$ = createNodeAssign(
          createNodeId($1),
          createNodeBinOp(
            "+",
            createNodeId($1),
            createNodeNum(1)
          )
        );
      }
    }
  | IDENT DECREMENT
    {
      if (!searchSymbol($1)) {
        char _msg[128];
        snprintf(_msg, sizeof(_msg), "Erro semântico: decremento em símbolo não declarado: %s", $1);
        yyerror(_msg);
        YYABORT;
      } else {
        const char *t = getSymbolType($1);
        if (!t || strcmp(t, "int") != 0) {
          char _msg[128];
          snprintf(_msg, sizeof(_msg), "Erro semântico: decremento apenas permitido para int: %s", $1);
          yyerror(_msg);
          YYABORT;
        }
        $$ = createNodeAssign(
          createNodeId($1),
          createNodeBinOp(
            "-",
            createNodeId($1),
            createNodeNum(1)
          )
        );
      }
    }
;

/* Hierarquia de expressões com precedência:
   lógica -> comparação -> aritmética -> unária -> primária */
expr:
    expr OR expr         { $$ = createNodeBinOp("||", $1, $3); }
  | expr AND expr        { $$ = createNodeBinOp("&&", $1, $3); }
  | expr EQ expr         { $$ = createNodeBinOp("==", $1, $3); }
  | expr NEQ expr        { $$ = createNodeBinOp("!=", $1, $3); }
  | expr LT expr         { $$ = createNodeBinOp("<", $1, $3);  }
  | expr LEQ expr        { $$ = createNodeBinOp("<=", $1, $3); }
  | expr GT expr         { $$ = createNodeBinOp(">", $1, $3);  }
  | expr GEQ expr        { $$ = createNodeBinOp(">=", $1, $3); }
  | expr PLUS expr       { $$ = createNodeBinOp("+", $1, $3);  }
  | expr MINUS expr      { $$ = createNodeBinOp("-", $1, $3);  }
  | expr TIMES expr      { $$ = createNodeBinOp("*", $1, $3);  }
  | expr DIVIDE expr     { $$ = createNodeBinOp("/", $1, $3);  }
  | NOT expr             { $$ = createNodeUnOp("!", $2);       }
  | MINUS expr %prec NOT { $$ = createNodeUnOp("-", $2);       }
  | LPAREN expr RPAREN   { $$ = $2;                            }
  | NUM                  { $$ = createNodeNum($1);             }
  | IDENT                { 
                       if (!searchSymbol($1))
                           fprintf(stderr, "Aviso semântico: símbolo não declarado: %s\n", $1);
                       $$ = createNodeId($1);
                     }
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
      statement
    | block
    ;

loop:
      WHILE LPAREN expr RPAREN body {printf("INFO: Laço WHILE detectado\n");}
    | FOR LPAREN init_for SEMICOLON cond_for SEMICOLON step_for RPAREN body {printf("INFO: Laço FOR detectado\n");}
    | DO body WHILE LPAREN expr RPAREN SEMICOLON {printf("INFO: Laço DO...WHILE detectado\n");}
    ;

/* Condicionais if-else (resolvem dangling-else) */
conditional:
      IF LPAREN expr RPAREN statement %prec LOWER_THAN_ELSE { printf("SUCESSO: Declaração if realizada.\n"); }
    | IF LPAREN expr RPAREN statement ELSE statement { printf("SUCESSO: Declaração if-else realizada.\n"); }
    | expr QUESTION body COLON body { printf("SUCESSO: Declaração condicional com operador ternário realizada.\n"); }
    ;
    
/* Funções: definição, parâmetros e lista de argumentos */
function_definition:
  FUNC INT IDENT LPAREN parameter_list_opt RPAREN block { printf("INFO: Função definida: %s\n", $3); }
    ;

parameter_list_opt:
      /* vazio */
    | parameter_list
    ;

parameter_list:
      parameter
    | parameter_list COMMA parameter
    ;

parameter:
      INT IDENT { printf("INFO: Parametro: %s\n", $2); }
    ;

arg_list_opt:
      /* vazio */
    | arg_list
    ;

arg_list:
      expr
    | arg_list COMMA expr
    ;

%%

int main(void) {
    printf("Digite expressoes terminadas com ';'. Pressione Ctrl+D para encerrar.\n");
      initTable();
      int ret = yyparse();
      freeTable();
      return ret;
}

void yyerror(const char *s) {
    fprintf(stderr, "Erro sintático: %s\n", s);
}