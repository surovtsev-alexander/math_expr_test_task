#pragma once

#include "ast_node.h"
#include "token.h"

typedef struct ast_display_node_e
{
  token_t token;
  int level;
} ast_display_node_t;

void print_tree(ast_node_t *tree_root);

