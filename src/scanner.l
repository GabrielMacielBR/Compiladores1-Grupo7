%{
#include <stdio.h>
#include <stdlib.h>
#include "parser.tab.h"  /* contém definições geradas pelo Bison (NUM, PLUS, etc.) */
%}

/* Definições de padrões de token e ações */
%%

auto        { /* return AUTO; */ }
break       { /* return BREAK; */ }
case        { /* return CASE; */ }
char        { /* return CHAR; */ }
const       { /* return CONST; */ }
continue    { /* return CONTINUE; */ }
default     { /* return DEFAULT; */ }
do          { /* return DO; */ }
double      { /* return DOUBLE; */ }
else        { /* return ELSE; */ }
enum        { /* return ENUM; */ }
extern      { /* return EXTERN; */ }
float       {  return FLOAT;  }
for         { /* return FOR; */ }
goto        { /* return GOTO; */ }
if          { /* return IF; */ }
int         {  return INT;  }
long        { /* return LONG; */ }
return      { /* return RETURN; */ }
short       { /* return SHORT; */ }
signed      { /* return SIGNED; */ }
sizeof      { /* return SIZEOF; */ }
static      { /* return STATIC; */ }
struct      { /* return STRUCT; */ }
switch      { /* return SWITCH; */ }
typedef     { /* return TYPEDEF; */ }
union       { /* return UNION; */ }
unsigned    { /* return UNSIGNED; */ }
void        { /* return VOID; */ }
volatile    { /* return VOLATILE; */ }
while       { /* return WHILE; */ }

[ \t\n]+  { /* Não faz nada */ }

"//".*     { /* Ignorar comentários de linha */ }
"/*"([^*]|\*+[^*/])*\*+\/    { /* Ignorar comentários de bloco */ }

"=="   { return EQ; }
"++"   { /* return INCREMENT; */ }
"--"   { /* return DECREMENT; */ }
"+="   { /* return PLUS_ASSIGN; */ }
"-="   { /* return MINUS_ASSIGN; */ }
"*="   { /* return TIMES_ASSIGN; */ }
"/="   { /* return DIVIDE_ASSIGN; */ }
"%="   { /* return MOD_ASSIGN; */ }
"!="   { return NEQ; }
"<="   { return LEQ; }
">="   { return GEQ; }
"->"   { /* return ARROW; */ }
"+"    {  return PLUS;  }
"-"    {  return MINUS;  }
"*"    {  return TIMES;  }
"/"    {  return DIVIDE;  }
"^"    { /* return POWER; */ } 
"%"    { /* return MOD; */ }
"="    {  return ASSIGN;  }
"<"    { return LT; }
">"    { return GT; }
"["    { /* return LBRACKET; */ }
"]"    { /* return RBRACKET; */ }
"("    {  return LPAREN;  }
")"    {  return RPAREN;  }
"{"    { /* return LBRACE; */ }
"}"    { /* return RBRACE; */ }
","    { /* return COMMA; */ }
":"    { /* return COLON; */ }
";"    {  return SEMICOLON;  }
"&&"   { return AND; }
"||"   { return OR; }
"!"    { return NOT; }

\"([^\"\n]*)\" { /* return STRING; */ }

[\const\w=]    { /* return CONST; */ }

[a-zA-Z_][a-zA-Z0-9_]*  {  return IDENT;  }

[0-9]+  { yylval.intValue = atoi(yytext); return NUM; } 

.  { printf("UNKNOWN(%s)\n", yytext); }

%%

/* Função auxiliar do Flex (quando termina o arquivo de entrada) */
int yywrap(void) {
    return 1;
}