#include "reverse_polish_notation.h"

static tokens_queue_t   output_queue            = TAILQ_HEAD_INITIALIZER(output_queue);
static tokens_queue_t   stack                   = TAILQ_HEAD_INITIALIZER(stack);

void reverse_polish_notation_init(void)
{
  tokens_queue_empty(&output_queue);
  tokens_queue_empty(&stack);
}

ret_code_t reverse_polish_notation_push_token(const token_t *token)
{
  ret_code_t ret_code = RET_CODE_OK;

  tokens_queue_insert(
      &output_queue,
      token,
      INSERT_TO_THE_TAIL);

  return ret_code;
}

const tokens_queue_t* reverse_polish_notation_get_result_queue()
{
  return &output_queue;
}

