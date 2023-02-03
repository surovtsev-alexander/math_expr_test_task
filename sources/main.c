#include "math_expr_calculator.h"

int main()
{
  ret_code_t ret_code = math_expr_calculator_read_line_by_line_and_print_results(stdin);

  return ret_code;
}

