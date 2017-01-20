
#pragma once

#include "test_context.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

unsigned qcc_gen_unsigned(struct qcc_test_context *ctx, unsigned min,
                          unsigned max);

#define GIVEN_UINT_IN_RANGE(name, min, max)                                    \
    unsigned name = qcc_gen_unsigned(_ctx, min, max);                          \
    qcc_test_context_register_param(_ctx, "%s: %u", #name, name);

#define ASSERT_UINT(got, cond, exp)                                            \
    do                                                                         \
    {                                                                          \
        if (!((got)cond(exp)))                                                 \
        {                                                                      \
            _ctx->result = QCC_TEST_FAIL;                                      \
            _ctx->error = qcc_arena_sprintf(                                   \
                &_ctx->arena, "Assertion \"%s %s %s\" (%u %s %u) failed in "   \
                              "%s (%s, line %d)\n",                            \
                #got, #cond, #exp, (got), #cond, (exp), __FUNCTION__,          \
                __FILE__, __LINE__);                                           \
            return;                                                            \
        }                                                                      \
    } while (0)

#ifdef __cplusplus
}
#endif // __cplusplus
