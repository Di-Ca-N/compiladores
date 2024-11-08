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
#include <string.h>
#include <tree.h>
int yylex(void);
void yyerror (char const *mensagem);
extern int get_line_number();
extern void* arvore;
struct node_t *binary_op(node_type_t type, char* label, struct node_t *left, struct node_t *right);
struct node_t *list_append(struct node_t *head, struct node_t *item);
%}

%define parse.error verbose 
%union {
    struct node_t *ast_node;
    struct lexical_value_t *lexical_value;
};

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
%token<lexical_value> TK_IDENTIFICADOR
%token<lexical_value> TK_LIT_INT
%token<lexical_value> TK_LIT_FLOAT
%token TK_ERRO

%type<ast_node> expressao0 expressao1 expressao2 expressao3 expressao4 expressao5 expressao6 expressao chamadaFuncao
%type<ast_node> terminal_identificador terminal_lit_float terminal_lit_int comandoSimples atribuicao declaracaoVariavel
%type<ast_node> retorno blocoIf blocoElse blocoWhile blocoDeComandos listaDeComandos comando argumento listaDeArgumentos
%type<ast_node> identificador listaDeIdentificadores tipo corpoFuncao parametro listaDeParametros cabecalhoFuncao
%type<ast_node> funcao listaDeFuncoes programa

%% 
programa
    : listaDeFuncoes { $$ = $1; arvore = (void*) $1; }
    | %empty { $$ = NULL; }
    ;

listaDeFuncoes
    : listaDeFuncoes funcao { $$ = list_append($1, $2); } 
    | funcao { $$ = $1; }
    ;

funcao: cabecalhoFuncao corpoFuncao { $$ = $1; node_add_child($$, $2); };

cabecalhoFuncao
    : terminal_identificador '=' listaDeParametros '>' tipo { $$ = node_create(NODE_FUNC, $1->label); node_free($1); }
    | terminal_identificador '=' '>' tipo { $$ = node_create(NODE_FUNC, $1->label); node_free($1); }
    ;

listaDeParametros
    : listaDeParametros TK_OC_OR parametro { $$ = list_append($1, $3); } 
    | parametro { $$ = $1; }
    ;

parametro
    : terminal_identificador '<' '-' tipo { $$ = NULL; node_free($1); }
    ;

tipo
    : TK_PR_INT   { $$ = NULL; }
    | TK_PR_FLOAT { $$ = NULL; }
    ;

corpoFuncao: blocoDeComandos { $$ = $1; };

blocoDeComandos
    : '{' listaDeComandos '}' { $$ = $2; }
    | '{' '}'                 { $$ = NULL; }
    ;

listaDeComandos
    : listaDeComandos comando { $$ = list_append($1, $2); }
    | comando  { $$ = $1; }
    ;

comando
    : comandoSimples ';' { $$ = $1; } 
    ;

comandoSimples
    : declaracaoVariavel  { $$ = $1; }
    | blocoDeComandos     { $$ = $1; }
    | atribuicao          { $$ = $1; }
    | chamadaFuncao       { $$ = $1; }
    | retorno             { $$ = $1; }
    | blocoIf             { $$ = $1; }
    | blocoWhile          { $$ = $1; }
    ;

declaracaoVariavel: tipo listaDeIdentificadores { $$ = $2; };
listaDeIdentificadores
    : listaDeIdentificadores ',' identificador { $$ = list_append($1, $3); }
    | identificador { $$ = $1; }
    ;

identificador
    : terminal_identificador { $$ = NULL; node_free($1); }
    | terminal_identificador TK_OC_LE terminal_lit_float { $$ = binary_op(NODE_VAR_INIT, "<=", $1, $3); }
    | terminal_identificador TK_OC_LE terminal_lit_int { $$ = binary_op(NODE_VAR_INIT, "<=", $1, $3); }
    ;

atribuicao 
    : terminal_identificador '=' expressao { $$ = binary_op(NODE_ASSIGN, "=", $1, $3); }
    ;

chamadaFuncao
    : terminal_identificador '(' listaDeArgumentos ')' 
        { 
            char *dest = malloc(strlen("call ") + strlen($1->label) + 1); 
            strcpy(dest, "call ");
            strcat(dest, $1->label); 
            $$ = node_create(NODE_FUNC_CALL, dest);
            node_add_child($$, $3);
            free(dest);
            node_free($1);
        }
    ;

listaDeArgumentos
    : listaDeArgumentos ',' argumento { node_append($1, $3); $$ = $1; }
    | argumento                       { $$ = $1; }
    ;

argumento
    : expressao { $$ = $1; }
    ;

