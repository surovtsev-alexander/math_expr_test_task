#pragma once

#include "ret_code.h"

#include <stdbool.h>
#include <sys/queue.h>

#define HEAD_OR_FIRST      (true)
#define TAIL_OR_LAST       (false)

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
    void           *token,
    bool            to_the_head);

