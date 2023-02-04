#pragma once

#include "token.h"
#include "ret_code.h"

#include <stdbool.h>
#include <sys/queue.h>

#define INSERT_TO_THE_HEAD (true)
#define INSERT_TO_THE_TAIL (false)

#define PEEK_FIRST         (true)
#define PEEK_LAST          (false)

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

ret_code_t tokens_queue_insert(
    tokens_queue_t *queue,
    const token_t  *token,
    bool            to_the_head);

const token_t * tokens_queue_peek(
    tokens_queue_t *queue,
    bool            first_not_last);

void tokens_queue_print(
    const tokens_queue_t *queue);

