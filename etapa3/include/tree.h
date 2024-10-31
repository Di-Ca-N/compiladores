enum node_type_t {
    NODE_FUNC_LIST,
    NODE_CMD_LIST,
    NODE_EXPR_LIST,
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
};

struct node_t {
    enum node_type_t type;
    int nChildren;
    char *label;
    struct node_t **children;
};

struct node_t *node_create(enum node_type_t type, char *label);
void node_add_child(struct node_t *parent, struct node_t *child);
void node_print(struct node_t *node, int level);
void free_node(struct node_t *node);