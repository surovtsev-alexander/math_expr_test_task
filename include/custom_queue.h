#pragma once

#include "token.h"
#include "ret_code.h"

#include <stdbool.h>
#include <sys/queue.h>

#define HEAD_OR_FIRST      (true)
#define LAST_OR_TAIL       (false)

typedef struct custom_queue_entry_s
{
  void                              *data;
  TAILQ_ENTRY(custom_queue_entry_s)  entries;
} custom_queue_entry_t;

typedef TAILQ_HEAD(tailhead, custom_queue_entry_s) custom_queue_t;

void custom_queue_empty(
    custom_queue_t *queue);

ret_code_t custom_queue_insert(
    custom_queue_t *queue,
    const void     *token,
    size_t          data_size,
    bool            to_the_head);

const void * custom_queue_peek(
    custom_queue_t *queue,
    bool            first_not_last);

void custom_queue_pop(
    custom_queue_t *queue,
    bool            first_not_last);

