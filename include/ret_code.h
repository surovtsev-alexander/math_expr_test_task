#pragma once

#include "stdint.h"

#include <stdbool.h>

enum ret_code_e
{
  RET_CODE_OK   = 0,
  RET_CODE_EOF,
  RET_CODE_EOL,
  RET_CODE_PARSING_INPUT_ERROR,
  RET_CODE_NO_MEMORY,
  RET_CODE_UNINITIALIZED,
  RET_CODE_IGNORE_TOKEN,
  RET_CODES
};

typedef int32_t ret_code_t;

bool ret_code_is_critical_error(ret_code_t ret_code);

