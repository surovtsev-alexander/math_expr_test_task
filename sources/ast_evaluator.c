#include "ast_evaluator.h"


ret_code_t ast_evaluator_evaluate(ast_node_t *root)
{
  ret_code_t             ret_code;
  token_t               *root_token;
  token_id_t             root_token_id;


  if (NULL == root)
  {
    return RET_CODE_UNINITIALIZED;
  }

  ret_code = RET_CODE_OK;

  root_token = &(root->token);

  while (RET_CODE_OK == ret_code)
  {
    root_token_id = root_token->token_id;
    if (token_id_is_number(root_token_id))
    {
      break;
    }
    else if (token_id_is_equal_sign(root_token_id))
    {
      // processing
      break;
    }
    else
    {
      ret_code = RET_CODE_UNEXPECTED_TOKEN;
      break;
    }
  }

  return ret_code;
}
