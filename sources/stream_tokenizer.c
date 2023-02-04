#include "stream_tokenizer.h"

#include <ctype.h>


static int stored_symbol = EOF;


void stream_tokenizer_next_token(
    FILE        *input_stream,
    token_t     *token)
{
  int        symbol;
  token_id_t token_id;
  float      number;
  bool       float_number_after_point_symbol;
  float      after_point_multiplier;
  int        x;

  if (EOF != stored_symbol)
  {
    symbol              = stored_symbol;
    stored_symbol       = EOF;
  }
  else
  {
    symbol = getc(input_stream);
  }

  while (' ' == symbol)
  {
    symbol = getc(input_stream);
  }

  token_id = token_get_id_by_symbol(symbol);


  if (TOKEN_ID_NUMBER == token_id)
  {
    number   = symbol - '0';

    float_number_after_point_symbol = false;
    after_point_multiplier = 1;
    for (;;)
    {
      symbol = getc(input_stream);

      if ('.' == symbol || ',' == symbol)
      {
        if (float_number_after_point_symbol)
        {
          token_id = TOKEN_IDS;
          break;
        }
        float_number_after_point_symbol = true;
        continue;
      }

      if (isdigit(symbol))
      {
        x = symbol - '0';
        if (!float_number_after_point_symbol)
        {
          number *= 10;
          number += x;
        }
        else
        {
          after_point_multiplier /= 10;
          number += after_point_multiplier * x;
        }
        continue;
      }

      break;
    }
    stored_symbol = symbol;
  }

  if (TOKEN_IDS == token_id)
  {
    fprintf(stderr, "unexpected symbol: '%c'\n", symbol);
  }

  token->token_id = token_id;
  if (TOKEN_ID_NUMBER == token_id)
  {
    token->number = number;
  }
}

