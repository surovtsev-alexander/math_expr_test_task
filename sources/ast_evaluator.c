#include "ast_evaluator.h"

#include <stdio.h>

ret_code_t ast_evaluator_evaluate(ast_node_t *root)
{
  ret_code_t             ret_code;
  token_t               *root_token;
  token_t               *left_child_token;


  if (NULL == root)
  {
    return RET_CODE_UNINITIALIZED;
  }

  ret_code = RET_CODE_OK;

  root_token = &(root->token);

  if (!token_id_is_number(root->right->token.token_id))
  {
    ret_code = RET_CODE_UNEXPECTED_TOKEN;
  }
  if (!token_id_is_equal_sign(root->token.token_id))
  {
    ret_code == RET_CODE_UNEXPECTED_TOKEN;
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
      ret_code = RET_CODE_UNEXPECTED_TOKEN;

      break;
    }

    ret_code = RET_CODE_UNEXPECTED_TOKEN;
  }

  return ret_code;
}
