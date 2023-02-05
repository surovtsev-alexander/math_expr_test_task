#include "ast.h"
#include "ast_helper_create.h"
#include "ast_helper_delete.h"
#include "ast_helper_evaluate_x.h"
#include "ast_helper_fold.h"
#include "common.h"
#include "custom_queue_helpers.h"

#include <stdio.h>

ast_node_t *tree_head = NULL;

ret_code_t abstract_syntax_tree_init(void)
{
  ret_code_t ret_code;
  ret_code = ast_helper_delete(tree_head);
  tree_head = NULL;

  return ret_code;
}

ret_code_t abstract_syntax_tree_create(const custom_queue_t *queue)
{
  ret_code_t ret_code;

  ret_code = ast_helper_create(
      queue,
      &tree_head);

  if (RET_CODE_OK == ret_code)
  {
#ifdef DEBUG_TRACE
    ast_helper_print(tree_head);
#endif // DEBUG_TRACE

    ret_code = ast_helper_fold(tree_head);
  }

#ifdef DEBUG_TRACE
  if (RET_CODE_OK == ret_code)
  {
    ast_helper_print(tree_head);
  }
#endif // DEBUG_TRACE

  return ret_code;
}

ret_code_t abstract_syntax_tree_evaluate_x(float *result)
{
  ret_code_t ret_code = ast_helper_evaluate_x(tree_head);

#ifdef DEBUG_TRACE
  puts("after_evaluation");
  ast_helper_print(tree_head);
#endif

  if (RET_CODE_OK == ret_code)
  {
    result[0] = tree_head->right->token.number;
  }

  return ret_code;
}

