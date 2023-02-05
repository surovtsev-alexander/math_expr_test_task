#include "token.h"

#include <stdio.h>
#include <ctype.h>


typedef struct one_symbol_token_s
{
  int           symbol;
  token_id_t    token_id;
} PACKED one_symbol_token_t;

static const one_symbol_token_t one_symbol_token_map[] =
{
  {
    .symbol     = EOF,
    .token_id   = TOKEN_ID_EOF,
  },
  {
    .symbol     = '\n',
    .token_id   = TOKEN_ID_EOL,
  },
  {
    .symbol     = 'x',
    .token_id   = TOKEN_ID_X,
  },
  {
    .symbol     = 'X',
    .token_id   = TOKEN_ID_X,
  },
  {
    .symbol     = '=',
    .token_id   = TOKEN_ID_EQUAL,
  },
  {
    .symbol     = '+',
    .token_id   = TOKEN_ID_PLUS,
  },
  {
    .symbol     = '-',
    .token_id   = TOKEN_ID_MINUS,
  },
  {
    .symbol     = '*',
    .token_id   = TOKEN_ID_TIMES,
  },
  {
    .symbol     = '/',
    .token_id   = TOKEN_ID_DIVIDE,
  },
  {
    .symbol     = '-',
    .token_id   = TOKEN_ID_CHANGE_SING,
  },
};
static const uint8_t one_symbol_token_map_len = ARR_LEN(one_symbol_token_map);

static const token_priority_t token_priorities[] =
{
  [TOKEN_ID_EOF]    = TOKEN_PRIORITY_IGNORE,
  [TOKEN_ID_EOL]    = TOKEN_PRIORITY_IGNORE,
  [TOKEN_ID_NUMBER] = TOKEN_PRIORITY_PUSH_TO_OUTPUT,
  [TOKEN_ID_X]      = TOKEN_PRIORITY_PUSH_TO_OUTPUT,
  [TOKEN_ID_EQUAL]  = TOKEN_PRIORITY_UNSTACK_AND_PUSH_TO_OUTPUT,
  [TOKEN_ID_PLUS]   = TOKEN_PRIORITY_01,
  [TOKEN_ID_MINUS]  = TOKEN_PRIORITY_01,
  [TOKEN_ID_TIMES]  = TOKEN_PRIORITY_02,
  [TOKEN_ID_DIVIDE] = TOKEN_PRIORITY_02,
  [TOKEN_ID_CHANGE_SING] = TOKEN_PRIORITY_IGNORE,
};
COMPILE_TIME_ASSERT(TOKEN_IDS == ARR_LEN(token_priorities));

token_id_t token_get_id_by_symbol(int symbol)
{
  const one_symbol_token_t      *one_symbol_token;

  if (isdigit(symbol))
  {
    return TOKEN_ID_NUMBER;
  }

  for (uint8_t i = 0; i < one_symbol_token_map_len; i++)
  {
    one_symbol_token = one_symbol_token_map + i;

    if (one_symbol_token->symbol == symbol)
    {
      return one_symbol_token->token_id;
    }
  }

  return TOKEN_IDS;
}

void token_print(const token_t *token)
{
  const one_symbol_token_t      *one_symbol_token;
  token_id_t                     token_id               = token->token_id;

  if (TOKEN_IDS <= token_id)
  {
    return;
  }
  
  if (TOKEN_ID_NUMBER == token_id)
  {
    printf("%f", token->number);
    return;
  }

  for (uint8_t i = 0; i < one_symbol_token_map_len; i++)
  {
    one_symbol_token = one_symbol_token_map + i;

    if (one_symbol_token->token_id == token_id)
    {
      printf("%c", one_symbol_token->symbol);
      return;
    }
  }
}

token_priority_t token_get_priority(
    const token_t *token)
{
  token_id_t token_id;

  if (NULL == token)
  {
    return TOKEN_PRIORITY_IGNORE;
  }

  token_id = token->token_id;

  if (TOKEN_IDS <= token_id)
  {
    return TOKEN_PRIORITY_IGNORE;
  }

  return token_priorities[token_id];
}

bool token_id_is_number_or_x(token_id_t token_id)
{
  return TOKEN_ID_NUMBER == token_id ||
    TOKEN_ID_X == token_id;
}

bool token_id_is_equal_sign(token_id_t token_id)
{
  return TOKEN_ID_EQUAL == token_id;
}

bool token_id_is_operation(token_id_t token_id)
{
  return TOKEN_ID_PLUS == token_id ||
    TOKEN_ID_MINUS == token_id ||
    TOKEN_ID_TIMES == token_id ||
    TOKEN_ID_DIVIDE == token_id;
}

ret_code_t token_get_number(
    const token_t *token,
    float *res)
{
  if (TOKEN_ID_NUMBER == token->token_id)
  {
    res[0] = token->number;
    return RET_CODE_OK;
  }
  
  return RET_CODE_UNINITIALIZED;
}

bool token_id_is_number(token_id_t token_id)
{
  return TOKEN_ID_NUMBER == token_id;
}

bool token_id_is_change_sign(token_id_t token_id)
{
  return TOKEN_ID_CHANGE_SING == token_id;
}

ret_code_t token_id_calculate_result(
    token_id_t  token_id,
    float       left_value,
    float       right_value,
    float      *res)
{
  float x;

  if (TOKEN_ID_PLUS == token_id)
  {
    x = left_value + right_value;
  }
  else if (TOKEN_ID_MINUS == token_id)
  {
    x = left_value - right_value;
  }
  else if (TOKEN_ID_TIMES == token_id)
  {
    x = left_value * right_value;
  }
  else if (TOKEN_ID_DIVIDE == token_id)
  {
    x = left_value / right_value;
  }
  else
  {
    return RET_CODE_UNEXPECTED_TOKEN;
  }

  res[0] = x;

  return RET_CODE_OK;
}

