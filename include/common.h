#pragma once

#define PACKED __attribute__((__packed__))

#define ARR_LEN(arr__) (sizeof(arr__) / sizeof(arr__[0]))

#define ASSERT_CONCAT_(a, b) a##b
#define ASSERT_CONCAT(a, b) ASSERT_CONCAT_(a, b)
#define COMPILE_TIME_ASSERT(e) enum { ASSERT_CONCAT(assert_line_, __LINE__) = 1/(!!(e)) }

