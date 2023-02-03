#pragma once

#include "token.h"
#include "ret_code.h"

#include <stdio.h>


ret_code_t stream_tokenizer_next_token(
    FILE        *input_stream,
    token_t     *token);

