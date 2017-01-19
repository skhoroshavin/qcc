
#pragma once

#include "test_context.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

unsigned qcc_gen_unsigned(struct qcc_test_context *ctx, unsigned min,
                          unsigned max);

#ifdef __cplusplus
}
#endif // __cplusplus
