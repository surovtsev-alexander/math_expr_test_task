#pragma once

#include "common.h"

#include <stdint.h>
#include <stdbool.h>


enum token_id_e
{
  TOKEN_ID_EOF = 0,
  TOKEN_ID_EOL,
  TOKEN_ID_NUMBER,
  TOKEN_ID_X,
  TOKEN_ID_EQUAL,
  TOKEN_ID_PLUS,
  TOKEN_ID_MINUS,
  TOKEN_ID_TIMES,
  TOKEN_ID_DIVIDE,
  TOKEN_ID_CHANGE_SING,

  TOKEN_IDS
};

typedef uint8_t token_id_t;

typedef struct token_s
{
  token_id_t    token_id;
  float         number;
} PACKED token_t;

enum token_priority_e
{
  TOKEN_PRIORITY_PUSH_TO_OUTPUT = 0,
  TOKEN_PRIORITY_01,
  TOKEN_PRIORITY_02,
  TOKEN_PRIORITY_UNSTACK_AND_PUSH_TO_OUTPUT,

  TOKEN_PRIORITIES
};
typedef uint8_t token_priority_t;

#define TOKEN_PRIORITY_IGNORE (TOKEN_PRIORITIES)

token_id_t       token_get_id_by_symbol(int symbol);
void             token_print(const token_t *token);
token_priority_t token_get_priority(const token_t *token);


bool token_id_is_number_or_x(token_id_t token_id);
bool token_id_is_equal_sign(token_id_t token_id);
bool token_id_is_operation(token_id_t token_id);

