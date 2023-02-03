#include "stream_tokenizer.h"


static int stored_symbol = EOF;

typedef struct one_symbol_token_s
{
  token_id_t    token_id;
  int           symbol;
} PACKED one_symbol_token_t;

static const one_symbol_token_t one_symbol_token_map[] =
{
  {
    .token_id   = TOKEN_ID_EOF,
    .symbol     = EOF,
  },
  {
    .token_id   = TOKEN_ID_EOL,
    .symbol     = '\n',
  },
  {
    .token_id   = TOKEN_ID_PLUS,
    .symbol     = '+',
  },
  {
    .token_id   = TOKEN_ID_MINUS,
    .symbol     = '-',
  },
  {
    .token_id   = TOKEN_ID_TIMES,
    .symbol     = '*',
  },
  {
    .token_id   = TOKEN_ID_DIVIDE,
    .symbol     = '/',
  },
};
static const uint8_t one_symbol_token_map_len = ARR_LEN(one_symbol_token_map);


ret_code_t stream_tokenizer_next_token(
    FILE        *input_stream,
    token_t     *token)
{
  ret_code_t                     ret_code               = RET_CODE_OK;
  int                            symbol;
  token_id_t                     token_id;
  const one_symbol_token_t      *one_symbol_token;

  if (EOF != stored_symbol)
  {
    symbol              = stored_symbol;
    stored_symbol       = EOF;
  }
  else
  {
    symbol = getc(input_stream);
  }

  token_id = TOKEN_IDS;
  for (uint8_t i = 0; i < one_symbol_token_map_len; i++)
  {
    one_symbol_token = one_symbol_token_map + i;

    if (one_symbol_token->symbol == symbol)
    {
      token_id = one_symbol_token->token_id;
      break;
    }
  }

  if (TOKEN_IDS == token_id)
  {
    token_id = TOKEN_ID_NUMBER;
  }

  token->token_id = token_id;

  printf("symbol: %c\n", symbol);
  printf("token_id: %d\n", token_id);

  return ret_code;
}

