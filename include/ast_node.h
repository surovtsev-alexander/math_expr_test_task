#pragma once

#include "token.h"

typedef struct ast_node_s
{
  token_t            token;
  struct ast_node_s *left;
  struct ast_node_s *right;
} ast_node_t;

