/*
Grupo:
- Diego Cardoso Nunes (00333984)
- Geancarlo Kozenieski (00264414)
*/
%{
#include "parser.tab.h"
%}
%option yylineno

%%

int return TK_PR_INT;
float return TK_PR_FLOAT;
if return TK_PR_IF;
else return TK_PR_ELSE;
while return TK_PR_WHILE;
return return TK_PR_RETURN;

"<=" return TK_OC_LE;
">=" return TK_OC_GE;
"==" return TK_OC_EQ;
"!=" return TK_OC_NE;
"&" return TK_OC_AND;
"|" return TK_OC_OR;

[!\*/+\-%<>(){}=,;] return yytext[0];

[a-z_][a-z0-9_]* return TK_IDENTIFICADOR;
[0-9]+ return TK_LIT_INT;
[0-9]*"."[0-9]+ return TK_LIT_FLOAT;

[[:blank:]]
"\n"
"//".*

. return TK_ERRO;

%%
int get_line_number(void) {
    return yylineno;
}
