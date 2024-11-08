#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tree.h"

extern int get_line_number();

struct node_t *node_create(node_type_t type, char *label) {
    struct node_t *node = malloc(sizeof(struct node_t));
    node->type = type;
    node->label = strdup(label);
    node->nChildren = 0;
    node->children = NULL;
    node->next = NULL;
    return node;
}

void node_add_child(struct node_t *parent, struct node_t *child) {
    if (parent == NULL || child == NULL) {
        return;
    }
    parent->children = realloc(parent->children, sizeof(struct node_t *) * (parent->nChildren + 1));
    parent->children[parent->nChildren] = child;
    parent->nChildren++;
}

void node_print(struct node_t *node, int level) {
    if (node == NULL) {
        return;
    }

    for (int i = 0; i < level; i++) {
        printf("  ");
    }
    printf("'%s' (%d)\n", node->label, node->type);
    for (int i = 0; i < node->nChildren; i++) {
        node_print(node->children[i], level + 1);
    }
    node_print(node->next, level + 1);
}

void node_append(struct node_t *first, struct node_t *new) {
    if (new == NULL) {
        return;
    }

    if (first == NULL && new != NULL) {
        first = new;
        return;
    }

    struct node_t *p = first;

    if (p == NULL) {
        printf("NULL\n");
    }

    while (p->next != NULL) {
        p = p->next;
    }
    p->next = new;
}

void node_free(struct node_t *node) {
    if (node == NULL) {
        return;
    }
    for (int i = 0; i < node->nChildren; i++) {
        node_free(node->children[i]);
    }
    node_free(node->next);

    free(node->label);
    free(node->children);
    free(node);
}

void exporta(void *a) {
    struct node_t *arvore = (struct node_t *) a;
    
    if (arvore == NULL) return;

    printf("%p [label=\"%s\"]\n", arvore, arvore->label);
    for (int i = 0; i < arvore->nChildren; i++) {
        printf("%p, %p\n", arvore, arvore->children[i]);
        exporta(arvore->children[i]);
    }
    if (arvore->next != NULL) {
        printf("%p, %p\n", arvore, arvore->next);
        exporta(arvore->next);
    }
}

struct lexical_value_t *lexical_value_create(lexical_type_t type, char* label) {
    struct lexical_value_t *lex_val = malloc(sizeof(struct lexical_value_t));
    lex_val->lineNumber = get_line_number();
    lex_val->type = type;
    lex_val->value = strdup(label);
    return lex_val;
}

void lexical_value_free(struct lexical_value_t *lex_val) {
    free(lex_val->value);
    free(lex_val);
}
