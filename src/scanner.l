%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.tab.h"  /* contém definições geradas pelo Bison (NUM, PLUS, etc.) */

int yyline = 1;
int yycolumn = 1;

#define YY_USER_ACTION \
    for (int i = 0; yytext[i] != '\0'; i++) { \
        if (yytext[i] == '\n') { \
            yyline++; \
            yycolumn = 1; \
        } else { \
            yycolumn++; \
        } \
    }
%}

/* Definições de padrões de token e ações */
%%

break       { return BREAK; }
continue    { return CONTINUE; }
do          { return DO; }
else        { return ELSE; }
float       { return FLOAT; }
for         { return FOR; }
func        { return FUNC; }
if          { return IF; }
int         { return INT; }
return      { return RETURN; }
while       { return WHILE; }

[ \t\r\n]+    { /* ignora espaços, tabs e quebras de linha */ }

"//".*     { /* Ignorar comentários de linha */ }
"/*"([^*]|\*+[^*/])*\*+\/    { /* Ignorar comentários de bloco */ }

"=="   { return EQ; }
"!="   { return NEQ; }
"<="   { return LEQ; }
">="   { return GEQ; }
"++"   { return INCREMENT; }
"--"   { return DECREMENT; }
"+"    { return PLUS; }
"-"    { return MINUS; }
"*"    { return TIMES; }
"/"    { return DIVIDE; }
"%"    { return MOD; }
"="    { return ASSIGN; }
"<"    { return LT; }
">"    { return GT; }
"("    { return LPAREN; }
")"    { return RPAREN; }
"{"    { return LBRACE; }
"}"    { return RBRACE; }
","    { return COMMA; }
";"    { return SEMICOLON; }
"&&"   { return AND; }
"||"   { return OR; }
"!"    { return NOT; }

[a-zA-Z_][a-zA-Z0-9_]*  { yylval.id = strdup(yytext); return IDENT; }

[0-9]+"."[0-9]+ {
    yylval.floatValue = atof(yytext);
    return FLOAT_NUM;
}

[0-9]+  { yylval.intValue = atoi(yytext); return NUM; } 

.  { printf("UNKNOWN(%s)\n", yytext); }

%%

/* Função auxiliar do Flex (quando termina o arquivo de entrada) */
int yywrap(void) {
    return 1;
}