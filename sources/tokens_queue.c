#include "tokens_queue.h"
#include "common.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/queue.h>


void tokens_queue_init(
    tokens_queue_t *queue)
{
  TAILQ_INIT(queue);
}

void tokens_queue_empty(
    tokens_queue_t *queue)
{
  tokens_queue_entry_t *curr_entry;
  tokens_queue_entry_t *next_entry;

  if (NULL == queue)
  {
    return;
  }

  curr_entry = TAILQ_FIRST(queue);
  while (NULL != curr_entry)
  {
    next_entry = TAILQ_NEXT(curr_entry, entries);
    free(curr_entry);
    curr_entry = next_entry;
  }

  tokens_queue_init(queue);
}

ret_code_t tokens_queue_insert(
    tokens_queue_t *queue,
    const token_t  *token,
    bool            to_the_head)
{
  if (NULL == queue)
  {
    return RET_CODE_UNINITIALIZED;
  }

  tokens_queue_entry_t *new_entry = malloc(sizeof(tokens_queue_entry_t));

  if (NULL == new_entry)
  {
    return RET_CODE_NO_MEMORY;
  }

  memcpy(&(new_entry->token), token, sizeof(token_t));

  if (INSERT_TO_THE_HEAD == to_the_head)
  {
    TAILQ_INSERT_HEAD(queue, new_entry, entries);
  }
  else
  {
    TAILQ_INSERT_TAIL(queue, new_entry, entries);
  }

  return RET_CODE_OK;
}

const token_t * tokens_queue_peek(
    tokens_queue_t *queue,
    bool            first_not_last)
{
  const tokens_queue_entry_t *entry;

  if (NULL == queue || TAILQ_EMPTY(queue))
  {
    return NULL;
  }

  if (PEEK_FIRST == first_not_last)
  {
    entry = TAILQ_FIRST(queue);
  }
  else
  {
    entry = TAILQ_LAST(queue, tailhead);
  }
  assert(NULL != entry);

  return &(entry->token);
}

void tokens_queue_print(
    const tokens_queue_t *queue)
{
  tokens_queue_entry_t       *entry;
  const tokens_queue_entry_t *last;

  if (NULL == queue)
  {
    return;
  }

  last = TAILQ_LAST(queue, tailhead);

  if (!TAILQ_EMPTY(queue))
  {
    TAILQ_FOREACH(entry, queue, entries)
    {
      token_print(&(entry->token));

      if (entry != last)
      {
        printf(" ");
      }
    }
    puts("");
  }
}

