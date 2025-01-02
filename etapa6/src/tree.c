/*
Grupo:
- Diego Cardoso Nunes (00333984)
- Geancarlo Kozenieski (00264414)
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tree.h"

extern int get_line_number();
struct node_t *node_create(node_type_t type, char *label) {
    struct node_t *node = malloc(sizeof(struct node_t));
    node->type = type;
    node->id_type = DATA_UNDEFINED;
    node->label = strdup(label);
    node->lexical_value = NULL;
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
    printf("'%s' (%d) type=%s\n", node->label, node->type, type_to_str(node->id_type));
    for (int i = 0; i < node->nChildren; i++) {
        node_print(node->children[i], level + 1);
    }
    node_print(node->next, level + 1);
}

struct node_t *node_append(struct node_t *first, struct node_t *new) {
    if (new == NULL) {
        return first;
    }

    if (first == NULL && new != NULL) {
        return new;
    }

    struct node_t *p = first;

    while (p->next != NULL) {
        p = p->next;
    }
    p->next = new;
    return first;
}

void node_free(struct node_t *node) {
    if (node == NULL) {
        return;
    }

    if (node->lexical_value) {
        lexical_value_free(node->lexical_value);
    }

    for (int i = 0; i < node->nChildren; i++) {
        node_free(node->children[i]);
    }
    node_free(node->next);

    free(node->label);
    free(node->children);
    free(node);
}

void exporta(void *arvore) {
    struct node_t *ast = (struct node_t *) arvore;

    if (ast == NULL) return;

    printf("%p [label=\"%s\"];\n", ast, ast->label);

    for (int i = 0; i < ast->nChildren; i++) {
        printf("%p, %p\n", ast, ast->children[i]);
        exporta(ast->children[i]);
    }
    if (ast->next != NULL) {
        printf("%p, %p\n", ast, ast->next);
        exporta(ast->next);
    }
}

struct lexical_value_t *lexical_value_create(lexical_type_t type, char* label) {
    struct lexical_value_t *lex_val = malloc(sizeof(struct lexical_value_t));
    lex_val->lineNumber=get_line_number();
    lex_val->type = type;
    lex_val->value = strdup(label);
    return lex_val;
}

void lexical_value_free(struct lexical_value_t *lex_val) {
    free(lex_val->value);
    free(lex_val);
}