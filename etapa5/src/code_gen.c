/*
Grupo:
- Diego Cardoso Nunes (00333984)
- Geancarlo Kozenieski (00264414)
*/

#include "code_gen.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

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

code_t *code_create_with_label(char *label, char* mnemonic, char* arg1, char* arg2, char* arg3) {
    code_t *code = code_create(mnemonic, arg1, arg2, arg3);
    set_label(code, label);
    return code;
}

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
    if (strcmp(instr.mnemonic, "jumpI") == 0 || 
        strcmp(instr.mnemonic, "cbr") == 0) {
        printf(
            "%2s%1s %-7s %3s -> %3s%1s %3s\n",
            instr.label != NULL ? instr.label : "",
            instr.label != NULL ? ":" : "",
            instr.mnemonic,
            instr.arg1 != NULL ? instr.arg1 : "",
            instr.arg2 != NULL ? instr.arg2 : "",
            instr.arg3 != NULL ? "," : "",
            instr.arg3 != NULL ? instr.arg3 : ""
        );
    } else if (strcmp(instr.mnemonic, "nop") == 0) {
        printf(
            "%2s%1s %-7s\n",
            instr.label != NULL ? instr.label : "", 
            instr.label != NULL ? ":" : "",
            instr.mnemonic
        );
    } else if (strcmp(instr.mnemonic, "storeAI") == 0) {
        printf(
            "%2s%1s %-7s %3s => %3s%1s %3s\n", 
            instr.label != NULL ? instr.label : "", 
            instr.label != NULL ? ":" : "",
            instr.mnemonic, 
            instr.arg1 != NULL ? instr.arg1 : "",
            instr.arg2 != NULL ? instr.arg2 : "", 
            instr.arg2 != NULL ? "," : "",
            instr.arg3 != NULL ? instr.arg3 : ""
        );
    } else {
        char *arrow = strncmp(instr.mnemonic, "cmp", 3) == 0 ? "->" : "=>";
        printf(
            "%2s%1s %-7s %3s%1s %3s %s %3s\n", 
            instr.label != NULL ? instr.label : "", 
            instr.label != NULL ? ":" : "",
            instr.mnemonic, 
            instr.arg1 != NULL ? instr.arg1 : "",
            instr.arg2 != NULL ? "," : "",
            instr.arg2 != NULL ? instr.arg2 : "", 
            arrow,
            instr.arg3 != NULL ? instr.arg3 : ""
        );
    }

    
    code_print(code->next);
}

// Concatenate the given codes, placing the second after the first.
code_t *code_concat(code_t *first, code_t *second) {
    if (second == NULL)
        return first;
    
    if (first == NULL && second != NULL)
        return second;

    code_t *p = first;

    while (p->next != NULL) {
        p = p->next;
    }

    p->next = second;
    return first;
}


code_t *code_concat_many(code_t *first, ...) {
    va_list ptr;

    va_start(ptr, first);

    for (code_t *next_code = va_arg(ptr, code_t*); next_code != NULL; next_code = va_arg(ptr, code_t*)) {
        first = code_concat(first, next_code);
    }
    va_end(ptr);
    
    return first;
}
