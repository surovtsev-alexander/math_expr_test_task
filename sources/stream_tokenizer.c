#include "stream_tokenizer.h"

#include <ctype.h>


static int stored_symbol = EOF;

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
};
static const uint8_t one_symbol_token_map_len = ARR_LEN(one_symbol_token_map);


void stream_tokenizer_next_token(
    FILE        *input_stream,
    token_t     *token)
{
  int                            symbol;
  token_id_t                     token_id;
  const one_symbol_token_t      *one_symbol_token;
  float                          number;

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

  if (TOKEN_IDS == token_id && isdigit(symbol))
  {
    number   = symbol - '0';
    token_id = TOKEN_ID_NUMBER;

    for (symbol = getc(input_stream); isdigit(symbol); symbol = getc(input_stream))
    {
      number *= 10;
      number += symbol - '0';
    }
    stored_symbol = symbol;
  }


  token->token_id = token_id;
  if (TOKEN_ID_NUMBER == token_id)
  {
    token->number = number;
  }
}

