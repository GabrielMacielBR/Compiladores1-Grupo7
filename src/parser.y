%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "table.h"

extern int yyline;
extern int yycolumn;

/* Prototipos para evitar avisos de funcao implicita */
int yylex(void);
void yyerror(const char *s);

int yydebug = 0;
NodeAST *root = NULL;
static const char *current_function_return_type = NULL;
static int current_function_has_return = 0;

int loopCounter = 0;
%}

%code requires {
#include "ast.h"
}

/* UNION para valores semanticos */
%union {
  int intValue;
  float floatValue;
  char *id;
  NodeAST *ast;
}

/* Declaracao de tokens e seus tipos */
%token <intValue> NUM
%token <floatValue> FLOAT_NUM
%token INT FLOAT FUNC RETURN
%token <id> IDENT
%token ASSIGN SEMICOLON COMMA
%token COLON QUESTION
%token PLUS MINUS TIMES DIVIDE LPAREN RPAREN LBRACE RBRACE
%token EQ NEQ LT GT LEQ GEQ AND OR NOT INCREMENT DECREMENT
%token WHILE FOR DO
%token IF ELSE
%token BREAK CONTINUE

%type <ast> input
%type <ast> expr declaration assignment statement statements
%type <ast> loop conditional body block init_for cond_for step_for

%type <ast> function_definition
%type <ast> function_block
%type <ast> function_call
%type <ast> return_statement

%type <ast> arg_list_opt
%type <ast> arg_list

%type <ast> parameter_list_opt
%type <ast> parameter_list
%type <ast> parameter

%type <id> type_specifier
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
      {
          if ($1 == NULL)
              $$ = $2;
          else
              $$ = createNodeSeq($1, $2);

          root = $$;

          if ($2) {
              printf("\nAST:\n");
              printAST($2, 0);
              printf("\n");
              printTable();
          }
      }
    | input function_definition
      {
          if ($1 == NULL)
              $$ = $2;
          else
              $$ = createNodeSeq($1, $2);

          root = $$;

          if ($2) {
              printf("\nAST:\n");
              printAST($2, 0);
              printf("\n");
              printTable();
          }
      }
    | input error SEMICOLON
      {
          fprintf(stderr,
                  "[ERRO SINTATICO] Erro recuperado ate ';'\n");
          yyerrok;
          yyclearin;
          $$ = $1;
      }
    ;

statement:
    declaration SEMICOLON { $$ = $1; }
  | assignment SEMICOLON { $$ = $1; }
  | function_call SEMICOLON { $$ = $1; }
  | return_statement SEMICOLON { $$ = $1; }
  | loop { $$ = $1; }
  | conditional { $$ = $1; }
  | BREAK SEMICOLON
      {
        if (loopCounter <= 0) {
          yyerror("Erro semântico [L%d:C%d]: break fora de laço");
          YYABORT;
        }
        $$ = createNode(AST_BREAK);
      }
  | CONTINUE SEMICOLON
      {
        if (loopCounter <= 0) {
          yyerror("Erro semântico [L%d:C%d]: continue fora de laço");
          YYABORT;
        }
        $$ = createNode(AST_CONTINUE);
      }
  ;

statements:
      /* vazio */ { $$ = NULL; }
    | statements statement { $$ = createNodeSeq($1, $2); }
    ;

