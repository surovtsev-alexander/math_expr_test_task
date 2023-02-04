#include "custom_queue_helpers.h"

#include <stdlib.h>

ret_code_t custom_queue_helper_insert_token(
    custom_queue_t *queue,
    const token_t  *token,
    bool            head_not_tail)
{
  ret_code_t ret_code;
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
  
  ret_code = custom_queue_insert(
      queue,
      token_to_store,
      head_not_tail);

  if (RET_CODE_OK != ret_code)
  {
    free(token_to_store);
  }

  return ret_code;
}

