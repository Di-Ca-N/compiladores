#include "code_gen.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_TEMP_LEN 100
#define MAX_LABEL_LEN 100

struct iloc_t {
    char* label;
    char* mnemonic;
    char* arg1;
    char* arg2;
    char* arg3;
};

struct code_t {
    struct iloc_t instruction;
    struct code_t *next;
};


code_t *code_create(char* mnemonic, char* arg1, char* arg2, char* arg3) {
    code_t *code = (code_t *) malloc(sizeof(code_t));

    code->instruction.mnemonic = mnemonic;
    if (arg1 != NULL) code->instruction.arg1 = strdup(arg1);
    if (arg2 != NULL) code->instruction.arg2 = strdup(arg2);
    if (arg3 != NULL) code->instruction.arg3 = strdup(arg3);
    code->next = NULL;
    return code;
}

code_t* set_label(code_t* code, char* label) {
    code->instruction.label = label;
    return code;
}

// Create a new label
char* new_label() {
    static int current_label = 0;
    char label[MAX_LABEL_LEN];
    snprintf(label, MAX_LABEL_LEN, "L%d", current_label++);
    return strdup(label);
}

// Create a new temporary
char* new_temp() {
    static int current_temp = 0;
    char temp[MAX_TEMP_LEN];
    snprintf(temp, MAX_TEMP_LEN, "r%d", current_temp++);
    return strdup(temp);
}

// Print the code in the required format
void code_print(code_t *code) {
    if (code == NULL) return;

    struct iloc_t instr = code->instruction;
    printf(
        "%2s%1s %-6s %3s%1s %3s => %2s\n", 
        instr.label != NULL ? instr.label : "", 
        instr.label != NULL ? ":" : "",
        instr.mnemonic, 
        instr.arg1 != NULL ? instr.arg1 : "",
        instr.arg2 != NULL ? "," : "",
        instr.arg2 != NULL ? instr.arg2 : "", 
        instr.arg3 != NULL ? instr.arg3 : ""
    );
    code_print(code->next);
}

// Concatenate the given codes, placing the second after the first.
code_t *code_concat(code_t *first, code_t *second) {
    code_t *p = first;

    while (p->next != NULL) {
        p = p->next;
    }

    p->next = second;
    return first;
}
