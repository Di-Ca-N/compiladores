/*
Grupo:
- Diego Cardoso Nunes (00333984)
- Geancarlo Kozenieski (00264414)
*/

#include "table.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct symbol_t *create_symbol(char *label, int line_number, symbol_type type, data_type_t data_type) {
    struct symbol_t *symbol = (struct symbol_t *) malloc(sizeof(struct symbol_t));
    symbol->label = strdup(label);
    symbol->line_number = line_number;
    symbol->type = type;
    symbol->data_type = data_type;
    return symbol;
}

void free_symbol(struct symbol_t *symbol) {
    free(symbol->label);
    free(symbol);
}

int get_next_offset(struct symbol_table_t *table) {
    // As we do not have global variables, the offset of the global table can always be 0
    if (table->next == NULL) {
        return 0;
    }

    if (table->num_entries == 0) {
        return table->base_offset;
    }

    struct symbol_t *last_symbol = table->entries[table->num_entries - 1];
    return last_symbol->offset + last_symbol->size;
}

struct symbol_table_t *enter_scope(struct symbol_table_t *table) {
    struct symbol_table_t *top = (struct symbol_table_t*) malloc(sizeof(struct symbol_table_t));
    top->num_entries = 0;
    top->entries = NULL;
    top->next = table;

    if (table == NULL) {
        top->base_offset = 0;
    } else {
        top->base_offset = get_next_offset(table);
    }
    return top;
}

struct symbol_table_t *exit_scope(struct symbol_table_t *table) {
    if (table == NULL) return NULL;

    struct symbol_table_t *aux = table->next;
    for (int i = 0; i < table->num_entries; i++) {
        free_symbol(table->entries[i]);
    }

    if (table->entries != NULL) {
        free(table->entries);
    }

    free(table);

    return aux;
}

int declare_symbol(struct symbol_table_t *table, struct symbol_t *symbol) {
    for (int i = 0; i < table->num_entries; i++) {
        if (strcmp(table->entries[i]->label, symbol->label) == 0)
            return -1;
    }
    table->entries = realloc(table->entries, (table->num_entries + 1) * sizeof(struct symbol_t *));

    symbol->offset = get_next_offset(table);
    symbol->size = get_data_type_size(symbol->data_type);
    
    table->entries[table->num_entries] = symbol;
    table->num_entries++;

    return 0;
}

struct symbol_t *find_symbol_on_scope(struct symbol_table_t *table, char *key) {
    if (table == NULL) 
        return NULL;

    for (int i = 0; i < table->num_entries; i++) {
        if (strcmp(table->entries[i]->label, key) == 0)
            return table->entries[i];
    }

    return NULL;
}

struct symbol_t *find_symbol(struct symbol_table_t *table, char *key) {
    if (table == NULL) return NULL;

    struct symbol_t *symbol = find_symbol_on_scope(table, key);

    if (symbol != NULL) 
        return symbol;
    return find_symbol(table->next, key);
}

void print_table_inner(struct symbol_table_t *table, int depth) {
    if (table == NULL) return;

    printf("-- depth %02d -------------------------------\n", depth);
    printf("%-15s| %-6s | %-5s | %-9s | %-8s | %-5s\n", "Key", "Line #", "Type", "Data Type", "Offset", "Size");

    for (int i = 0; i < table->num_entries; i++) {
        struct symbol_t *symbol = table->entries[i];

        char *type_str = (symbol->type == SYMBOL_VARIABLE) ? "var" : "func";
        char *data_type_str = type_to_str(symbol->data_type);

        printf("%-15s| %-6d | %-5s | %-9s | %-8d | %-5d\n", symbol->label, symbol->line_number, type_str, data_type_str, symbol->offset, symbol->size);
    }

    printf("-------------------------------------------\n");
    print_table_inner(table->next, depth + 1);
}

void print_table(struct symbol_table_t *table) {
    printf("=============================\n");
    print_table_inner(table, 0);
    printf("=============================\n");
}

