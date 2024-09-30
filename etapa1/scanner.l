%{
#include "tokens.h"
%}

%%

"int" return TK_PR_INT;
"float" return TK_PR_FLOAT;
"if" return TK_PR_IF;
"else" return TK_PR_ELSE;
"while" return TK_PR_WHILE;
"return" return TK_PR_RETURN;
"<=" return TK_OC_LE;
">=" return TK_OC_GE;
"==" return TK_OC_EQ;
"!=" return TK_OC_NE;
"&" return TK_OC_AND;
"|" return TK_OC_OR;

"\n"
"//".*

%%
int get_line_number(void) {
    return 1;
}
