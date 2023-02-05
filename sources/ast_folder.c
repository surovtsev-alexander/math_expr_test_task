#include "ast_folder.h"
#include "custom_queue.h"
#include "custom_queue_helpers.h"


ret_code_t ast_folder_fold(ast_node_t **root)
{
  ret_code_t            ret_code;
  custom_queue_t        stack = TAILQ_HEAD_INITIALIZER(stack);
  custom_queue_entry_t *curr_entry;

  ret_code = RET_CODE_OK;

  custom_queue_empty(&stack);

  ret_code = custom_queue_helpers_insert_ast_node(
      &stack,
      *root,
      HEAD_OR_FIRST);

  while (NULL != (curr_entry = TAILQ_FIRST(&stack)))
  {

  }

  custom_queue_empty(&stack);

  return ret_code;
}

