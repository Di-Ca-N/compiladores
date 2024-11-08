/*
Grupo:
- Diego Cardoso Nunes (00333984)
- Geancarlo Kozenieski (00264414)
*/
%{
#include "parser.tab.h"
#include "tree.h"

struct lexical_value_t *lexical_value(lexical_type_t type, char* label);

%}
%option yylineno
%option noinput
%option nounput

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

[!\*/+\-%<>(){}\=,;] { return yytext[0]; }

[a-z_][a-z0-9_]* { yylval.lexical_value = lexical_value_create(TYPE_IDENTIFIER, yytext); return TK_IDENTIFICADOR;}
[0-9]+ { yylval.lexical_value = lexical_value_create(TYPE_LIT_INT, yytext); return TK_LIT_INT; }
[0-9]*"."[0-9]+ { yylval.lexical_value = lexical_value_create(TYPE_LIT_FLOAT, yytext); return TK_LIT_FLOAT; }

[[:blank:]]
"\n"
"//".*

. return TK_ERRO;

%%
int get_line_number(void) {
    return yylineno;
}


