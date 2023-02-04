#pragma once

#include "stdint.h"


enum ret_code_e
{
  RET_CODE_OK   = 0,
  RET_CODE_EOF,
  RET_CODE_EOL,
  RET_CODE_PARSING_INPUT_ERROR,

  RET_CODES
};

typedef int32_t ret_code_t;

