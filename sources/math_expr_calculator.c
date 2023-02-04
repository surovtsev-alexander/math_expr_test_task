#include "math_expr_calculator.h"
#include "stream_tokenizer.h"
#include "reverse_polish_notation.h"
#include "abstract_syntax_tree.h"


static ret_code_t read_next_line_and_print_result(
    FILE *input_stream);

static ret_code_t read_line_and_calculate_polish_notation(
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
  ret_code_t ret_code                = RET_CODE_OK;
  ret_code_t ret_code_2              = RET_CODE_OK;
  float      evaluation_result;


  ret_code = read_line_and_calculate_polish_notation(input_stream);

  if (!ret_code_is_critical_error(ret_code))
  {
    ret_code_2 = reverse_polish_notation_unstack_all_to_output();
    if (RET_CODE_OK == ret_code || ret_code_is_critical_error(ret_code_2))
    {
      ret_code = ret_code_2;
    }
  }
  
  if (!ret_code_is_critical_error(ret_code))
  {
    tokens_queue_print(
        reverse_polish_notation_get_result_queue());
  }

  if (!ret_code_is_critical_error(ret_code))
  {
    ret_code_2 = abstract_syntax_tree_create(
        reverse_polish_notation_get_result_queue());

    if (RET_CODE_OK == ret_code || ret_code_is_critical_error(ret_code_2))
    {
      ret_code = ret_code_2;
    }
  }

  if (!ret_code_is_critical_error(ret_code))
  {
    ret_code_2 = abstract_syntax_tree_evaluate_x(
        &evaluation_result);

    if (RET_CODE_OK == ret_code || ret_code_is_critical_error(ret_code_2))
    {
      printf("%f\n", evaluation_result);
    }

    if (ret_code == RET_CODE_OK)
    {
      ret_code = ret_code_2;
    }
  }

  if (ret_code_is_critical_error(ret_code))
  {
    fprintf(stderr, "result is undefined\n");
  }

  reverse_polish_notation_init();

  return ret_code;
}

static ret_code_t read_line_and_calculate_polish_notation(
    FILE *input_stream)
{
  token_t    token;
  token_id_t token_id;
  ret_code_t ret_code = RET_CODE_OK;

  reverse_polish_notation_init();

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

    ret_code = reverse_polish_notation_push_token(
        &token);

    if (RET_CODE_IGNORE_TOKEN == ret_code)
    {
      ret_code = RET_CODE_OK;
    }
  }

  return ret_code;
}

