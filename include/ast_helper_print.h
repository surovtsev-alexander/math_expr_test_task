#pragma once

#include "ast_node.h"
#include "token.h"

typedef struct ast_display_node_e
{
  ast_node_t *ast_node;
  int         level;
} ast_display_node_t;

void ast_helper_print(ast_node_t *tree_root);

