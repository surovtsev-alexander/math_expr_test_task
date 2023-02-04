#include "tokens_queue.h"
#include "common.h"

#include <assert.h>
#include <stdlib.h>


tokens_queue_t* tokens_queue_create(void)
{
  tokens_queue_t* res = malloc(sizeof(tokens_queue_t));

  if (NULL == res)
  {
    return NULL;
  }

  res->head = NULL;
  res->tail = NULL;

  return res;
}

ret_code_t tokens_queue_destroy_and_set_null(
    tokens_queue_t **pointer_to_queue)
{
  tokens_queue_t *queue;
  ret_code_t      ret_code;

  if (NULL == pointer_to_queue)
  {
    return RET_CODE_OK;
  }

  queue = pointer_to_queue[0];

  if (NULL == queue)
  {
    return RET_CODE_OK;
  }

  ret_code = tokens_queue_empty(queue);
  free(queue);
  pointer_to_queue[0] = NULL;

  return ret_code;
}

bool tokens_queue_is_empty(
    const tokens_queue_t * const queue)
{
  return NULL == queue || NULL == queue->head;
}

ret_code_t tokens_queue_empty(
    tokens_queue_t * const queue)
{
  ret_code_t ret_code = RET_CODE_OK;

  if (NULL == queue)
  {
    return RET_CODE_OK;
  }

  while (!tokens_queue_is_empty(queue))
  {
    ret_code = tokens_queue_pop_front(queue);

    if (RET_CODE_OK != ret_code)
    {
      break;
    }
  }

  return ret_code;
}

ret_code_t tokens_queue_push_front(
    tokens_queue_t * const queue,
    const token_t token)
{
  node_t *new_node = NULL;
  node_t *head;
  node_t *tail;

  if (NULL == queue)
  {
    return RET_CODE_UNINITIALIZED;
  }

  head = queue->head;
  tail = queue->tail;

  if (NULL == head)
  {
    if (NULL != tail)
    {
      return RET_CODE_INTERNAL_ERROR_001;
    }

    new_node = malloc(sizeof(node_t));
    if (NULL == new_node)
    {
      return RET_CODE_NO_MEMORY;
    }

    new_node->next = NULL;
    // queue->head is set after `if` block
    queue->tail = new_node;
  }
  else
  {
    if (NULL == tail)
    {
      return RET_CODE_INTERNAL_ERROR_002;
    }

    new_node = malloc(sizeof(node_t));
    if (NULL == new_node)
    {
      return RET_CODE_NO_MEMORY;
    }

    new_node->next = head;
  }

  queue->head = new_node;

  new_node->token = token;

  return RET_CODE_OK;
}

ret_code_t tokens_queue_push_back(
    tokens_queue_t * const queue,
    const token_t token)
{
  node_t *new_node = NULL;
  node_t *head;
  node_t *tail;

  if (NULL == queue)
  {
    return RET_CODE_UNINITIALIZED;
  }

  head = queue->head;
  tail = queue->tail;

  if (NULL == tail)
  {
    if (NULL != head)
    {
      return RET_CODE_INTERNAL_ERROR_003;
    }

    new_node = malloc(sizeof(node_t));
    if (NULL == new_node)
    {
      return RET_CODE_NO_MEMORY;
    }

    queue->head = new_node;
    queue->tail = new_node;
  }
  else
  {
    if (NULL == head)
    {
      return RET_CODE_INTERNAL_ERROR_004;
    }

    if (NULL != tail->next)
    {
      return RET_CODE_INTERNAL_ERROR_005;
    }

    new_node = malloc(sizeof(node_t));
    if (NULL == new_node)
    {
      return RET_CODE_NO_MEMORY;
    }

    tail->next = new_node;
  }

  new_node->token = token;
  new_node->next  = NULL;

  return RET_CODE_OK;
}

const token_t* tokens_queue_peek_front(
    tokens_queue_t * const queue)
{
  if (NULL == queue || NULL == queue->head)
  {
    return NULL;
  }

  return &(queue->head->token);
}


const token_t* tokens_queue_peek_back(
    tokens_queue_t * const queue)
{
  if (NULL == queue && NULL == queue->tail)
  {
    return NULL;
  }

  return &(queue->tail->token);
}

ret_code_t tokens_queue_pop_front(
    tokens_queue_t * const queue)
{
  node_t *next_node;
  node_t *head;
  node_t *tail;

  if (NULL == queue)
  {
    return RET_CODE_OK;
  }

  head = queue->head;
  tail = queue->tail;

  if (NULL == head)
  {
    if (NULL != tail)
    {
      return RET_CODE_INTERNAL_ERROR_006;
    }

    return RET_CODE_OK;
  }

  next_node = head->next;

  if (NULL == next_node)
  {
    if (NULL != tail)
    {
      return RET_CODE_INTERNAL_ERROR_007;
    }

    queue->tail = NULL;
  }

  free(head);

  queue->head = next_node;

  return RET_CODE_OK;
}

ret_code_t tokens_queue_pop_back(
    tokens_queue_t * const queue)
{
  node_t *head;
  node_t *tail;
  node_t *prev_node;
  node_t *curr_node;

  if (NULL == queue)
  {
    return RET_CODE_OK;
  }

  head = queue->head;
  tail = queue->tail;

  if (NULL == head || NULL == tail)
  {
    if (NULL != head || NULL != tail)
    {
      return RET_CODE_INTERNAL_ERROR_008;
    }

    return RET_CODE_OK;
  }

  if (NULL == head->next)
  {
    if (tail != head)
    {
      return RET_CODE_INTERNAL_ERROR_009;
    }

    free(head);

    queue->head = NULL;
    queue->tail = NULL;

    return RET_CODE_OK;
  }

  prev_node = NULL;
  for (
      curr_node = head;
      NULL != curr_node;
      curr_node = curr_node->next)
  {
    if (tail == curr_node->next)
    {
      prev_node = curr_node;
      break;
    }
  }

  if (prev_node == NULL)
  {
    return RET_CODE_INTERNAL_ERROR_010;
  }

  free(tail);

  queue->tail = prev_node;
  tail->next = NULL;

  return RET_CODE_OK;
}


