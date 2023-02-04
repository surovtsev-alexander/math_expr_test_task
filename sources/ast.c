#include "ast.h"
#include "custom_queue_helpers.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/queue.h>

ast_node_t *tree_head = NULL;

static ret_code_t delete_tree(ast_node_t *root_node);
static ret_code_t check_x_and_eaqual_signs_in_queue(const custom_queue_t *queue);
static int get_token_id_first_position(
    const custom_queue_t *queue,
    token_id_t token_id);

static bool  _result_is_defined = false;
static float _result;

ret_code_t abstract_syntax_tree_init(void)
{
  ret_code_t ret_code;
  ret_code = delete_tree(tree_head);
  tree_head = NULL;

  _result_is_defined = false;

  return ret_code;
}

static ast_node_t* create_node_by_range(
    const custom_queue_t *queue,
    int start_idx,
    int finish_idx)
{
  ret_code_t ret_code = RET_CODE_OK;
  custom_queue_t stack = TAILQ_HEAD_INITIALIZER(stack);
  custom_queue_entry_t *curr_entry;
  ast_node_t *res = NULL;
  int curr_idx;
  bool start_reached;
  bool finish_not_reached;
  token_t *token;

  custom_queue_empty(&stack);

  curr_idx = 0;
  curr_entry = TAILQ_FIRST(queue);
  while (NULL != curr_entry)
  {
    start_reached      = curr_idx >= start_idx;
    finish_not_reached = curr_idx < finish_idx || 0 > finish_idx;

    if (!finish_not_reached)
    {
      break;
    }

    if (start_reached && finish_not_reached)
    {
      token = curr_entry->data;

      if (token_id_is_number_or_x(token->token_id))
      {
        custom_queue_helper_insert_token(
            &stack,
            token,
            TAIL_OR_LAST);
      }
      else if (token_id_is_operation(token->token_id))
      {

      }
    }

    curr_idx++;
    curr_entry = TAILQ_NEXT(curr_entry, entries);
  }

  if (RET_CODE_OK != ret_code)
  {
    delete_tree(res);
  }

  custom_queue_empty(&stack);

  return res;
}

static const token_t equal_sign_token =
{
  .token_id = TOKEN_ID_EQUAL,
};

ret_code_t abstract_syntax_tree_create(const custom_queue_t *queue)
{
  ret_code_t ret_code;
  int equal_token_position;
  int x_token_position;
  ast_node_t *a;
  ast_node_t *b;

  ret_code = check_x_and_eaqual_signs_in_queue(queue);

  if (RET_CODE_OK != ret_code)
  {
    return ret_code;
  }

  equal_token_position = get_token_id_first_position(
      queue,
      TOKEN_ID_EQUAL);
  x_token_position = get_token_id_first_position(
      queue,
      TOKEN_ID_X);

  tree_head = malloc(sizeof(ast_node_t));
  if (NULL == tree_head)
  {
    return RET_CODE_NO_MEMORY;
  }

  a = create_node_by_range(
      queue,
      0,
      equal_token_position);

  b = create_node_by_range(
      queue,
      equal_token_position + 1,
      -1);

  if (NULL == a || NULL == b)
  {
    if (NULL == a)
    {
      delete_tree(a);
    }
    if (NULL == b)
    {
      delete_tree(b);
    }
    delete_tree(tree_head);
    ret_code = RET_CODE_NO_MEMORY;
  }

  if (RET_CODE_OK != ret_code)
  {
    memcpy(&(tree_head->token), &equal_sign_token, sizeof(token_t));
    if (x_token_position < equal_token_position)
    {
      tree_head->left  = a;
      tree_head->right = b;
    }
    else
    {
      tree_head->left  = b;
      tree_head->right = a;
    }
  }

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

static ret_code_t delete_tree(ast_node_t *root_node)
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

  ret_code = custom_queue_helpers_insert_ast_node(
      &queue,
      root_node,
      TAIL_OR_LAST);

  if (RET_CODE_OK != ret_code)
  {
    return ret_code;
  }

  free(root_node);

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

static int get_token_id_first_position(
    const custom_queue_t *queue,
    token_id_t token_id)
{
  int curr_idx;
  custom_queue_entry_t *entry;
  token_t              *token;

  curr_idx = 0;
  TAILQ_FOREACH(entry, queue, entries)
  {
    token = entry->data;
    if (token_id == token->token_id)
    {
      return curr_idx;
    }
    curr_idx++;
  }

  return -1;
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

