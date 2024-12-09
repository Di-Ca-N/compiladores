#pragma once

typedef struct code_t code_t;

struct code_attrs_t {
    code_t *code;
    char* location;
};

// Create code containing the given instruction
code_t *code_create(char* mnemonic, char* arg1, char* arg2, char* arg3);
code_t *set_label(code_t *code, char* label);

// Create a new label
char* new_label();

// Create a new temporary
char* new_temp();

// Print the code in the required format
void code_print(code_t *code);

// Concatenate the given codes, placing the second after the first.
code_t *code_concat(code_t *first, code_t *second);
