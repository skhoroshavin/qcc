
#pragma once

#include "test_gen.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

unsigned qcc_gen_uint_any(struct qcc_test_context *ctx);
unsigned qcc_gen_uint_in_range(struct qcc_test_context *ctx, unsigned min,
                               unsigned max);
unsigned qcc_gen_uint_less_than(struct qcc_test_context *ctx, unsigned max);
unsigned qcc_gen_uint_not_less_than(struct qcc_test_context *ctx, unsigned min);
unsigned qcc_gen_uint_greater_than(struct qcc_test_context *ctx, unsigned min);
unsigned qcc_gen_uint_not_greater_than(struct qcc_test_context *ctx,
                                       unsigned max);

#define GIVEN_UINT(name, constraint, ...)                                      \
	unsigned name = qcc_gen_uint_##constraint(_ctx, ##__VA_ARGS__);            \
    qcc_test_context_register_param(_ctx, "%s: %u", #name, name);

#define ASSERT_UINT(got, cond, exp)                                            \
    do                                                                         \
    {                                                                          \
        if (!((got)cond(exp)))                                                 \
        {                                                                      \
            qcc_test_context_fail(                                             \
                _ctx, "Assertion \"%s %s %s\" (%u %s %u) failed in "           \
                      "%s (%s, line %d)\n",                                    \
                #got, #cond, #exp, (got), #cond, (exp), __FUNCTION__,          \
                __FILE__, __LINE__);                                           \
            return;                                                            \
        }                                                                      \
    } while (0)

#ifdef __cplusplus
}
#endif // __cplusplus
