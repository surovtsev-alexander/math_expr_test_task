#include "custom_queue_helpers.h"

ret_code_t custom_queue_helper_insert_token(
    custom_queue_t *queue,
    const token_t  *token,
    bool            head_not_tail)
{
  token_t *token_to_store;

  if (NULL == token)
  {
    return RET_CODE_UNINITIALIZED;
  }

  token_to_store = custom_memdup(token, sizeof(token_t));
  if (NULL == token_to_store)
  {
    return RET_CODE_NO_MEMORY;
  }
  
  return custom_queue_insert(
      queue,
      token_to_store,
      head_not_tail);
}

