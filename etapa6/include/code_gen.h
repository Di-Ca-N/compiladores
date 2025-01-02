/*
Grupo:
- Diego Cardoso Nunes (00333984)
- Geancarlo Kozenieski (00264414)
*/

#pragma once

struct iloc_t {
    char* label;
    char* mnemonic;
    char* arg1;
    char* arg2;
    char* arg3;
};

typedef struct code_t {
    struct iloc_t instruction;
    struct code_t *next;
} code_t;

// Create code containing the given instruction
code_t *code_create(char* mnemonic, char* arg1, char* arg2, char* arg3);
code_t *code_create_with_label(char *label, char* mnemonic, char* arg1, char* arg2, char* arg3);
code_t *set_label(code_t *code, char* label);

// Create a new label
char* new_label();

// Create a new temporary
char* new_temp();

// Print the code in the required format
void code_print(code_t *code);

// Concatenate the given codes, placing the second after the first.
code_t *code_concat(code_t *first, code_t *second);

// Receive multiple code arguments and concatenate them in the given order.
// A NULL argument mark the end of the argument list and must be present.
code_t *code_concat_many(code_t *first, ...);
