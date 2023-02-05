#pragma once

#include "ret_code.h"
#include "ast_node.h"

#include <stdint.h>

typedef struct ast_folder_node_e
{
  const ast_node_t *ast_node;
  uint8_t     left_processed;
  uint8_t     right_processed;
} ast_folder_node_t;

ret_code_t ast_folder_fold(ast_node_t **root);
