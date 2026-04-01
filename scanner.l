%{
#include <stdio.h>
%}

/* Definições de padrões de token e ações */
%%

auto        { printf("KW_AUTO\n"); }
break       { printf("KW_BREAK\n"); }
case        { printf("KW_CASE\n"); }
char        { printf("KW_CHAR\n"); }
const       { printf("KW_CONST\n"); }
continue    { printf("KW_CONTINUE\n"); }
default     { printf("KW_DEFAULT\n"); }
do          { printf("KW_DO\n"); }
double      { printf("KW_DOUBLE\n"); }
else        { printf("KW_ELSE\n"); }
enum        { printf("KW_ENUM\n"); }
extern      { printf("KW_EXTERN\n"); }
float       { printf("KW_FLOAT\n"); }
for         { printf("KW_FOR\n"); }
goto        { printf("KW_GOTO\n"); }
if          { printf("KW_IF\n"); }
int         { printf("KW_INT\n"); }
long        { printf("KW_LONG\n"); }
register    { printf("KW_INT\n"); }
return      { printf("KW_RETURN\n"); }
short       { printf("KW_SHORT\n"); }
signed      { printf("KW_SIGNED\n"); }
sizeof      { printf("KW_SIZEOF\n"); }
static      { printf("KW_STATIC\n"); }
struct      { printf("KW_STRUCT\n"); }
switch      { printf("KW_SWITCH\n"); }
typedef     { printf("KW_TYPEDEF\n"); }
union       { printf("KW_UNION\n"); }
unsigned    { printf("KW_UNSIGNED\n"); }
void        { printf("KW_VOID\n"); }
volatile    { printf("KW_VOLATILE\n"); }
while       { printf("KW_WHILE\n"); }

"//".*     { /* Ignorar comentários de linha */ }

"/*"([^*]|\*+[^*/])*\*+\/    { /* Ignorar comentários de bloco */ }

[\[\]\(\)\{\}\,\:\;\*\==\=\#\.\~]    { printf("POUNCTUATORS(%s)\n", yytext);}

[\"\w]    { printf("STRING(%s)\n", yytext);}

[\const\w=]    { printf("CONST(%s)\n", yytext);}

[a-zA-Z_][a-zA-Z0-9_]*  { printf("IDENT(%s)\n", yytext); }

[0-9]+(\.[0-9]+)?  { printf("NUMBER(%s)\n", yytext); }

[+\-*/=]   { printf("OPERATOR(%s)\n", yytext); }

[ \t\n]+  { /* Não faz nada */ }

.  { printf("UNKNOWN(%s)\n", yytext); }

%%

/* Função auxiliar do Flex (quando termina o arquivo de entrada) */
int yywrap(void) {
    return 1;
}

/* Função principal: executa o analisador léxico */
int main(void) {
    yylex();
    return 0;
}
