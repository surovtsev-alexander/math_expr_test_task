#include "ast_folder.h"
#include "custom_queue.h"
#include "custom_queue_helpers.h"


#include <stdlib.h>


static ret_code_t push_node(
  custom_queue_t *stack,
  ast_node_t *ast_note,
  uint8_t left_processed,
  uint8_t right_processed);


ret_code_t ast_folder_fold(ast_node_t *root)
{
  ret_code_t            ret_code;
  custom_queue_t        stack = TAILQ_HEAD_INITIALIZER(stack);
  custom_queue_entry_t *curr_entry;
  ast_folder_node_t    *ast_folder_node;
  ast_node_t           *ast_node;
  bool                  left_processed;
  bool                  right_processed;
  bool                  need_to_process_left;
  bool                  need_to_process_right;
  bool                  pass_to_processing_left;
  bool                  pass_to_processing_right;
  const token_t        *token;
  token_id_t            token_id;
  float                 left_value;
  float                 right_value;
  float                 calculated_ast_node_value;

  ret_code = RET_CODE_OK;

  custom_queue_empty(&stack);

  if (NULL == root)
  {
    return RET_CODE_UNINITIALIZED;
  }

  ret_code = push_node(
      &stack,
      root,
      false,
      false);
  if (RET_CODE_OK != ret_code)
  {
    return ret_code;
  }

  while (!TAILQ_EMPTY(&stack) && RET_CODE_OK == ret_code)
  {
    curr_entry = TAILQ_FIRST(&stack);
    TAILQ_REMOVE(&stack, curr_entry, entries);

    ast_folder_node = curr_entry->data;

    left_processed  = ast_folder_node->left_processed;
    right_processed = ast_folder_node->right_processed;

    ast_node = ast_folder_node->ast_node;

    need_to_process_left =
      NULL != ast_node->left &&
      !left_processed;
    need_to_process_right =
      NULL != ast_node->right &&
      !right_processed;

    pass_to_processing_left = need_to_process_left;
    pass_to_processing_right =
      !pass_to_processing_left &&
      need_to_process_right;

    if (pass_to_processing_left || pass_to_processing_right)
    {
      ret_code = push_node(
          &stack,
          ast_node,
          left_processed || pass_to_processing_left,
          right_processed || pass_to_processing_right);
    }

    if (RET_CODE_OK == ret_code && pass_to_processing_left)
    {
      ret_code = push_node(
          &stack,
          ast_node->left,
          false,
          false);
    }

    if (RET_CODE_OK == ret_code && pass_to_processing_right)
    {
      ret_code = push_node(
          &stack,
          ast_node->right,
          false,
          false);
    }

    if (RET_CODE_OK == ret_code &&
        !pass_to_processing_left &&
        !pass_to_processing_right)
    {
      token = &(ast_node->token);
      token_id = token->token_id;

      if (token_id_is_operation(token_id) &&
          token_id_is_number(ast_node->left->token.token_id) &&
          token_id_is_number(ast_node->right->token.token_id))
      {
        ret_code = token_get_number(
            &(ast_node->left->token),
            &left_value);

        if (RET_CODE_OK == ret_code)
        {
          ret_code = token_get_number(
              &(ast_node->right->token),
              &right_value);
        }

        if (RET_CODE_OK == ret_code)
        {
          ret_code = token_id_calculate_result(
              token_id,
              left_value,
              right_value,
              &calculated_ast_node_value);
        }

        if (RET_CODE_OK == ret_code)
        {
          free(ast_node->left);
          free(ast_node->right);
          ast_node->token.token_id = TOKEN_ID_NUMBER;
          ast_node->token.number = calculated_ast_node_value;
          ast_node->left = NULL;
          ast_node->right = NULL;
        }
      }
      else if (token_id_is_change_sign(token_id) &&
          token_id_is_number(ast_node->left->token.token_id))
      {
        ret_code = token_get_number(
            &(ast_node->left->token),
            &left_value);

        if (RET_CODE_OK == ret_code)
        {
          free(ast_node->left);
          ast_node->token.token_id = TOKEN_ID_NUMBER;
          ast_node->token.number = left_value * (-1);
          ast_node->left = NULL;
          ast_node->right = NULL;
        }
      }
    }

    free(curr_entry);
  }

  custom_queue_empty(&stack);

  return ret_code;
}

static ret_code_t push_node(
  custom_queue_t *stack,
  ast_node_t *ast_node,
  uint8_t left_processed,
  uint8_t right_processed)
{
  ast_folder_node_t ast_folder_node = {
    .ast_node = ast_node,
    .left_processed = left_processed,
    .right_processed = right_processed,
  };
  return custom_queue_helper_insert_ast_folder_node(
      stack,
      &ast_folder_node,
      HEAD_OR_FIRST);
}

