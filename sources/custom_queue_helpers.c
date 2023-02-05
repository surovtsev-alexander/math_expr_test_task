#include "custom_queue_helpers.h"

#include <stdlib.h>


static ret_code_t insert_to_custom_queue_helper(
    custom_queue_t *queue,
    const void     *data_to_copy,
    size_t          data_size,
    bool            head_not_tail);

ret_code_t custom_queue_helper_insert_token(
    custom_queue_t *queue,
    const token_t  *token,
    bool            head_not_tail)
{
  return insert_to_custom_queue_helper(
      queue,
      token,
      sizeof(token_t),
      head_not_tail);
}

ret_code_t custom_queue_helper_insert_ast_node(
    custom_queue_t   *queue,
    const ast_node_t *ast_node,
    bool              head_not_tail)
{
  return insert_to_custom_queue_helper(
      queue,
      ast_node,
      sizeof(ast_node_t),
      head_not_tail);
}

ret_code_t custom_queue_helper_insert_ast_display_node(
    custom_queue_t           *queue,
    const ast_display_node_t *ast_display_node,
    bool                      head_not_tail)
{
  return insert_to_custom_queue_helper(
      queue,
      ast_display_node,
      sizeof(ast_display_node_t),
      head_not_tail);
}

ret_code_t custom_queue_helper_insert_ast_folder_node(
    custom_queue_t    *queue,
    ast_folder_node_t *ast_folder_node,
    bool               head_not_tail)
{
  return insert_to_custom_queue_helper(
      queue,
      ast_folder_node,
      sizeof(ast_folder_node_t),
      head_not_tail);
}


static ret_code_t insert_to_custom_queue_helper(
    custom_queue_t *queue,
    const void     *data_to_copy,
    size_t          data_size,
    bool            head_not_tail)
{
  ret_code_t ret_code;
  void *data_to_store;

  if (NULL == data_to_copy)
  {
    return RET_CODE_UNINITIALIZED;
  }

  data_to_store = custom_memdup(data_to_copy, data_size);
  if (NULL == data_to_store)
  {
    return RET_CODE_NO_MEMORY;
  }
  
  ret_code = custom_queue_insert(
      queue,
      data_to_store,
      head_not_tail);

  if (RET_CODE_OK != ret_code)
  {
    free(data_to_store);
  }

  return ret_code;
}

