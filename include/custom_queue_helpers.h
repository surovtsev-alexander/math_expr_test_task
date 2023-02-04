#pragma once

#include "custom_queue.h"
#include "token.h"

ret_code_t custom_queue_helper_insert_token(
    custom_queue_t *queue,
    const token_t  *token,
    bool            head_not_tail);

