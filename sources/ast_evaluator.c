#include "ast_evaluator.h"

#include <stdio.h>
#include <stdlib.h>

ret_code_t ast_evaluator_evaluate(ast_node_t *root)
{
  ret_code_t             ret_code;
  token_t               *right_child_token;
  token_t               *left_child_token;
  token_id_t             left_child_token_id;
  ast_node_t            *left_then_left;
  ast_node_t            *left_then_right;
  float                  right_child_number;


  if (NULL == root)
  {
    return RET_CODE_UNINITIALIZED;
  }

  ret_code = RET_CODE_OK;

  if (!token_id_is_equal_sign(root->token.token_id))
  {
    ret_code = RET_CODE_UNEXPECTED_TOKEN;
  }

  if (RET_CODE_OK == ret_code)
  {
    if (NULL == root->right)
    {
      ret_code = RET_CODE_UNINITIALIZED;
    }
    if (RET_CODE_OK == ret_code)
    {
      right_child_token = &(root->right->token);
      if (!token_id_is_number(right_child_token->token_id))
      {
        ret_code = RET_CODE_UNEXPECTED_TOKEN;
      }
    }
  }

  while (RET_CODE_OK == ret_code)
  {
    left_child_token    = &(root->left->token);
    left_child_token_id = left_child_token->token_id;
    if (token_id_is_x(left_child_token_id))
    {
      break;
    }
    
    left_then_left = root->left->left;

    if (token_id_is_change_sign(left_child_token_id))
    {
      free(root->left);

      root->left = left_then_left;

      right_child_token->number *= -1;

      continue;
    }

    if (!token_id_is_operation(left_child_token_id))
    {

      ret_code = RET_CODE_UNEXPECTED_TOKEN;
      break;
    }

    left_then_right = root->left->right;

    if (token_id_is_number(left_then_left->token.token_id))
    {
      right_child_number = right_child_token->number;
      ret_code = token_id_inverse_right_side_value(
          left_child_token_id,
          &(right_child_number));
      right_child_token->number = right_child_number;

      if (RET_CODE_OK == ret_code)
      {
        root->left->left  = left_then_right;
        root->left->right = left_then_left;
      }
      continue;
    }

    ret_code = RET_CODE_INTERNAL_ERROR_001;
  }

  return ret_code;
}

