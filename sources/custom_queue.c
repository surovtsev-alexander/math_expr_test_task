#include "custom_queue.h"
#include "common.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/queue.h>


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

  TAILQ_INIT(queue);
}

ret_code_t custom_queue_insert(
    custom_queue_t *queue,
    void           *data,
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

  new_entry->data = data;

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

ret_code_t custom_queue_pop(
    custom_queue_t  *queue,
    void           **data,
    bool             from_the_head)
{
  custom_queue_entry_t *entry;

  data[0] = NULL;

  if (NULL == queue || TAILQ_EMPTY(queue))
  {
    return RET_CODE_EOF;
  }

  if (HEAD_OR_FIRST == from_the_head)
  {
    entry = TAILQ_FIRST(queue);
  }
  else
  {
    entry = TAILQ_LAST(queue, tailhead);
  }

  data[0] = entry->data;

  TAILQ_REMOVE(queue, entry, entries);

  free(entry);

  return RET_CODE_OK;
}

