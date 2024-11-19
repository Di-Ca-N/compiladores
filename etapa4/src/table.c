#include "table.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct symbol_t *create_symbol(char *label, int line_number, symbol_type type, symbol_data_type data_type) {
    struct symbol_t *symbol = (struct symbol_t *) malloc(sizeof(struct symbol_t));
    symbol->label = strdup(label);
    symbol->line_number = line_number;
    symbol->type = type;
    symbol->data_type = data_type;
    return symbol;
}

struct symbol_table_t *enter_scope(struct symbol_table_t *table) {
    struct symbol_table_t *top = (struct symbol_table_t*) malloc(sizeof(struct symbol_table_t));
    top->num_entries = 0;
    top->next = table;
    return top;
}

struct symbol_table_t *exit_scope(struct symbol_table_t *table) {
    if (table == NULL) return NULL;

    struct symbol_table_t *aux = table->next;
    for (int i = 0; i < table->num_entries; i++) {
        free(table->entries[i]);
    }
    free(table->entries);
    free(table);

    return aux;
}

int declare_symbol(struct symbol_table_t *table, struct symbol_t *symbol) {
    for (int i = 0; i < table->num_entries; i++) {
        if (strcmp(table->entries[i]->label, symbol->label) == 0)
            return -1;
    }
    table->entries = realloc(table->entries, (table->num_entries + 1) * sizeof(struct symbol_t *));
    table->entries[table->num_entries] = symbol;
    table->num_entries++;

    return 0;
}

struct symbol_t *find_symbol(struct symbol_table_t *table, char *key) {
    if (table == NULL) return NULL;
    for (int i = 0; i < table->num_entries; i++) {
        if (strcmp(table->entries[i]->label, key) == 0)
            return table->entries[i];
    }
    return find_symbol(table->next, key);
}

void print_table(struct symbol_table_t *table) {
    if (table == NULL) return;

    printf("---------------------------------------\n");
    printf("Key\tLine #\tType\tData Type\n");

    for (int i = 0; i < table->num_entries; i++) {
        struct symbol_t *symbol = table->entries[i];

        char *type_str = (symbol->type == SYMBOL_VARIABLE) ? "var" : "func";
        char *data_type_str = (symbol->data_type == DATA_INT) ? "int" : "float";

        printf("%s\t%d\t%s\t%s\n", symbol->label, symbol->line_number, type_str, data_type_str);
    }

    printf("---------------------------------------\n");
    print_table(table->next);
}
