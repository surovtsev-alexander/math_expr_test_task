#include "ast.h"
#include "ast_creator.h"
#include "ast_eraser.h"
#include "ast_evaluator.h"
#include "ast_folder.h"
#include "custom_queue_helpers.h"

#include <stdio.h>

ast_node_t *tree_head = NULL;

static bool  _result_is_defined = false;
static float _result;

ret_code_t abstract_syntax_tree_init(void)
{
  ret_code_t ret_code;
  ret_code = ast_eraser_erase(tree_head);
  tree_head = NULL;

  _result_is_defined = false;

  return ret_code;
}

ret_code_t abstract_syntax_tree_create(const custom_queue_t *queue)
{
  ret_code_t ret_code;

  ret_code = ast_creator_create(
      queue,
      &tree_head);

  if (RET_CODE_OK == ret_code)
  {
    ast_helpers_print_tree(tree_head);

    ret_code = ast_folder_fold(tree_head);
  }

  if (RET_CODE_OK == ret_code)
  {
    ast_helpers_print_tree(tree_head);
  }

  return ret_code;
}

ret_code_t abstract_syntax_tree_evaluate_x(float *result)
{
  ret_code_t ret_code = ast_evaluator_evaluate(tree_head);

  puts("after_evaluation");
  ast_helpers_print_tree(tree_head);

  if (RET_CODE_OK == ret_code)
  {
    _result_is_defined = true;
    _result = tree_head->right->token.number;
  }

  if (_result_is_defined)
  {
    result[0] = _result;
    return RET_CODE_OK;
  }
  else
  {
    return RET_CODE_RESULT_IS_UNDEFINED;
  }
}

