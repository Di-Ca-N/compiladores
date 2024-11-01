#include <stdlib.h>
#include <stdio.h>
#include "tree.h"

struct node_t *node_create(node_type_t type, char *label) {
    struct node_t *node = malloc(sizeof(struct node_t));
    node->type = type;
    node->label = label;
    node->nChildren = 0;
    node->children = NULL;
    return node;
}

void node_add_child(struct node_t *parent, struct node_t *child) {
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
}

void free_node(struct node_t *node) {
    if (node == NULL) {
        return;
    }
    for (int i = 0; i < node->nChildren; i++) {
        free_node(node->children[i]);
    }
    free(node->label);
    free(node->children);
    free(node);
}

void exporta (struct node_t *arvore) {
    node_print(arvore, 0);
}