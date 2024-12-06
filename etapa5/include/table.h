/*
Grupo:
- Diego Cardoso Nunes (00333984)
- Geancarlo Kozenieski (00264414)
*/

#pragma once

#include "data_types.h"

typedef enum  {
    SYMBOL_VARIABLE,
    SYMBOL_FUNCTION,
} symbol_type;


struct symbol_t {
    char* label;
    int line_number;
    symbol_type type;
    data_type_t data_type;
    int size;
    int offset;
};

struct symbol_table_t {
    int base_offset;
    int num_entries;
    struct symbol_t **entries;
    struct  symbol_table_t *next;
};

// Create a new symbol
struct symbol_t *create_symbol(char* label, int line_number, symbol_type type, data_type_t data_type);

// Create a new symbol table and pushes it into the stack. Return the new table at the top of the stack.
struct symbol_table_t *enter_scope(struct symbol_table_t *table);

// Pop the top symbol table and frees itself and its symbols. Return the new table at the top of the stack.
struct symbol_table_t *exit_scope(struct symbol_table_t *table);

// Declare a new symbol in the top scope. Return 0 on success and -1 on error.
// This function may fail if the symbol is arelady declared in the current scope.
int declare_symbol(struct symbol_table_t *table, struct symbol_t *symbol);

// Lookup a symbol in the entire scope stack. Return NULL on fail.
struct symbol_t *find_symbol(struct symbol_table_t *table, char* key);

// Lookup a symbol only on the top scope. Return NULL on fail.
struct symbol_t *find_symbol_on_scope(struct symbol_table_t *table, char* key);

// Prints the table for debug purposes
void print_table(struct symbol_table_t *table);
