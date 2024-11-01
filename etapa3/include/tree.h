#pragma once

typedef enum {
    NODE_FUNC,
    NODE_CMD,
    NODE_EXPR,
    NODE_IF,
    NODE_WHILE,
    NODE_RETURN,
    NODE_ASSIGN,
    NODE_VAR_INIT,
    NODE_FUNC_CALL,
    NODE_IDENTIFIER,
    NODE_INT_LITERAL,
    NODE_FLOAT_LITERAL,
} node_type_t;

typedef enum {
    TYPE_INT,
    TYPE_FLOAT,
} lexical_type_t;

struct lexical_value_t {
    int lineNumber;
    lexical_type_t type;
    char *value;
};

struct node_t {
    node_type_t type;
    int nChildren;
    char *label;
    struct node_t **children;
};

struct node_t *node_create(node_type_t type, char *label);
void node_add_child(struct node_t *parent, struct node_t *child);
void node_print(struct node_t *node, int level);
void free_node(struct node_t *node);
