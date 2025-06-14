/*
Grupo:
- Diego Cardoso Nunes (00333984)
- Geancarlo Kozenieski (00264414)
*/


%{
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "tree.h"
#include "code_gen.h"
#include "table.h"
#include "semantic_checks.h"
#include "translation_helpers.h"

// Global functions and variables defined on other files
extern int get_line_number();
extern void* arvore;
extern struct symbol_table_t *scope_stack;

data_type_t data_type;

// Prototype of helper functions on this file
int yylex(void);
void yyerror (char const *mensagem);
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

%start programa
%% 

/* Helper non-terminals for semantic analysis */
empilha_tabela: %empty { scope_stack = enter_scope(scope_stack); }
desempilha_tabela: %empty { scope_stack = exit_scope(scope_stack); }


programa
    : empilha_tabela listaDeFuncoes { $$ = $2; arvore = (void*) $2; exit_scope(scope_stack); }
    | %empty { $$ = NULL; }
    ;

listaDeFuncoes
    : listaDeFuncoes funcao { $$ = node_append($1, $2); } 
    | funcao { $$ = $1; }
    ;

funcao: empilha_tabela cabecalhoFuncao corpoFuncao desempilha_tabela { $$ = $2; node_add_child($$, $3); code_print($3->code);};

cabecalhoFuncao
    : terminal_identificador '=' listaDeParametros '>' tipo 
        {
            check_no_redeclaration($1->lexical_value, scope_stack->next, SYMBOL_FUNCTION, data_type);
            declare_symbol(
                scope_stack->next, 
                create_symbol(
                    $1->label, $1->lexical_value->lineNumber, 
                    SYMBOL_FUNCTION, data_type
                )
            );

            $$ = node_create(NODE_FUNC, $1->label); 
            node_free($1);
        }
    ;

listaDeParametros
    : listaDeParametros TK_OC_OR parametro { $$ = node_append($1, $3); } 
    | parametro { $$ = $1; }
    | %empty    { $$ = NULL; }
    ;

parametro
    : terminal_identificador '<' '-' tipo 
        { 
            check_no_redeclaration($1->lexical_value, scope_stack, SYMBOL_VARIABLE, data_type);
            declare_symbol(
                scope_stack, 
                create_symbol(
                    $1->lexical_value->value,
                    $1->lexical_value->lineNumber, 
                    SYMBOL_VARIABLE, 
                    data_type
                )
            );

            $$ = NULL;
            node_free($1);
        }
    ;

tipo
    : TK_PR_INT   { $$ = NULL; data_type = DATA_INT; }
    | TK_PR_FLOAT { $$ = NULL; data_type = DATA_FLOAT; }
    ;

corpoFuncao     
    : '{' listaDeComandos '}' { $$ = $2; }
    | '{' '}'                 { $$ = NULL; }
    ;

blocoDeComandos
    : '{' empilha_tabela listaDeComandos desempilha_tabela '}'
        { $$ = $3; }
    | '{' '}'
        { $$ = NULL; }
    ;

listaDeComandos
    : listaDeComandos comando 
        {
            $$ = node_append($1, $2); 
            if ($1 != NULL && $2 != NULL) {
                $$->code = code_concat($1->code, $2->code);
            }
        }
    | comando { $$ = $1; }
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
    : listaDeIdentificadores ',' identificador { $$ = node_append($1, $3); }
    | identificador { $$ = $1; }
    ;

identificador
    : terminal_identificador 
        { 
            check_no_redeclaration($1->lexical_value, scope_stack, SYMBOL_VARIABLE, data_type);
            declare_symbol(
                scope_stack, 
                create_symbol(
                    $1->lexical_value->value, 
                    $1->lexical_value->lineNumber, 
                    SYMBOL_VARIABLE, 
                    data_type
                )
            );
            $$ = NULL; 
            node_free($1); 
        }
    | terminal_identificador TK_OC_LE terminal_lit_float 
        { 
            check_no_redeclaration($1->lexical_value, scope_stack, SYMBOL_VARIABLE, data_type);
            declare_symbol(
                scope_stack, 
                create_symbol(
                    $1->lexical_value->value, 
                    $1->lexical_value->lineNumber, 
                    SYMBOL_VARIABLE, 
                    data_type
                )
            );
            $$ = ast_binary_op(NODE_VAR_INIT, "<=", $1, $3); 
        }
    | terminal_identificador TK_OC_LE terminal_lit_int 
        { 
            check_no_redeclaration($1->lexical_value, scope_stack, SYMBOL_VARIABLE, data_type);

            declare_symbol(
                scope_stack, 
                create_symbol(
                    $1->lexical_value->value, 
                    $1->lexical_value->lineNumber, 
                    SYMBOL_VARIABLE, 
                    data_type
                )
            );
            $$ = ast_binary_op(NODE_VAR_INIT, "<=", $1, $3); 
        }
    ;

atribuicao 
    : terminal_identificador '=' expressao 
        { 
            check_was_declared($1->lexical_value, scope_stack, SYMBOL_VARIABLE);
            check_correct_usage($1->lexical_value, scope_stack, SYMBOL_VARIABLE);

            $$ = ast_binary_op(NODE_ASSIGN, "=", $1, $3); 
            $$->id_type = $3->id_type; 

            struct symbol_t *symbol = find_symbol(scope_stack, $1->label);
            char buf[100];
            snprintf(buf, sizeof(buf), "%d", symbol->offset);

            $$->location = new_temp();
            $$->code = code_concat($3->code, code_create("storeAI", $3->location, "rfp", buf));

            //code_print($$->code);

            // ToDo: Code Generation
        }
    ;

chamadaFuncao
    : terminal_identificador '(' listaDeArgumentos ')' 
        { 
            check_was_declared($1->lexical_value, scope_stack, SYMBOL_FUNCTION);
            check_correct_usage($1->lexical_value, scope_stack, SYMBOL_FUNCTION);

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
    : listaDeArgumentos ',' argumento { $$ = node_append($1, $3); }
    | argumento                       { $$ = $1; }
    ;

argumento
    : expressao { $$ = $1; }
    ;

retorno
    : TK_PR_RETURN expressao 
        { 
            $$ = node_create(NODE_RETURN, "return"); 
            node_add_child($$, $2); 
        }
    ;

blocoIf
    : TK_PR_IF '(' expressao ')' blocoDeComandos blocoElse 
        {
            $$ = node_create(NODE_IF, "if");
            node_add_child($$, $3);
            if($5 != NULL) node_add_child($$, $5);
            if($6 != NULL) node_add_child($$, $6);

            // ToDo: Code Generation
            
            char *true_label = new_label();
            char *false_label = new_label();
            char *final_label = new_label();

            $$->code = code_concat_many(
                $3->code,
                code_create("cbr", $3->location, true_label, false_label),
                safe_set_label($5, true_label),
                code_create("jumpI", NULL, final_label, NULL),
                safe_set_label($6, false_label),
                code_create_with_label(final_label, "nop", NULL, NULL, NULL),
                NULL
            );
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

            // ToDo: Code Generation
            char *true_label = new_label();
            char *false_label = new_label();
            char *back_label = new_label();

            set_label($3->code, back_label);
            
            code_t *code = safe_set_label($5, true_label);

            $$->code = code_concat_many(
                $3->code,
                code_create("cbr", $3->location, true_label, false_label),
                code,
                code_create("jumpI", NULL, back_label, NULL),
                code_create_with_label(false_label, "nop", NULL, NULL, NULL),
                NULL
            );
        }
    ;

expressao
    : expressao6 { $$ = $1; }
    | expressao TK_OC_OR expressao6  
        {
            $$ = ast_binary_op(NODE_EXPR, "|", $1, $3); 
            $$->id_type = DATA_INT; 
            code_gen_binary_op("or", $$, $1, $3); 
        }
    ;

expressao6
    : expressao5 { $$ = $1; }
    | expressao6 TK_OC_AND expressao5  
        {
            $$ = ast_binary_op(NODE_EXPR, "&", $1, $3);
            $$->id_type = DATA_INT;
            code_gen_binary_op("and", $$, $1, $3);
        }
    ;

expressao5
    : expressao4 { $$ = $1; }
    | expressao5 TK_OC_EQ expressao4 
        {
            $$ = ast_binary_op(NODE_EXPR, "==", $1, $3); 
            $$->id_type = DATA_INT;
            code_gen_binary_op("cmp_EQ", $$, $1, $3); 
        }
    | expressao5 TK_OC_NE expressao4 
        {
            $$ = ast_binary_op(NODE_EXPR, "!=", $1, $3); 
            $$->id_type = DATA_INT;
            code_gen_binary_op("cmp_NE", $$, $1, $3); 
        }
    ;

expressao4
    : expressao3 { $$ = $1; }
    | expressao4 '<' expressao3 
        {
            $$ = ast_binary_op(NODE_EXPR, "<", $1, $3); 
            $$->id_type = DATA_INT; 
            code_gen_binary_op("cmp_LT", $$, $1, $3); 
        }
    | expressao4 '>' expressao3
        {
            $$ = ast_binary_op(NODE_EXPR, ">", $1, $3);
            $$->id_type = DATA_INT; 
            code_gen_binary_op("cmp_GT", $$, $1, $3);
        }
    | expressao4 TK_OC_LE expressao3 
        {
            $$ = ast_binary_op(NODE_EXPR, "<=", $1, $3); 
            $$->id_type = DATA_INT;
            code_gen_binary_op("cmp_LE", $$, $1, $3);
        }
    | expressao4 TK_OC_GE expressao3 
        {
            $$ = ast_binary_op(NODE_EXPR, ">=", $1, $3); 
            $$->id_type = DATA_INT; 
            code_gen_binary_op("cmp_GE", $$, $1, $3);
        }
    ;

expressao3
    : expressao2 { $$ = $1; }
    | expressao3 '+' expressao2  
        { 
            $$ = ast_binary_op(NODE_EXPR, "+", $1, $3); 
            $$->id_type = data_type_infer($1->id_type, $3->id_type);
            code_gen_binary_op("add", $$, $1, $3);
        }
    | expressao3 '-' expressao2 
        {
            $$ = ast_binary_op(NODE_EXPR, "-", $1, $3);
            $$->id_type = data_type_infer($1->id_type, $3->id_type);
            code_gen_binary_op("sub", $$, $1, $3);
        }

expressao2
    : expressao1 { $$ = $1; }
    | expressao2 '*' expressao1  
        { 
            $$ = ast_binary_op(NODE_EXPR, "*", $1, $3);
            $$->id_type = data_type_infer($1->id_type, $3->id_type);
            code_gen_binary_op("mult", $$, $1, $3);
        }
    | expressao2 '/' expressao1 
        { 
            $$ = ast_binary_op(NODE_EXPR, "/", $1, $3);
            $$->id_type = data_type_infer($1->id_type, $3->id_type);
            code_gen_binary_op("div", $$, $1, $3);
        }
    | expressao2 '%' expressao1  
        { 
            $$ = ast_binary_op(NODE_EXPR, "%", $1, $3);
            $$->id_type = data_type_infer($1->id_type, $3->id_type);
        }
    ;

expressao1
    : expressao0      { $$ = $1; }
    | '-' expressao1 
        {
            $$ = node_create(NODE_EXPR, "-"); 
            node_add_child($$, $2); 
            $$->id_type = $2->id_type;
            
            $$->location = new_temp();
            $$->code = code_concat($2->code, code_create("rsubI", $2->location, "0", $$->location)); 
        }
    | '!' expressao1 
        { 
            $$ = node_create(NODE_EXPR, "!"); 
            node_add_child($$, $2); 
            $$->id_type = $2->id_type;

            // ToDo: Code Generation
            $$->location = new_temp();
            char *temp0 = new_temp();

            $$->code = code_concat_many(
                $2->code,
                code_create("loadI", "0", NULL, temp0),
                code_create("cmp_EQ", $2->location, temp0, $$->location),
                NULL
            );
        }
    ;

expressao0
    : '(' expressao ')' { $$ = $2; }
    | chamadaFuncao { $$ = $1; }
    | terminal_lit_float { $$ = $1; }
    | terminal_lit_int 
        { 
            $$ = $1;

            $$->location = new_temp();
            $$->code = code_create("loadI", $$->label, NULL, $$->location);
        }
    | terminal_identificador 
        { 
            check_was_declared($1->lexical_value, scope_stack, SYMBOL_VARIABLE);
            check_correct_usage($1->lexical_value, scope_stack, SYMBOL_VARIABLE);
    
            $$ = $1;

            struct symbol_t *symbol = find_symbol(scope_stack, $1->label);
            $$->id_type = symbol->data_type;

            char buf[100];
            snprintf(buf, sizeof(buf), "%d", symbol->offset);

            $$->location = new_temp();
            $$->code = code_create("loadAI", "rfp", buf, $$->location);
        }
    ;

terminal_identificador
    : TK_IDENTIFICADOR  
        { 
            $$ = node_create(NODE_IDENTIFIER, $1->value); 
            $$->lexical_value = $1;
        }
    ;

terminal_lit_float
    : TK_LIT_FLOAT 
        { 
            $$ = node_create(NODE_FLOAT_LITERAL, $1->value); 
            $$->lexical_value = $1;
            $$->id_type = DATA_FLOAT;
        }
    ;

terminal_lit_int
    : TK_LIT_INT  
        { 
            $$ = node_create(NODE_INT_LITERAL, $1->value);
            $$->lexical_value = $1;
            $$->id_type = DATA_INT;
        }
    ;

%%
void yyerror(char const *mensagem) {
    fprintf(stderr, "Error at line %d: %s\n", get_line_number(), mensagem);
}
