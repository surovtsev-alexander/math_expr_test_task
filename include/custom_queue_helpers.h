#pragma once

#include "ast_node.h"
#include "custom_queue.h"
#include "token.h"

ret_code_t custom_queue_helper_insert_token(
    custom_queue_t *queue,
    const token_t  *token,
    bool            head_not_tail);

ret_code_t custom_queue_helpers_insert_ast_node(
    custom_queue_t   *queue,
    const ast_node_t *ast_node,
    bool              head_not_tail);

