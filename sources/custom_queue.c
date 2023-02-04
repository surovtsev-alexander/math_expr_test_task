#include "custom_queue.h"
#include "common.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/queue.h>


void custom_queue_init(
    custom_queue_t *queue)
{
  TAILQ_INIT(queue);
}

void custom_queue_empty(
    custom_queue_t *queue)
{
  custom_queue_entry_t *curr_entry;
  custom_queue_entry_t *next_entry;

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

  custom_queue_init(queue);
}

ret_code_t custom_queue_insert(
    custom_queue_t *queue,
    const void     *data,
    size_t          data_size,
    bool            to_the_head)
{
  if (NULL == queue || NULL == data)
  {
    return RET_CODE_UNINITIALIZED;
  }

  custom_queue_entry_t *new_entry = malloc(sizeof(custom_queue_entry_t));

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

static const custom_queue_entry_t * peek_entry(
    custom_queue_t *queue,
    bool            first_not_last)
{
  if (TAILQ_EMPTY(queue))
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

const void * custom_queue_peek(
    custom_queue_t *queue,
    bool            first_not_last)
{
  const custom_queue_entry_t *entry = peek_entry(
      queue, first_not_last);

  if (NULL == entry)
  {
    return NULL;
  }

  return entry->data;
}

void custom_queue_pop(
    custom_queue_t *queue,
    bool            first_not_last)
{
  const custom_queue_entry_t *entry = peek_entry(
      queue, first_not_last);

  if (NULL == entry)
  {
    return;
  }

  TAILQ_REMOVE(queue, entry, entries);
}

