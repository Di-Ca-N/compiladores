/*
Grupo:
- Diego Cardoso Nunes (00333984)
- Geancarlo Kozenieski (00264414)
*/

#pragma once

#include "data_types.h"
#include "table.h"
#include "tree.h"

// Check for a symbol redeclaration. If it occurs, print an error message and 
// force the program to exit with ERR_DECLARED
void check_no_redeclaration(struct lexical_value_t *lex_val,
                            struct symbol_table_t *scope, symbol_type type,
                            data_type_t data_type);

// Check if a used symbol was declared. If it was not declared, print an error 
// message and force the program to exit with ERR_UNDECLARED
void check_was_declared(struct lexical_value_t *lex_val,
                        struct symbol_table_t *scope, symbol_type type);

// Check if a declared symbol was used correctly as a variable or as a function.
// If the usage was incorrect, print an error message and force the program to 
// exit with ERR_FUNCTION or ERR_VARIABLE, depending on the nature of the declared symbol
void check_correct_usage(struct lexical_value_t *lex_val,
                         struct symbol_table_t *scope,
                         symbol_type expected_type);