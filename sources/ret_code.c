#include "ret_code.h"

bool ret_code_is_critical_error(ret_code_t ret_code)
{
  return
    RET_CODE_OK  != ret_code &&
    RET_CODE_EOF != ret_code &&
    RET_CODE_EOL != ret_code;
}

