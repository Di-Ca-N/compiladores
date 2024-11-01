/*
Grupo:
- Diego Cardoso Nunes (00333984)
- Geancarlo Kozenieski (00264414)
*/

%code requires {
    #include "tree.h"
}

%{
#include <stdio.h>
int yylex(void);
void yyerror (char const *mensagem);
extern int get_line_number();
%}

%define parse.error verbose 
%define api.value.type { struct node_t* }

%token TK_PR_INT
%token TK_PR_FLOAT
%token TK_PR_IF
%token TK_PR_ELSE
%token TK_PR_WHILE
%token TK_PR_RETURN
%token TK_OC_LE
%token TK_OC_GE
%token TK_OC_EQ
%token TK_OC_NE
%token TK_OC_AND
%token TK_OC_OR
%token TK_IDENTIFICADOR
%token TK_LIT_INT
%token TK_LIT_FLOAT
%token TK_ERRO


%% 
programa: listaDeFuncoes | %empty;
listaDeFuncoes: listaDeFuncoes funcao | funcao;
funcao: cabecalhoFuncao corpoFuncao;
cabecalhoFuncao: TK_IDENTIFICADOR '=' listaDeParametros '>' tipo | TK_IDENTIFICADOR '=' '>' tipo;
listaDeParametros: listaDeParametros TK_OC_OR parametro | parametro;
parametro: TK_IDENTIFICADOR '<' '-' tipo;
tipo: TK_PR_INT | TK_PR_FLOAT;
corpoFuncao: blocoDeComandos;

blocoDeComandos: '{' listaDeComandos '}' | '{' '}';
listaDeComandos: listaDeComandos comando | comando;
comando: comandoSimples ';' ; 
comandoSimples
    : declaracaoVariavel
    | blocoDeComandos
    | atribuicao
    | chamadaFuncao
    | retorno
    | blocoIf
    | blocoWhile
    ;

declaracaoVariavel: tipo listaDeIdentificadores;
listaDeIdentificadores: listaDeIdentificadores ',' identificador | identificador;
identificador: TK_IDENTIFICADOR | TK_IDENTIFICADOR TK_OC_LE TK_LIT_FLOAT | TK_IDENTIFICADOR TK_OC_LE TK_LIT_INT;

atribuicao 
    : TK_IDENTIFICADOR '=' expressao
    ;

chamadaFuncao: TK_IDENTIFICADOR '(' listaDeArgumentos ')';
listaDeArgumentos: listaDeArgumentos ',' argumento | argumento;
argumento: expressao;

retorno: TK_PR_RETURN expressao;

blocoIf: TK_PR_IF '(' expressao ')' blocoDeComandos blocoElse ;
blocoElse: TK_PR_ELSE blocoDeComandos | %empty;

blocoWhile: TK_PR_WHILE '(' expressao ')' blocoDeComandos;

expressao
    : expressao6
    | expressao TK_OC_OR expressao6 
    ;

expressao6
    : expressao5
    | expressao6 TK_OC_AND expressao5 
    ;

expressao5
    : expressao4
    | expressao5 TK_OC_EQ expressao4
    | expressao5 TK_OC_NE expressao4
    ;

expressao4
    : expressao3
    | expressao4 '<' expressao3
    | expressao4 '>' expressao3
    | expressao4 TK_OC_LE expressao3
    | expressao4 TK_OC_GE expressao3
    ;

expressao3
    : expressao2
    | expressao3 '+' expressao2
    | expressao3 '-' expressao2

expressao2
    : expressao1
    | expressao2 '*' expressao1
    | expressao2 '/' expressao1
    | expressao2 '%' expressao1
    ;

expressao1
    : expressao0
    | '-' expressao1
    | '!' expressao1
    ;

expressao0
    : '(' expressao ')'
    | TK_LIT_FLOAT
    | TK_LIT_INT
    | TK_IDENTIFICADOR
    | chamadaFuncao
    ;

%%
void yyerror (char const *mensagem) {
    fprintf(stderr, "Error at line %d: %s\n", get_line_number(), mensagem);
}
