#include "reverse_polish_notation.h"
#include "custom_queue_helpers.h"

#include <stdio.h>

static ret_code_t push_to_output(
    const token_t *token);
static ret_code_t stack_token(
    const token_t *token);
static ret_code_t unstack_and_push_to_output();
static ret_code_t unstack_with_greater_priotity_and_stack(
    const token_t *token);

static ret_code_t state_wait_operand_token_id_checker(const token_id_t token_id);
static ret_code_t state_wait_operation_token_id_checker(const token_id_t token_id);

static custom_queue_t   output_queue            = TAILQ_HEAD_INITIALIZER(output_queue);
static custom_queue_t   stack                   = TAILQ_HEAD_INITIALIZER(stack);


const static token_t change_sign_token =
{
  .token_id = TOKEN_ID_CHANGE_SING,
};


static bool             change_number_sign      = false;

enum state_e
{
  STATE_WAIT_OPERAND,
  STATE_WAIT_OPERATION,

  STATES
};
typedef uint8_t state_t;

static state_t state = STATE_WAIT_OPERAND;

typedef  ret_code_t (* state_token_id_checker_t)(const token_id_t token_id);

const static state_token_id_checker_t state_token_id_checker_map[] =
{
  [STATE_WAIT_OPERAND]   = state_wait_operand_token_id_checker,
  [STATE_WAIT_OPERATION] = state_wait_operation_token_id_checker,
};
COMPILE_TIME_ASSERT(STATES == ARR_LEN(state_token_id_checker_map));


void reverse_polish_notation_init(void)
{
  custom_queue_empty(&output_queue);
  custom_queue_empty(&stack);

  state = STATE_WAIT_OPERAND;
  change_number_sign = false;
}

ret_code_t reverse_polish_notation_push_token(const token_t *token)
{
  token_priority_t      token_priority;
  ret_code_t            ret_code = RET_CODE_OK;

  if (NULL == token)
  {
    return RET_CODE_UNINITIALIZED;
  }

  ret_code = state_token_id_checker_map[state](token->token_id);

  if (RET_CODE_OK != ret_code)
  {
    return ret_code;
  }

  token_priority = token_get_priority(token);

  if (TOKEN_PRIORITY_PUSH_TO_OUTPUT == token_priority)
  {
    ret_code = push_to_output(token);
  }
  else if (TOKEN_PRIORITY_01 <= token_priority &&
      TOKEN_PRIORITY_02 >= token_priority)
  {
    ret_code = unstack_with_greater_priotity_and_stack(token);
  }
  else if (TOKEN_PRIORITY_UNSTACK_AND_PUSH_TO_OUTPUT == token_priority)
  {
    ret_code = reverse_polish_notation_unstack_all_to_output();
    if (RET_CODE_OK == ret_code)
    {
      ret_code = push_to_output(token);
    }
  }
  else
  {
    ret_code = RET_CODE_IGNORE_TOKEN;
  }

  if (RET_CODE_OK == ret_code && change_number_sign)
  {
    ret_code = push_to_output(
        &change_sign_token);
    change_number_sign = false;
  }

  return ret_code;
}

static ret_code_t state_wait_operand_token_id_checker(token_id_t token_id)
{
  if (TOKEN_ID_PLUS == token_id ||
      TOKEN_ID_MINUS == token_id)
  {
    if (TOKEN_ID_MINUS == token_id)
    {
      change_number_sign = !change_number_sign;
    }

    return RET_CODE_IGNORE_TOKEN;
  }

  if (token_id_is_equal_sign(token_id) ||
      token_id_is_operation(token_id))
  {
    return RET_CODE_UNEXPECTED_TOKEN;
  }

  if (token_id_is_number_or_x(token_id))
  {
    state = STATE_WAIT_OPERATION;
    return RET_CODE_OK;
  }

  return RET_CODE_IGNORE_TOKEN;
}

static ret_code_t state_wait_operation_token_id_checker(token_id_t token_id)
{
  if (token_id_is_operation(token_id) ||
      token_id_is_equal_sign(token_id))
  {
    state = STATE_WAIT_OPERAND;
    return RET_CODE_OK;
  }

  if (token_id_is_number_or_x(token_id))
  {
    return RET_CODE_UNEXPECTED_TOKEN;
  }

  return RET_CODE_IGNORE_TOKEN;
}

static ret_code_t push_to_output(
    const token_t *token)
{
  return custom_queue_helper_insert_token(
        &output_queue,
        token,
        TAIL_OR_LAST);
}

static ret_code_t stack_token(
    const token_t *token)
{
  return custom_queue_helper_insert_token(
      &stack,
      token,
      TAIL_OR_LAST);
}

static ret_code_t unstack_and_push_to_output()
{
  ret_code_t     ret_code;
  const token_t *token_from_stack;

  ret_code = custom_queue_pop(
      &stack,
      (void **)&token_from_stack,
      TAIL_OR_LAST);
  if (RET_CODE_OK != ret_code)
  {
    return ret_code;
  }

  ret_code = push_to_output(
      token_from_stack);

  return ret_code;
}

static ret_code_t unstack_with_greater_priotity_and_stack(
    const token_t *token)
{
  ret_code_t       ret_code = RET_CODE_OK;
  token_priority_t token_priority;

  if (NULL == token)
  {
    return RET_CODE_OK;
  }

  token_priority = token_get_priority(token);

  while (!TAILQ_EMPTY(&stack))
  {
    const token_t *token_from_stack = TAILQ_LAST(&stack, tailhead)->data;

    if (token_get_priority(token_from_stack) >= token_priority)
    {
      ret_code = unstack_and_push_to_output();
    }
    else
    {
      break;
    }

    if (RET_CODE_OK != ret_code)
    {
      break;
    }
  }

  if (RET_CODE_OK == ret_code)
  {
    ret_code = stack_token(token);
  }

  return ret_code;
}

ret_code_t reverse_polish_notation_unstack_all_to_output(void)
{
  ret_code_t ret_code = RET_CODE_OK;

  while (
      RET_CODE_OK == ret_code &&
      !TAILQ_EMPTY(&stack))
  {
    ret_code = unstack_and_push_to_output();

    if (RET_CODE_EOF == ret_code)
    {
      ret_code = RET_CODE_OK;
      break;
    }
  }

  return ret_code;
}

const custom_queue_t* reverse_polish_notation_get_result_queue(void)
{
  return &output_queue;
}

