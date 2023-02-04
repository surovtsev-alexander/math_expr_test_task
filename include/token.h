#pragma once

#include "common.h"

#include <stdint.h>


enum token_id_e
{
  TOKEN_ID_EOF = 0,
  TOKEN_ID_EOL,
  TOKEN_ID_X,
  TOKEN_ID_EQUAL,
  TOKEN_ID_PLUS,
  TOKEN_ID_MINUS,
  TOKEN_ID_TIMES,
  TOKEN_ID_DIVIDE,
  TOKEN_ID_NUMBER,

  TOKEN_IDS
};

typedef uint8_t token_id_t;

typedef struct token_s
{
  token_id_t    token_id;
  float         number;
} PACKED token_t;

