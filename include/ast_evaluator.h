#pragma once

#include "ret_code.h"
#include "ast_node.h"

ret_code_t ast_evaluator_evaluate(ast_node_t *root);

