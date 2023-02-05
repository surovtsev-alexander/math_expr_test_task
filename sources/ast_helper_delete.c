#include "ast_helper_delete.h"
#include "custom_queue.h"
#include "custom_queue_helpers.h"

#include <stdlib.h>
#include <sys/queue.h>

ret_code_t ast_eraser_erase(ast_node_t *root_node)
{
  custom_queue_t        queue = TAILQ_HEAD_INITIALIZER(queue);
  custom_queue_entry_t *entry;
  ast_node_t           *node;
  ret_code_t            ret_code;

  if (NULL == root_node)
  {
    return RET_CODE_OK;
  }

  custom_queue_empty(&queue);

  ret_code = custom_queue_helper_insert_ast_node(
      &queue,
      root_node,
      TAIL_OR_LAST);

  if (RET_CODE_OK != ret_code)
  {
    return ret_code;
  }

  free(root_node);

  while (!TAILQ_EMPTY(&queue) && RET_CODE_OK == ret_code)
  {
    entry = TAILQ_FIRST(&queue);
    TAILQ_REMOVE(&queue, entry, entries);

    node = entry->data;

    if (NULL != node->left)
    {
      ret_code = custom_queue_helper_insert_ast_node(
          &queue,
          node->left,
          TAIL_OR_LAST);
    }

    if (RET_CODE_OK == ret_code && NULL != node->right)
    {
      ret_code = custom_queue_helper_insert_ast_node(
          &queue,
          node->right,
          TAIL_OR_LAST);
    }

    free(node);
    free(entry);
  }

  return ret_code;
}

