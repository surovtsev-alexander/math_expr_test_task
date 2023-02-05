#include "ast_evaluator.h"

#include <stdio.h>
#include <stdlib.h>

ret_code_t ast_evaluator_evaluate(ast_node_t *root)
{
  ret_code_t             ret_code;
  token_t               *right_child_token;
  token_t               *left_child_token;
  ast_node_t            *left_then_left;
  ast_node_t            *left_then_right;


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
    left_child_token = &(root->left->token);
    if (token_id_is_x(left_child_token->token_id))
    {
      break;
    }
    else if (token_id_is_change_sign(left_child_token->token_id))
    {
      left_then_left = root->left->left;

      free(root->left);

      root->left = left_then_left;

      right_child_token->number *= -1;

      continue;
    }

    ret_code = RET_CODE_UNEXPECTED_TOKEN;
  }

  return ret_code;
}