retorno
    : TK_PR_RETURN expressao { $$ = node_create(NODE_RETURN, "return"); node_add_child($$, $2); }
    ;

blocoIf
    : TK_PR_IF '(' expressao ')' blocoDeComandos blocoElse 
    {
        $$ = node_create(NODE_IF, "if");
        node_add_child($$, $3);
        if($5 != NULL) node_add_child($$, $5);
        if($6 != NULL) node_add_child($$, $6);
    }
    ;

blocoElse
    : TK_PR_ELSE blocoDeComandos { $$ = $2; }
    | %empty { $$ = NULL; }
    ;

blocoWhile
    : TK_PR_WHILE '(' expressao ')' blocoDeComandos 
        { 
            $$ = node_create(NODE_WHILE, "while"); 
            node_add_child($$, $3); 
            if ($5 != NULL) {
                node_add_child($$, $5); 
            }
        }
    ;

expressao
    : expressao6                     { $$ = $1; }
    | expressao TK_OC_OR expressao6  { $$ = binary_op(NODE_EXPR, "|", $1, $3); }
    ;

expressao6
    : expressao5                       { $$ = $1; }
    | expressao6 TK_OC_AND expressao5  { $$ = binary_op(NODE_EXPR, "&", $1, $3); }
    ;

expressao5
    : expressao4                      { $$ = $1; }
    | expressao5 TK_OC_EQ expressao4  { $$ = binary_op(NODE_EXPR, "==", $1, $3); }
    | expressao5 TK_OC_NE expressao4  { $$ = binary_op(NODE_EXPR, "!=", $1, $3); }
    ;

expressao4
    : expressao3                      { $$ = $1; }
    | expressao4 '<' expressao3       { $$ = binary_op(NODE_EXPR, "<", $1, $3); }
    | expressao4 '>' expressao3       { $$ = binary_op(NODE_EXPR, ">", $1, $3); }
    | expressao4 TK_OC_LE expressao3  { $$ = binary_op(NODE_EXPR, "<=", $1, $3); }
    | expressao4 TK_OC_GE expressao3  { $$ = binary_op(NODE_EXPR, ">=", $1, $3); }
    ;

expressao3
    : expressao2                 { $$ = $1; }
    | expressao3 '+' expressao2  { $$ = binary_op(NODE_EXPR, "+", $1, $3); }
    | expressao3 '-' expressao2  { $$ = binary_op(NODE_EXPR, "-", $1, $3); }

expressao2
    : expressao1                 { $$ = $1; }
    | expressao2 '*' expressao1  { $$ = binary_op(NODE_EXPR, "*", $1, $3); }
    | expressao2 '/' expressao1  { $$ = binary_op(NODE_EXPR, "/", $1, $3); }
    | expressao2 '%' expressao1  { $$ = binary_op(NODE_EXPR, "%", $1, $3); }
    ;

expressao1
    : expressao0      { $$ = $1; }
    | '-' expressao1  { $$ = node_create(NODE_EXPR, "-"); node_add_child($$, $2); }
    | '!' expressao1  { $$ = node_create(NODE_EXPR, "!"); node_add_child($$, $2); }
    ;

expressao0
    : '(' expressao ')'      { $$ = $2; }
    | terminal_lit_float     { $$ = $1; }
    | terminal_lit_int       { $$ = $1; }
    | terminal_identificador { $$ = $1; }
    | chamadaFuncao          { $$ = $1; }
    ;

terminal_identificador
    : TK_IDENTIFICADOR  { $$ = node_create(NODE_IDENTIFIER, $1->value); lexical_value_free($1); }
    ;

terminal_lit_float
    : TK_LIT_FLOAT  { $$ = node_create(NODE_FLOAT_LITERAL, $1->value); lexical_value_free($1); }
    ;

terminal_lit_int
    : TK_LIT_INT  { $$ = node_create(NODE_INT_LITERAL, $1->value); lexical_value_free($1); }
    ;

%%
void yyerror (char const *mensagem) {
    fprintf(stderr, "Error at line %d: %s\n", get_line_number(), mensagem);
}

struct node_t *binary_op(node_type_t type, char* label, struct node_t *left, struct node_t *right) {
    struct node_t *root = node_create(type, label); 
    node_add_child(root, left); 
    node_add_child(root, right);
    return root;  
}

struct node_t *list_append(struct node_t *head, struct node_t *item) {
    if (head == NULL && item == NULL) {
        return NULL;
    } else if (head == NULL && item != NULL) {
        return item;
    } else if (head != NULL && item == NULL) {
        return head;
    } else {
        node_append(head, item);
        return head;
    }
}