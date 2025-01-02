/*
Grupo:
- Diego Cardoso Nunes (00333984)
- Geancarlo Kozenieski (00264414)
*/

#pragma once

#include "tree.h"
#include "code_gen.h"

struct node_t *ast_binary_op(node_type_t type, char* label, struct node_t *left, struct node_t *right);
void code_gen_binary_op(char* mnemonic, struct node_t *root, struct node_t *left, struct node_t *right);
code_t *safe_set_label(struct node_t *node, char *label);