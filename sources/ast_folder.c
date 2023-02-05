#include "ast_folder.h"
#include "custom_queue.h"
#include "custom_queue_helpers.h"


#include <stdlib.h>


ret_code_t ast_folder_fold(ast_node_t **root)
{
  ret_code_t            ret_code;
  custom_queue_t        stack = TAILQ_HEAD_INITIALIZER(stack);
  custom_queue_entry_t *curr_entry;

  ret_code = RET_CODE_OK;

  custom_queue_empty(&stack);

  if (NULL == root[0])
  {
    return RET_CODE_UNINITIALIZED;
  }

  ret_code = custom_queue_helpers_insert_ast_node(
      &stack,
      *root,
      HEAD_OR_FIRST);
  if (RET_CODE_OK != ret_code)
  {
    return ret_code;
  }

  free(*root);

  while (!TAILQ_EMPTY(&stack))
  {
    curr_entry = TAILQ_FIRST(&stack);
    TAILQ_REMOVE(&stack, curr_entry, entries);


  }

  custom_queue_empty(&stack);

  return ret_code;
}

