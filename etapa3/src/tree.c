#include <stdlib.h>
#include <stdio.h>
#include "tree.h"

struct node_t *node_create(node_type_t type, char *label) {
    struct node_t *node = malloc(sizeof(struct node_t));
    node->type = type;
    node->label = label;
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

void exporta (struct node_t *arvore) {
    node_print(arvore, 0);
}