/* Regra para: int x; */
declaration:
    INT IDENT
    {
      if (searchSymbolInCurrentScope($2)) {
        if (checkTypeConflict($2, "int")) {
          char _msg[128];
          snprintf(_msg, sizeof(_msg), "Erro semântico [L%d:C%d]: redeclaração com tipo diferente: %s", yyline, yycolumn - (int)strlen($2), $2);
          yyerror(_msg);
          YYABORT;
        } else {
          fprintf(stderr, "Aviso semântico [L%d:C%d]: símbolo já declarado: %s\n", yyline, yycolumn - (int)strlen($2), $2);
        }
      } else {
        insertSymbol($2, "int", yyline, yycolumn - (int)strlen($2));
      }

      $$ = createNodeDecl(
        "int",
        createNodeId($2),
        NULL
      );
    }
  | FLOAT IDENT
    {
      if (searchSymbolInCurrentScope($2)) {
        if (checkTypeConflict($2, "float")) {
          char _msg[128];
          snprintf(_msg, sizeof(_msg), "Erro semântico [L%d:C%d]: redeclaração com tipo diferente: %s", yyline, yycolumn - (int)strlen($2), $2);
          yyerror(_msg);
          YYABORT;
        } else {
          fprintf(stderr, "Aviso semântico [L%d:C%d]: símbolo já declarado: %s\n", yyline, yycolumn - (int)strlen($2), $2);
        }
      } else {
        insertSymbol($2, "float", yyline, yycolumn - (int)strlen($2));
      }

      $$ = createNodeDecl(
        "float",
        createNodeId($2),
        NULL
      );
    }
  | INT IDENT ASSIGN expr
    {
      if (searchSymbolInCurrentScope($2)) {
        if (checkTypeConflict($2, "int")) {
          char _msg[128];
          snprintf(_msg, sizeof(_msg), "Erro semântico [L%d:C%d]: redeclaração com tipo diferente: %s", yyline, yycolumn - (int)strlen($2), $2);
          yyerror(_msg);
          YYABORT;
        } else {
          fprintf(stderr, "Aviso semântico [L%d:C%d]: símbolo já declarado: %s\n", yyline, yycolumn - (int)strlen($2), $2);
        }
      } else {
        insertSymbol($2, "int", yyline, yycolumn - (int)strlen($2));
      }
      /* checar compatibilidade de tipos entre declaração e expressão atribuída (Bottom-up AST) */
      {
        const char *rhsType = $4->dataType;
        if (rhsType && strlen(rhsType) > 0 && strcmp(rhsType, "int") != 0) {
          char _msg[128];
          snprintf(_msg, sizeof(_msg), "Erro semântico [L%d:C%d]: atribuição com tipo incompatível na declaração: %s", yyline, yycolumn - (int)strlen($2), $2);
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
      if (searchSymbolInCurrentScope($2)) {
        if (checkTypeConflict($2, "float")) {
          char _msg[128];
          snprintf(_msg, sizeof(_msg), "Erro semântico [L%d:C%d]: redeclaração com tipo diferente: %s", yyline, yycolumn - (int)strlen($2), $2);
          yyerror(_msg);
          YYABORT;
        } else {
          fprintf(stderr, "Aviso semântico [L%d:C%d]: símbolo já declarado: %s\n", yyline, yycolumn - (int)strlen($2), $2);
        }
      } else {
        insertSymbol($2, "float", yyline, yycolumn - (int)strlen($2));
      }
      /* checar compatibilidade de tipos entre declaração e expressão atribuída (Bottom-up AST) */
      {
        const char *rhsType = $4->dataType;
        if (rhsType && strlen(rhsType) > 0 && strcmp(rhsType, "float") != 0) {
          char _msg[128];
          snprintf(_msg, sizeof(_msg), "Erro semântico [L%d:C%d]: atribuição com tipo incompatível na declaração: %s", yyline, yycolumn - (int)strlen($2), $2);
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
        snprintf(_msg, sizeof(_msg), "Erro semântico [L%d:C%d]: atribuição a símbolo não declarado: %s", yyline, yycolumn - (int)strlen($1), $1);
        yyerror(_msg);
        YYABORT;
      } else {
        /* checar compatibilidade de tipos entre LHS e RHS (Bottom-up AST) */
        {
          const char *lhsType = getSymbolType($1);
          const char *rhsType = $3->dataType;
          if (rhsType && strlen(rhsType) > 0 && lhsType && !isAssignable(lhsType, rhsType)) {
            char _msg[128];
            snprintf(_msg, sizeof(_msg), "Erro semântico [L%d:C%d]: incompatibilidade de tipos na atribuição: %s", yyline, yycolumn - (int)strlen($1), $1);
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
        snprintf(_msg, sizeof(_msg), "Erro semântico [L%d:C%d]: incremento em símbolo não declarado: %s", yyline, yycolumn - (int)strlen($1) - 2, $1);
        yyerror(_msg);
        YYABORT;
      } else {
        const char *t = getSymbolType($1);
        if (!t || strcmp(t, "int") != 0) {
          char _msg[128];
          snprintf(_msg, sizeof(_msg), "Erro semântico [L%d:C%d]: incremento apenas permitido para int: %s", yyline, yycolumn - (int)strlen($1) - 2, $1);
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
        snprintf(_msg, sizeof(_msg), "Erro semântico [L%d:C%d]: decremento em símbolo não declarado: %s", yyline, yycolumn - (int)strlen($1) - 2, $1);
        yyerror(_msg);
        YYABORT;
      } else {
        const char *t = getSymbolType($1);
        if (!t || strcmp(t, "int") != 0) {
          char _msg[128];
          snprintf(_msg, sizeof(_msg), "Erro semântico [L%d:C%d]: decremento apenas permitido para int: %s", yyline, yycolumn - (int)strlen($1) - 2, $1);
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
    expr OR expr {
        $$ = createNodeBinOp("||", $1, $3);
    }
  | expr AND expr {
        $$ = createNodeBinOp("&&", $1, $3); 
    }
  | expr EQ expr {
        $$ = createNodeBinOp("==", $1, $3);
    }
  | expr NEQ expr {
        $$ = createNodeBinOp("!=", $1, $3);
    }
  | expr LT expr {
        $$ = createNodeBinOp("<", $1, $3);
    }
  | expr LEQ expr {
        $$ = createNodeBinOp("<=", $1, $3);
    }
  | expr GT expr {
        $$ = createNodeBinOp(">", $1, $3);
    }
  | expr GEQ expr {
        $$ = createNodeBinOp(">=", $1, $3);
    }
  | expr PLUS expr {
        $$ = createNodeBinOp("+", $1, $3);
    }
  | expr MINUS expr {
        $$ = createNodeBinOp("-", $1, $3);
    }
  | expr TIMES expr {
        $$ = createNodeBinOp("*", $1, $3);
    }
  | expr DIVIDE expr {
        $$ = createNodeBinOp("/", $1, $3);
    }
  | NOT expr {
        $$ = createNodeUnOp("!", $2);
    }
  | MINUS expr %prec NOT {
        $$ = createNodeUnOp("-", $2);
    }
  | LPAREN expr RPAREN {
        $$ = $2;
    }
  | NUM {
        $$ = createNodeNum($1);
    }
  | function_call {
        $$ = $1;
    }
  | FLOAT_NUM {
      $$ = createNodeFloat($1);
    }
  | IDENT { 
        if (!searchSymbol($1))
            fprintf(stderr, "Aviso semântico [L%d:C%d]: símbolo não declarado: %s\n", yyline, yycolumn - (int)strlen($1), $1);
        $$ = createNodeId($1);
    }

  ;


block: 
    LBRACE { pushScope(); } statements RBRACE {
        $$ = $3;
        popScope();
        printf("INFO: Bloco de codigo detectado\n");
    }
    ;

/* Laços WHILE e FOR e DO...WHILE */

init_for:
      declaration { $$ = $1; }
    | assignment { $$ = $1; }
    | /* vazio */ { $$ = NULL; }
    ;

cond_for:
    expr { $$ = $1; }
    | /* vazio */ { $$ = NULL; }
    ;

step_for:
      assignment { $$ = $1; }
    | /* vazio */ { $$ = NULL; }
    ;

body:
      statement { $$ = $1; }
    | block { $$ = $1; }
    ;

loop:
    WHILE
      {
        loopCounter++;
      }
      LPAREN expr RPAREN body
        {
          if (!isConditionValid($4)) {
            char _msg[128];
           snprintf(_msg, sizeof(_msg),"Erro semântico [L%d:C%d]: condição inválida no while", yyline, yycolumn);
            yyerror(_msg);
            YYABORT;
          }

          loopCounter--;
          $$ = createNodeWhile($4, $6);
          printf("INFO: Laço WHILE detectado\n");
        }
  | FOR LPAREN
      {
        pushScope();
        loopCounter++;
      }
      init_for SEMICOLON cond_for SEMICOLON step_for RPAREN body
        {
          if ($6 && !isConditionValid($6)) {
            char _msg[128];
           snprintf(_msg, sizeof(_msg),"Erro semântico [L%d:C%d]: condição inválida no for", yyline, yycolumn);
            yyerror(_msg);
            YYABORT;
          }

          loopCounter--;
          $$ = createNodeFor($4, $6, $8, $10);
          popScope();
          printf("INFO: Laço FOR detectado\n");
        }
  | DO
      {
        loopCounter++;
      }
      body WHILE LPAREN expr RPAREN SEMICOLON
        {
          if (!isConditionValid($6)) {
            char _msg[128];
            snprintf(_msg, sizeof(_msg),"Erro semântico [L%d:C%d]: condição inválida no do-while",yyline, yycolumn);
            yyerror(_msg);
            YYABORT;
          }

          loopCounter--;
          $$ = createNodeDoWhile($3, $6);
          printf("INFO: Laço DO...WHILE detectado\n");
        }
  ;

/* Condicionais if-else (resolvem dangling-else) */
conditional:
    IF LPAREN expr RPAREN body %prec LOWER_THAN_ELSE
      {
        if (!isConditionValid($3)) {
          char _msg[128];
          snprintf(_msg, sizeof(_msg),"Erro semântico [L%d:C%d]: condição inválida no if",yyline, yycolumn);
          yyerror(_msg);
          YYABORT;
        }
        
        $$ = createNodeIf($3, $5, NULL);
        printf("SUCESSO: Declaração if realizada.\n");
      }
  | IF LPAREN expr RPAREN body ELSE body
      {
        if (!isConditionValid($3)) {
          char _msg[128];
          snprintf(_msg, sizeof(_msg),"Erro semântico [L%d:C%d]: condição inválida no if", yyline, yycolumn);
          yyerror(_msg);
          YYABORT;
        }

        $$ = createNodeIf($3, $5, $7);
        printf("SUCESSO: Declaração if-else realizada.\n");
      }
    | expr QUESTION body COLON body {
          $$ = createNodeIf($1, $3, $5);
          printf("SUCESSO: Declaração condicional com operador ternário realizada.\n");
      }
    ;

/* Funções: definição, parâmetros, chamadas e retorno */
type_specifier:
      INT { $$ = "int"; }
    | FLOAT { $$ = "float"; }
    ;

function_definition:
    INT IDENT LPAREN
    {
      if (searchFunction($2)) {
        char _msg[128];
        snprintf(_msg, sizeof(_msg), "Erro semântico [L%d:C%d]: função já declarada: %s", yyline, yycolumn - (int)strlen($2) - 1, $2);
        yyerror(_msg);
        YYABORT;
      }

      insertFunction($2, "int", yyline, yycolumn - (int)strlen($2) - 1);
      current_function_return_type = "int";
      current_function_has_return = 0;
      pushScope();
    }
    parameter_list_opt RPAREN function_block
    {
  $$ = createNodeFunc($2, "int", $5, $7);
      setFunctionAst($2, $$);
      popScope();
      if (!current_function_has_return) {
        fprintf(stderr, "Aviso semântico [L%d:C%d]: função '%s' não possui return\n", yyline, yycolumn, $2);
      }
      current_function_return_type = NULL;
      printf("INFO: Função definida: %s\n", $2);
    }
  | FLOAT IDENT LPAREN
    {
      if (searchFunction($2)) {
        char _msg[128];
        snprintf(_msg, sizeof(_msg), "Erro semântico [L%d:C%d]: função já declarada: %s", yyline, yycolumn - (int)strlen($2) - 1, $2);
        yyerror(_msg);
        YYABORT;
      }

      insertFunction($2, "float", yyline, yycolumn - (int)strlen($2) - 1);
      current_function_return_type = "float";
      current_function_has_return = 0;
      pushScope();
    }
    parameter_list_opt RPAREN function_block
    {
  $$ = createNodeFunc($2, "float", $5, $7);
      setFunctionAst($2, $$);
      popScope();
      if (!current_function_has_return) {
        fprintf(stderr, "Aviso semântico [L%d:C%d]: função '%s' não possui return\n", yyline, yycolumn, $2);
      }
      current_function_return_type = NULL;
      printf("INFO: Função definida: %s\n", $2);
    }
    ;

function_block:
    LBRACE statements RBRACE
    {
      $$ = $2;
      printf("INFO: Bloco de codigo detectado\n");
    }
    ;

parameter_list_opt:
      /* vazio */ { $$ = NULL; }
    | parameter_list { $$ = $1; }
    ;

parameter_list:
      parameter { $$ = $1; }
    | parameter_list COMMA parameter { $$ = createNodeSeq($1, $3); }
    ;


parameter:
      type_specifier IDENT
      {
        if (searchSymbolInCurrentScope($2)) {
          fprintf(stderr, "Aviso semântico [L%d:C%d]: parâmetro já declarado: %s\n", yyline, yycolumn - (int)strlen($2), $2);
        } else {
          insertSymbol($2, $1, yyline, yycolumn - (int)strlen($2));
        }

        $$ = createNodeDecl($1, createNodeId($2), NULL);
        printf("INFO: Parametro: %s\n", $2);
      }
    ;

function_call:
    IDENT
    {
      if (!searchFunction($1)) {
        char _msg[128];
        snprintf(_msg, sizeof(_msg), "Erro semântico [L%d:C%d]: função não declarada: %s", yyline, yycolumn - (int)strlen($1), $1);
        yyerror(_msg);
        YYABORT;
      }
    }
    LPAREN arg_list_opt RPAREN
    {
      char _msg[160];
      if (!checkFunctionCallArgs($1, $4, _msg, sizeof(_msg))) {
        yyerror(_msg);
        YYABORT;
      }
      $$ = createNodeCall($1, $4);
    }
    ;

arg_list_opt:
      /* vazio */ { $$ = NULL; }
    | arg_list { $$ = $1; }
    ;

arg_list:
      expr { $$ = $1; }
    | arg_list COMMA expr { $$ = createNodeSeq($1, $3); }
    ;

return_statement:
      RETURN expr {
        if (current_function_return_type && $2 && strlen($2->dataType) > 0 && strcmp(current_function_return_type, $2->dataType) != 0) {
          char _msg[160];
          snprintf(_msg, sizeof(_msg), "Aviso semântico [L%d:C%d]: tipo de retorno incompatível, esperado %s", yyline, yycolumn, current_function_return_type);
          fprintf(stderr, "%s\n", _msg);
        }
        current_function_has_return = 1;
        $$ = createNodeReturn($2);
      }
    | RETURN {
        current_function_has_return = 1;
        $$ = createNodeReturn(NULL);
      }
    ;

%%

int main(void) {
    printf("Digite expressoes terminadas com ';'. Pressione Ctrl+D para encerrar.\n");
    initTable();
    int ret = yyparse();
  if (root) {
    printf("INFO: Gerando TAC a partir da AST...\n");
    generateTAC(root);
  }
  freeTable();
    return ret;
}

void yyerror(const char *s) {
  if (strncmp(s, "Erro semântico", 14) == 0) {
    fprintf(stderr, "%s\n", s);
  } else {
    fprintf(stderr, "Erro sintático [L%d:C%d]: %s\n", yyline, yycolumn, s);
  }
}
