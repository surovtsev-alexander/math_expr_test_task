#include "ast_creator.h"
#include "ast_eraser.h"
#include "custom_queue_helpers.h"

#include <stdlib.h>
#include <string.h>



static const token_t equal_sign_token =
{
  .token_id = TOKEN_ID_EQUAL,
};


static ast_node_t* create_node_by_range(
    const custom_queue_t *queue,
    int start_idx,
    int finish_idx);
static ret_code_t check_x_and_eaqual_signs_in_queue(const custom_queue_t *queue);
static int get_token_id_first_position(
    const custom_queue_t *queue,
    token_id_t token_id);


ret_code_t ast_creator_create(
    const custom_queue_t *queue,
    ast_node_t **root)
{
  ret_code_t ret_code;
  int equal_token_position;
  int x_token_position;
  ast_node_t *a;
  ast_node_t *b;
  ast_node_t *res;

  root[0] = NULL;

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

  res = malloc(sizeof(ast_node_t));
  if (NULL == res)
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
      ast_eraser_erase(a);
    }
    if (NULL == b)
    {
      ast_eraser_erase(b);
    }
    ast_eraser_erase(res);
    res = NULL;
    ret_code = RET_CODE_NO_MEMORY;
  }

  if (RET_CODE_OK == ret_code)
  {
    memcpy(&(res->token), &equal_sign_token, sizeof(token_t));
    if (x_token_position < equal_token_position)
    {
      res->left  = a;
      res->right = b;
    }
    else
    {
      res->left  = b;
      res->right = a;
    }
  }

  root[0] = res;

  return ret_code;
}

static ast_node_t* create_node_by_range(
    const custom_queue_t *queue,
    int start_idx,
    int finish_idx)
{
  ret_code_t            ret_code = RET_CODE_OK;
  custom_queue_t        stack = TAILQ_HEAD_INITIALIZER(stack);
  custom_queue_entry_t *curr_entry;
  int                   curr_idx;
  bool                  start_reached;
  bool                  finish_not_reached;
  token_t              *token;
  token_id_t            token_id;
  ast_node_t           *node;
  ast_node_t           *res;
  bool                  store_node;

  custom_queue_empty(&stack);

  //printf("create_node_by_range: %d %d\n", start_idx, finish_idx);

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

    if (start_reached)
    {
      node = malloc(sizeof(ast_node_t));
      if (NULL == node)
      {
        ret_code = RET_CODE_NO_MEMORY;
        break;
      }

      token = curr_entry->data;

      //printf("queue token:");
      //token_print(token);
      //printf("\n");

      memcpy(&(node->token), token, sizeof(token_t));
      node->left = NULL;
      node->right = NULL;

      store_node = false;
      token_id = token->token_id;
      if (token_id_is_number_or_x(token_id))
      {
        store_node = true;
      }
      else if (token_id_is_operation(token_id))
      {
        ret_code = custom_queue_pop(
            &stack,
            (void **)&(node->left),
            TAIL_OR_LAST);
        if (RET_CODE_OK == ret_code)
        {
          ret_code = custom_queue_pop(
              &stack,
              (void **)&(node->right),
              TAIL_OR_LAST);
        }
        store_node = true;
      }
      else if (TOKEN_ID_CHANGE_SING == token_id)
      {
        ret_code = custom_queue_pop(
            &stack,
            (void **)&(node->left),
            TAIL_OR_LAST);
        store_node = true;
      }

      if (RET_CODE_OK == ret_code && store_node)
      {
        //ast_helpers_print_tree(node);
        ret_code = custom_queue_helpers_insert_ast_node(
            &stack,
            node,
            TAIL_OR_LAST);
      }
    }

    if (RET_CODE_OK != ret_code)
    {
      break;
    }

    curr_idx++;
    curr_entry = TAILQ_NEXT(curr_entry, entries);
  }

  if (RET_CODE_OK == ret_code && !TAILQ_EMPTY(&stack))
  {
    ret_code = custom_queue_pop(
        &stack,
        (void **)&res,
        TAIL_OR_LAST);
  }

  if (RET_CODE_OK == ret_code)
  {
    if (!TAILQ_EMPTY(&stack))
    {
      ret_code = RET_CODE_INTERNAL_ERROR_001;
    }

    if (RET_CODE_OK != ret_code)
    {
      ast_eraser_erase(res);
      res = NULL;
    }
  }

  if (RET_CODE_OK != ret_code)
  {
    while (!TAILQ_EMPTY(&stack))
    {
      if (RET_CODE_OK != custom_queue_pop(
            &stack,
            (void **)&node,
            TAIL_OR_LAST))
      {
        break;
      }
      ast_eraser_erase(node);
    }
  }

  custom_queue_empty(&stack);

  return res;
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

  /*
  printf("x_count: %d\n=_count: %d\n",
      x_signs_count,
      equal_signs_count);
  */

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

