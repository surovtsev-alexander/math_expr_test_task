#include "ast.h"
#include "ast_node.h"
#include "token.h"

#include <stdio.h>

ast_node_t *tree_head = NULL;

static ret_code_t delete_tree(void);
static ret_code_t check_x_and_eaqual_signs_in_queue(const custom_queue_t *queue);

static bool  _result_is_defined = false;
static float _result;

ret_code_t abstract_syntax_tree_init(void)
{
  ret_code_t ret_code;
  ret_code = delete_tree();

  _result_is_defined = false;

  return ret_code;
}

ret_code_t abstract_syntax_tree_create(const custom_queue_t *queue)
{
  ret_code_t ret_code = RET_CODE_OK;

  _result_is_defined = false;
  ret_code = check_x_and_eaqual_signs_in_queue(queue);

  return ret_code;
}

ret_code_t abstract_syntax_tree_evaluate_x(float *result)
{
  if (_result_is_defined)
  {
    result[0] = _result;
    return RET_CODE_OK;
  }
  else
  {
    return RET_CODE_RESULT_IS_UNDEFINED;
  }
}

static ret_code_t delete_tree(void)
{
  //
  return RET_CODE_OK;
}


static ret_code_t check_x_and_eaqual_signs_in_queue(const custom_queue_t *queue)
{
  int                   x_signs_count = 0;
  int                   equal_signs_count = 0;
  custom_queue_entry_t *entry;
  token_t              *token;
  token_id_t            token_id;

  TAILQ_FOREACH(entry, queue, entries)
  {
    token = entry->data;
    token_id = token->token_id;

    if (TOKEN_ID_X == token_id)
    {
      x_signs_count++;
    }
    else if (TOKEN_ID_EQUAL == token_id)
    {
      equal_signs_count++;
    }
  }

  printf("x_count: %d\n=_count: %d\n",
      x_signs_count,
      equal_signs_count);

  if (
      x_signs_count == 1 &&
      equal_signs_count == 1)
  {
    return RET_CODE_OK;
  }

  if (x_signs_count == 0)
  {
    return RET_CODE_NO_X_SIGN;
  }

  if (x_signs_count > 1)
  {
    return RET_CODE_MORE_THAN_ONE_X_SIGN;
  }

  if (equal_signs_count == 0)
  {
    return RET_CODE_NO_EQUAL_SING;
  }

  if (equal_signs_count > 1)
  {
    return RET_CODE_MORE_TAN_ONE_EQUAL_SING;
  }


  return RET_CODE_OK;
}

