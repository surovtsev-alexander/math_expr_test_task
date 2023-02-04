#pragma once

#include "token.h"
#include "tokens_queue.h"

void reverse_polish_notation_init(void);
ret_code_t reverse_polish_notation_push_token(const token_t *token);
const tokens_queue_t* reverse_polish_notation_get_result_queue();

