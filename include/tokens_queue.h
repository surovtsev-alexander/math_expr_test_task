#pragma once

#include "token.h"
#include "ret_code.h"

#include <stdbool.h>


typedef struct node_s
{
  token_t        token;
  struct node_s *next;
} node_t;


typedef struct tokens_queue_s
{
  node_t *head;
  node_t *tail;
} tokens_queue_t;


tokens_queue_t* tokens_queue_create();
ret_code_t tokens_queue_destroy_and_set_null(
    tokens_queue_t **pointer_to_queue);

bool tokens_queue_is_empty(
    const tokens_queue_t * const queue);
ret_code_t tokens_queue_empty(
    tokens_queue_t * const queue);

ret_code_t tokens_queue_push_front(
    tokens_queue_t * const queue,
    const token_t token);
ret_code_t tokens_queue_push_back(
    tokens_queue_t * const queue,
    const token_t token);

const token_t* tokens_queue_peek_front(
    tokens_queue_t * const queue);
const token_t* tokens_queue_peek_back(
    tokens_queue_t * const queue);

ret_code_t tokens_queue_pop_front(
    tokens_queue_t * const queue);
ret_code_t tokens_queue_pop_back(
    tokens_queue_t * const queue);

