#pragma once

#include "token.h"
#include "ret_code.h"

#include <stdbool.h>
#include <sys/queue.h>

#define HEAD_OR_FIRST      (true)
#define LAST_OR_TAIL       (false)

typedef struct tokens_queue_entry_s
{
  void                              *data;
  TAILQ_ENTRY(tokens_queue_entry_s)  entries;
} tokens_queue_entry_t;

typedef TAILQ_HEAD(tailhead, tokens_queue_entry_s) tokens_queue_t;

void tokens_queue_init(
    tokens_queue_t *queue);

void tokens_queue_empty(
    tokens_queue_t *queue);

bool tokens_queue_is_empty(
    const tokens_queue_t *queue);

ret_code_t tokens_queue_insert(
    tokens_queue_t *queue,
    const void     *token,
    size_t          data_size,
    bool            to_the_head);

const void * tokens_queue_peek(
    tokens_queue_t *queue,
    bool            first_not_last);

void tokens_queue_pop(
    tokens_queue_t *queue,
    bool            first_not_last);

void tokens_queue_print(
    const tokens_queue_t *queue);

