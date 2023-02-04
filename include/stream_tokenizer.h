#pragma once

#include "token.h"
#include "ret_code.h"

#include <stdio.h>


void stream_tokenizer_next_token(
    FILE        *input_stream,
    token_t     *token);

