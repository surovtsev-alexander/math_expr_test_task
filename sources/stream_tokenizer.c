#include "stream_tokenizer.h"

#include <ctype.h>


static int stored_symbol = EOF;


void stream_tokenizer_next_token(
    FILE        *input_stream,
    token_t     *token)
{
  int                            symbol;
  token_id_t                     token_id;
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

  while (' ' == symbol)
  {
    symbol = getc(input_stream);
  }

  token_id = token_get_id_by_symbol(symbol);


  if (TOKEN_ID_NUMBER == token_id)
  {
    number   = symbol - '0';

    for (
        symbol = getc(input_stream);
        isdigit(symbol);
        symbol = getc(input_stream))
    {
      number *= 10;
      number += symbol - '0';
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

