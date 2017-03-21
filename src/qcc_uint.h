
#pragma once

#include "arena.h"
#include "qcc_array.h"

QCC_BEGIN_HEADER

qcc_generator_ptr qcc_gen_uint_from_array(struct qcc_context *ctx, size_t size,
                                          size_t src_size, const void *data,
                                          size_t count);
qcc_generator_ptr qcc_gen_uint_equal_to(struct qcc_context *ctx, size_t size,
                                        qcc_uint value);
qcc_generator_ptr qcc_gen_uint_in_range(struct qcc_context *ctx, size_t size,
                                        qcc_uint min, qcc_uint max);
qcc_generator_ptr qcc_gen_uint_less_than(struct qcc_context *ctx, size_t size,
                                         qcc_uint value);
qcc_generator_ptr qcc_gen_uint_greater_than(struct qcc_context *ctx,
                                            size_t size, qcc_uint value);
qcc_generator_ptr qcc_gen_uint_not_less_than(struct qcc_context *ctx,
                                             size_t size, qcc_uint value);
qcc_generator_ptr qcc_gen_uint_not_greater_than(struct qcc_context *ctx,
                                                size_t size, qcc_uint value);
qcc_generator_ptr qcc_gen_uint_not_equal_to(struct qcc_context *ctx,
                                            size_t size, qcc_uint value);
qcc_generator_ptr qcc_gen_uint_any(struct qcc_context *ctx, size_t size);

qcc_uint qcc_generate_uint(qcc_generator_ptr gen);

#define qcc_rand_unsigned(ctx, type, cond, ...)                                \
    (type) qcc_generate_uint(                                                  \
        qcc_gen_uint_##cond(ctx, sizeof(type), ##__VA_ARGS__))

#define GIVEN_UNSIGNED(type, name, cond, ...)                                  \
    type name = qcc_rand_unsigned(_ctx, type, cond, ##__VA_ARGS__);            \
    qcc_context_register_param(_ctx, "%s: %" PRIu64, #name, (uint64_t)name);

#define GIVEN_UNSIGNED_ARRAY(type, name, asize, cond, ...)                     \
    struct                                                                     \
    {                                                                          \
        type *data;                                                            \
        size_t size;                                                           \
    } name;                                                                    \
    qcc_generate(qcc_gen_array_of(                                             \
                     _ctx, qcc_array_##asize,                                  \
                     qcc_gen_uint_##cond(_ctx, sizeof(type), ##__VA_ARGS__)),  \
                 &name);                                                       \
    qcc_context_register_param(_ctx, "%s_size: %zu", #name, name.size);

#define qcc_rand_uint(ctx, cond, ...)                                          \
    qcc_rand_unsigned(ctx, unsigned, cond, ##__VA_ARGS__)

#define GIVEN_UINT(name, cond, ...)                                            \
    GIVEN_UNSIGNED(unsigned, name, cond, ##__VA_ARGS__);

#define GIVEN_UINT_ARRAY(name, asize, cond, ...)                               \
    GIVEN_UNSIGNED_ARRAY(unsigned, name, asize, cond, ##__VA_ARGS__)

#define ASSERT_UNSIGNED(got, cond, exp)                                        \
    do                                                                         \
    {                                                                          \
        qcc_uint got_val = (qcc_uint)got;                                      \
        qcc_uint exp_val = (qcc_uint)exp;                                      \
        if (!(got_val cond exp_val))                                           \
        {                                                                      \
            qcc_context_fail(_ctx, "Assertion \"%s %s %s\" (%" PRIu64          \
                                   " %s %" PRIu64 ") failed in "               \
                                   "%s (%s, line %d)\n",                       \
                             #got, #cond, #exp, got_val, #cond, exp_val,       \
                             __FUNCTION__, __FILE__, __LINE__);                \
            return;                                                            \
        }                                                                      \
    } while (0)

QCC_END_HEADER
