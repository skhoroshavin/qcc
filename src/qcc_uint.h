
#pragma once

#include "qcc_array.h"

QCC_BEGIN_HEADER

typedef uint64_t qcc_uint;
#define QCC_UINT_MAX UINT64_MAX

enum qcc_uint_type
{
    QCC_UINT8,
    QCC_UINT16,
    QCC_UINT32,
    QCC_UINT64
};

struct qcc_generator *qcc_gen_uint_equal_to(struct qcc_context *ctx,
                                            enum qcc_uint_type type,
                                            qcc_uint value);
struct qcc_generator *qcc_gen_uint_in_range(struct qcc_context *ctx,
                                            enum qcc_uint_type type,
                                            qcc_uint min, qcc_uint max);
struct qcc_generator *qcc_gen_uint_less_than(struct qcc_context *ctx,
                                             enum qcc_uint_type type,
                                             qcc_uint value);
struct qcc_generator *qcc_gen_uint_greater_than(struct qcc_context *ctx,
                                                enum qcc_uint_type type,
                                                qcc_uint value);
struct qcc_generator *qcc_gen_uint_not_less_than(struct qcc_context *ctx,
                                                 enum qcc_uint_type type,
                                                 qcc_uint value);
struct qcc_generator *qcc_gen_uint_not_greater_than(struct qcc_context *ctx,
                                                    enum qcc_uint_type type,
                                                    qcc_uint value);
struct qcc_generator *qcc_gen_uint_not_equal_to(struct qcc_context *ctx,
                                                enum qcc_uint_type type,
                                                qcc_uint value);
struct qcc_generator *qcc_gen_uint_any(struct qcc_context *ctx,
                                       enum qcc_uint_type type);

unsigned qcc_generate_uint(struct qcc_generator *self, struct qcc_context *ctx);
uint64_t qcc_generate_uint64(struct qcc_generator *self,
                             struct qcc_context *ctx);

#define qcc_rand_uint(ctx, cond, ...)                                          \
    qcc_generate_uint(qcc_gen_uint_##cond(ctx, QCC_UINT32, ##__VA_ARGS__), ctx)

#define qcc_rand_uint64(ctx, cond, ...)                                        \
    qcc_generate_uint64(qcc_gen_uint_##cond(ctx, QCC_UINT64, ##__VA_ARGS__),   \
                        ctx)

#define GIVEN_UINT(name, cond, ...)                                            \
    unsigned name = qcc_rand_uint(_ctx, cond, ##__VA_ARGS__);                  \
    qcc_context_register_param(_ctx, "%s: %u", #name, name);

#define GIVEN_UINT64(name, cond, ...)                                          \
    unsigned name = qcc_rand_uint64(_ctx, cond, ##__VA_ARGS__);                \
    qcc_context_register_param(_ctx, "%s: %ul", #name, name);

#define GIVEN_UINT_ARRAY(name, asize, cond, ...)                               \
    struct                                                                     \
    {                                                                          \
        unsigned *data;                                                        \
        size_t size;                                                           \
    } name;                                                                    \
    qcc_generate(qcc_gen_array_of(                                             \
                     _ctx, qcc_array_##asize,                                  \
                     qcc_gen_uint_##cond(_ctx, QCC_UINT32, ##__VA_ARGS__)),    \
                 _ctx, &name);                                                 \
    qcc_context_register_param(_ctx, "%s_size: %zu", #name, name.size);

#define GIVEN_UINT64_ARRAY(name, asize, cond, ...)                             \
    struct                                                                     \
    {                                                                          \
        uint64_t *data;                                                        \
        size_t size;                                                           \
    } name;                                                                    \
    qcc_generate(qcc_gen_array_of(                                             \
                     _ctx, qcc_array_##asize,                                  \
                     qcc_gen_uint_##cond(_ctx, QCC_UINT64, ##__VA_ARGS__)),    \
                 _ctx, &name);                                                 \
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

#define ASSERT_UINT64(got, cond, exp)                                          \
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
