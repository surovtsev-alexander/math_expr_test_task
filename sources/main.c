#include "math_expr_calculator.h"

#include <stdio.h>

int main()
{
  ret_code_t ret_code = math_expr_calculator_read_line_by_line_and_print_results(stdin);

  if (RET_CODE_OK != ret_code)
  {
    printf("ret_code: %d\n", ret_code);
    puts("for more information, please, inspect './include/ret_code.h'.");
  }

  return ret_code;
}

