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
    free(curr_entry->data);
    free(curr_entry);
    curr_entry = next_entry;
  }

  tokens_queue_init(queue);
}

bool tokens_queue_is_empty(
    const tokens_queue_t *queue)
{
  return NULL == queue || TAILQ_EMPTY(queue);
}

ret_code_t tokens_queue_insert(
    tokens_queue_t *queue,
    const void     *data,
    size_t          data_size,
    bool            to_the_head)
{
  if (NULL == queue || NULL == data)
  {
    return RET_CODE_UNINITIALIZED;
  }

  tokens_queue_entry_t *new_entry = malloc(sizeof(tokens_queue_entry_t));

  if (NULL == new_entry)
  {
    return RET_CODE_NO_MEMORY;
  }

  new_entry->data = malloc(data_size);
  if (NULL == new_entry->data)
  {
    free(new_entry);
    return RET_CODE_NO_MEMORY;
  }
  memcpy(new_entry->data, data, data_size);

  if (HEAD_OR_FIRST == to_the_head)
  {
    TAILQ_INSERT_HEAD(queue, new_entry, entries);
  }
  else
  {
    TAILQ_INSERT_TAIL(queue, new_entry, entries);
  }

  return RET_CODE_OK;
}

static const tokens_queue_entry_t * peek_entry(
    tokens_queue_t *queue,
    bool            first_not_last)
{
  if (tokens_queue_is_empty(queue))
  {
    return NULL;
  }

  if (HEAD_OR_FIRST == first_not_last)
  {
    return TAILQ_FIRST(queue);
  }
  else
  {
    return TAILQ_LAST(queue, tailhead);
  }
}

const void * tokens_queue_peek(
    tokens_queue_t *queue,
    bool            first_not_last)
{
  const tokens_queue_entry_t *entry = peek_entry(
      queue, first_not_last);

  if (NULL == entry)
  {
    return NULL;
  }

  return entry->data;
}

void tokens_queue_pop(
    tokens_queue_t *queue,
    bool            first_not_last)
{
  const tokens_queue_entry_t *entry = peek_entry(
      queue, first_not_last);

  if (NULL == entry)
  {
    return;
  }

  TAILQ_REMOVE(queue, entry, entries);
}

void tokens_queue_print(
    const tokens_queue_t *queue)
{
  tokens_queue_entry_t       *entry;
  const tokens_queue_entry_t *last;
  const token_t              *token;

  if (tokens_queue_is_empty(queue))
  {
    return;
  }

  last = TAILQ_LAST(queue, tailhead);
  TAILQ_FOREACH(entry, queue, entries)
  {
    token = entry->data;
    token_print(token);

    if (entry != last)
    {
      printf(" ");
    }
  }
  puts("");
}

