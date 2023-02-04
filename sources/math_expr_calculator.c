#include "math_expr_calculator.h"
#include "stream_tokenizer.h"
#include "tokens_queue.h"

#include <sys/queue.h>


static ret_code_t read_next_line_and_print_result(
    FILE *input_stream);


ret_code_t math_expr_calculator_read_line_by_line_and_print_results(
    FILE *input_stream)
{
  ret_code_t    ret_code = RET_CODE_OK;

  while (RET_CODE_OK  == ret_code)
  {
    ret_code = read_next_line_and_print_result(input_stream);

    if (RET_CODE_EOL == ret_code)
    {
      ret_code = RET_CODE_OK;
    }
  }

  if (RET_CODE_EOF == ret_code)
  {
    ret_code = RET_CODE_OK;
  }

  return ret_code;
}

static ret_code_t read_next_line_and_print_result(
    FILE *input_stream)
{
  token_t               token;
  token_id_t            token_id;
  ret_code_t            ret_code        = RET_CODE_OK;
  tokens_queue_t        output_queue    = TAILQ_HEAD_INITIALIZER(output_queue);

  tokens_queue_init(&output_queue);

  while (RET_CODE_OK == ret_code)
  {
    stream_tokenizer_next_token(input_stream, &token);

    token_id = token.token_id;

    if (TOKEN_ID_EOF == token_id)
    {
      ret_code = RET_CODE_EOF;
      break;
    }

    if (TOKEN_ID_EOL == token_id)
    {
      ret_code = RET_CODE_EOL;
      break;
    }

    if (TOKEN_IDS == token_id)
    {
      ret_code = RET_CODE_PARSING_INPUT_ERROR;
      break;
    }

    tokens_queue_insert_tail(
        &output_queue,
        &token);
  }

  tokens_queue_print(&output_queue);

  tokens_queue_empty(&output_queue);

  return ret_code;
}

