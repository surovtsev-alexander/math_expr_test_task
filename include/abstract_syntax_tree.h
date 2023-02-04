#pragma once

#include "custom_queue.h"

ret_code_t abstract_syntax_tree_init(void);
ret_code_t abstract_syntax_tree_create(const custom_queue_t *queue);
ret_code_t abstract_syntax_tree_evaluate_x(float *result);

