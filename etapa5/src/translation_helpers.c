/*
Grupo:
- Diego Cardoso Nunes (00333984)
- Geancarlo Kozenieski (00264414)
*/

#include "translation_helpers.h"
#include <stdlib.h>

struct node_t *ast_binary_op(node_type_t type, char* label, struct node_t *left, struct node_t *right) {
    struct node_t *root = node_create(type, label); 
    node_add_child(root, left); 
    node_add_child(root, right);
    return root;  
}

void code_gen_binary_op(char* mnemonic, struct node_t *root, struct node_t *left, struct node_t *right) {
    root->location = new_temp();
    root->code = code_concat_many(
        left->code, 
        right->code,
        code_create(mnemonic, left->location, right->location, root->location),
        NULL
    );
}

code_t *safe_set_label(struct node_t *node, char *label) {
    code_t *code;
    if(node == NULL)
        code = code_create("nop", NULL, NULL, NULL);
    else
        code = node->code;
    set_label(code, label);
    return code;
}