#pragma once

#include "token.h"
#include "ret_code.h"

#include <stdbool.h>
#include <sys/queue.h>


typedef struct tokens_queue_entry_s
{
  token_t                               token;
  TAILQ_ENTRY(tokens_queue_entry_s)     entries;
} tokens_queue_entry_t;

typedef TAILQ_HEAD(tailhead, tokens_queue_entry_s) tokens_queue_t;

void tokens_queue_init(
    tokens_queue_t *queue);

void tokens_queue_empty(
    tokens_queue_t *queue);

ret_code_t tokens_queue_insert_tail(
    tokens_queue_t *queue,
    const token_t  *token);

void tokens_queue_print(
    tokens_queue_t *queue);

