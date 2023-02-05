#pragma once

#include "ret_code.h"
#include "ast_node.h"
#include "custom_queue.h"


ret_code_t ast_creator_create(
    const custom_queue_t *queue,
    ast_node_t **root);

