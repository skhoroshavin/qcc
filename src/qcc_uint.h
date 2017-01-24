
#pragma once

#include "qcc_array.h"

QCC_BEGIN_HEADER

struct qcc_generator *qcc_gen_uint_equal_to(struct qcc_context *ctx,
                                            unsigned value);
struct qcc_generator *qcc_gen_uint_in_range(struct qcc_context *ctx,
                                            unsigned min, unsigned max);
struct qcc_generator *qcc_gen_uint_less_than(struct qcc_context *ctx,
                                             unsigned value);
struct qcc_generator *qcc_gen_uint_greater_than(struct qcc_context *ctx,
                                                unsigned value);
struct qcc_generator *qcc_gen_uint_not_less_than(struct qcc_context *ctx,
                                                 unsigned value);
struct qcc_generator *qcc_gen_uint_not_greater_than(struct qcc_context *ctx,
                                                    unsigned value);
struct qcc_generator *qcc_gen_uint_not_equal_to(struct qcc_context *ctx,
                                                unsigned value);
struct qcc_generator *qcc_gen_uint_any(struct qcc_context *ctx);

unsigned qcc_generate_uint(struct qcc_generator *self, struct qcc_context *ctx);

#define qcc_rand_uint(ctx, cond, ...)                                          \
    qcc_generate_uint(qcc_gen_uint_##cond(ctx, ##__VA_ARGS__), ctx)

#define GIVEN_UINT(name, cond, ...)                                            \
    unsigned name = qcc_rand_uint(_ctx, cond, ##__VA_ARGS__);                  \
    qcc_context_register_param(_ctx, "%s: %u", #name, name);

#define GIVEN_UINT_ARRAY(name, asize, cond, ...)                               \
    struct                                                                     \
    {                                                                          \
        unsigned *data;                                                        \
        size_t size;                                                           \
    } name;                                                                    \
    qcc_generate(qcc_gen_array_of(_ctx, qcc_array_##asize,                     \
                                  qcc_gen_uint_##cond(_ctx, ##__VA_ARGS__),    \
                                  sizeof(unsigned)),                           \
                 _ctx, &name, sizeof(name));                                   \
    qcc_context_register_param(_ctx, "%s_size: %zu", #name, name.size);

#define ASSERT_UINT(got, cond, exp)                                            \
    do                                                                         \
    {                                                                          \
        if (!((got)cond(exp)))                                                 \
        {                                                                      \
            qcc_context_fail(                                                  \
                _ctx, "Assertion \"%s %s %s\" (%u %s %u) failed in "           \
                      "%s (%s, line %d)\n",                                    \
                #got, #cond, #exp, (unsigned)(got), #cond, (unsigned)(exp),    \
                __FUNCTION__, __FILE__, __LINE__);                             \
            return;                                                            \
        }                                                                      \
    } while (0)

QCC_END_HEADER
