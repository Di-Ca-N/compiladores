#pragma once

typedef enum  {
    SYMBOL_VARIABLE,
    SYMBOL_FUNCTION,
} symbol_type;

typedef enum  {
    DATA_INT,
    DATA_FLOAT,
} symbol_data_type;

struct symbol_t {
    char* label;
    int line_number;
    symbol_type type;
    symbol_data_type data_type;
};

struct symbol_table_t {
    int num_entries;
    struct symbol_t **entries;
    struct  symbol_table_t *next;
};

// Create a new symbol
struct symbol_t *create_symbol(char* label, int line_number, symbol_type type, symbol_data_type data_type);

// Create a new symbol table and pushes it into the stack. Returns the new table at the top of the stack.
struct symbol_table_t *enter_scope(struct symbol_table_t *table);

// Pop the top symbol table and frees itself and its symbols. Returns the new table at the top of the stack.
struct symbol_table_t *exit_scope(struct symbol_table_t *table);

// Declare a new symbol in the top scope. Returns 0 on success and -1 on error.
// This function may fail if the symbol is arelady declared in the current scope.
int declare_symbol(struct symbol_table_t *table, struct symbol_t *symbol);

// Find a symbol in the scope stack. Returns NULL on fail.
struct symbol_t *find_symbol(struct symbol_table_t *table, char* key);

// Prints the table for debug purposes
void print_table(struct symbol_table_t *table);
