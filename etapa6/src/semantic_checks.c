/*
Grupo:
- Diego Cardoso Nunes (00333984)
- Geancarlo Kozenieski (00264414)
*/

#include "semantic_checks.h"

#include <stdio.h>
#include <stdlib.h>

#include "errors.h"

void check_no_redeclaration(struct lexical_value_t *lex_val,
                            struct symbol_table_t *scope, symbol_type type,
                            data_type_t data_type) {
    struct symbol_t *symbol = find_symbol_on_scope(scope, lex_val->value);
    if (symbol != NULL) {
        printf(
            "Error: Redeclaration of %s '%s (%s)' on line %d: Found previous "
            "declaration '%s (%s)' on line %d.\n",
            symbol_type_str(type), lex_val->value, type_to_str(data_type),
            lex_val->lineNumber, symbol->label, type_to_str(symbol->data_type),
            symbol->line_number);
        exit(ERR_DECLARED);
    }
}

void check_was_declared(struct lexical_value_t *lex_val,
                        struct symbol_table_t *scope, symbol_type type) {
    struct symbol_t *symbol = find_symbol(scope, lex_val->value);
    if (symbol == NULL) {
        printf("Error: Usage of undeclared %s '%s' on line %d.\n",
               symbol_type_str(type), lex_val->value, lex_val->lineNumber);
        exit(ERR_UNDECLARED);
    }
}

void check_correct_usage(struct lexical_value_t *lex_val,
                         struct symbol_table_t *scope,
                         symbol_type expected_type) {
    struct symbol_t *symbol = find_symbol(scope, lex_val->value);

    if (symbol->type != expected_type) {
        printf("Error: %s %s declared on line %d used as %s on line %d.\n",
               symbol_type_str(symbol->type), symbol->label,
               symbol->line_number, symbol_type_str(expected_type),
               lex_val->lineNumber);

        if (symbol->type == SYMBOL_VARIABLE) {
            exit(ERR_VARIABLE);
        } else {
            exit(ERR_FUNCTION);
        }
    }
}