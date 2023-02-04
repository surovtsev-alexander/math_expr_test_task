#include "ast.h"
#include "custom_queue_helpers.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/queue.h>

ast_node_t *tree_head = NULL;

static ret_code_t delete_tree(void);
static ret_code_t check_x_and_eaqual_signs_in_queue(const custom_queue_t *queue);

static bool  _result_is_defined = false;
static float _result;

ret_code_t abstract_syntax_tree_init(void)
{
  ret_code_t ret_code;
  ret_code = delete_tree();

  _result_is_defined = false;

  return ret_code;
}

ret_code_t abstract_syntax_tree_create(const custom_queue_t *queue)
{
  ret_code_t ret_code = RET_CODE_OK;

  _result_is_defined = false;
  ret_code = check_x_and_eaqual_signs_in_queue(queue);

  return ret_code;
}

ret_code_t abstract_syntax_tree_evaluate_x(float *result)
{
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

static ret_code_t delete_tree(void)
{
  custom_queue_t        queue = TAILQ_HEAD_INITIALIZER(queue);
  custom_queue_entry_t *entry;
  ast_node_t           *node;
  ret_code_t            ret_code;

  if (NULL == tree_head)
  {
    return RET_CODE_OK;
  }

  custom_queue_empty(&queue);

  ret_code = custom_queue_helpers_insert_ast_node(
      &queue,
      tree_head,
      TAIL_OR_LAST);

  if (RET_CODE_OK != ret_code)
  {
    return ret_code;
  }

  free(tree_head);
  tree_head = NULL;

  while (!TAILQ_EMPTY(&queue))
  {
    entry = TAILQ_FIRST(&queue);
    TAILQ_REMOVE(&queue, entry, entries);

    node = entry->data;

    if (NULL != node->left)
    {
      ret_code = custom_queue_helpers_insert_ast_node(
          &queue,
          node->left,
          TAIL_OR_LAST);

      if (RET_CODE_OK != ret_code)
      {
        break;
      }
    }

    if (NULL != node->right)
    {
      ret_code = custom_queue_helpers_insert_ast_node(
          &queue,
          node->right,
          TAIL_OR_LAST);
      if (RET_CODE_OK != ret_code)
      {
        break;
      }
    }

    free(node);
    free(entry);
  }

  return ret_code;
}


static ret_code_t check_x_and_eaqual_signs_in_queue(const custom_queue_t *queue)
{
  int                   x_signs_count = 0;
  int                   equal_signs_count = 0;
  custom_queue_entry_t *entry;
  token_t              *token;
  token_id_t            token_id;

  TAILQ_FOREACH(entry, queue, entries)
  {
    token = entry->data;
    token_id = token->token_id;

    if (TOKEN_ID_X == token_id)
    {
      x_signs_count++;
    }
    else if (TOKEN_ID_EQUAL == token_id)
    {
      equal_signs_count++;
    }
  }

  printf("x_count: %d\n=_count: %d\n",
      x_signs_count,
      equal_signs_count);

  if (
      x_signs_count == 1 &&
      equal_signs_count == 1)
  {
    return RET_CODE_OK;
  }

  if (x_signs_count == 0)
  {
    return RET_CODE_NO_X_SIGN;
  }

  if (x_signs_count > 1)
  {
    return RET_CODE_MORE_THAN_ONE_X_SIGN;
  }

  if (equal_signs_count == 0)
  {
    return RET_CODE_NO_EQUAL_SING;
  }

  if (equal_signs_count > 1)
  {
    return RET_CODE_MORE_TAN_ONE_EQUAL_SING;
  }


  return RET_CODE_OK;
}

