#pragma once

#include "tokens_queue.h"

void abstract_syntax_tree_init(void);
ret_code_t abstract_syntax_tree_create(const tokens_queue_t *queue);
ret_code_t abstract_syntax_tree_evaluate_x(float *result);

