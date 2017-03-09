
#pragma once

#include "arena.h"
#include "qcc_array.h"

QCC_BEGIN_HEADER

struct qcc_generator *qcc_gen_uint_equal_to(struct qcc_context *ctx,
                                            size_t size, qcc_uint value);
struct qcc_generator *qcc_gen_uint_in_range(struct qcc_context *ctx,
                                            size_t size, qcc_uint min,
                                            qcc_uint max);
struct qcc_generator *qcc_gen_uint_less_than(struct qcc_context *ctx,
                                             size_t size, qcc_uint value);
struct qcc_generator *qcc_gen_uint_greater_than(struct qcc_context *ctx,
                                                size_t size, qcc_uint value);
struct qcc_generator *qcc_gen_uint_not_less_than(struct qcc_context *ctx,
                                                 size_t size, qcc_uint value);
struct qcc_generator *qcc_gen_uint_not_greater_than(struct qcc_context *ctx,
                                                    size_t size,
                                                    qcc_uint value);
struct qcc_generator *qcc_gen_uint_not_equal_to(struct qcc_context *ctx,
                                                size_t size, qcc_uint value);
struct qcc_generator *qcc_gen_uint_any(struct qcc_context *ctx, size_t size);

qcc_uint qcc_generate_uint(struct qcc_generator *self, struct qcc_context *ctx);

#define qcc_rand_uint(ctx, cond, ...)                                          \
    qcc_generate_uint(                                                         \
        qcc_gen_uint_##cond(ctx, sizeof(qcc_uint), ##__VA_ARGS__), ctx)

#define GIVEN_UNSIGNED(type, name, cond, ...)                                  \
    type name;                                                                 \
    qcc_generate(qcc_gen_uint_##cond(_ctx, sizeof(type), ##__VA_ARGS__), _ctx, \
                 &name);                                                       \
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
                 _ctx, &name);                                                 \
    qcc_context_register_param(_ctx, "%s_size: %zu", #name, name.size);

#define GIVEN_UINT(name, cond, ...)                                            \
    GIVEN_UNSIGNED(unsigned, name, cond, ##__VA_ARGS__);

#define GIVEN_UINT8(name, cond, ...)                                           \
    GIVEN_UNSIGNED(uint8_t, name, cond, ##__VA_ARGS__);

#define GIVEN_UINT16(name, cond, ...)                                          \
    GIVEN_UNSIGNED(uint16_t, name, cond, ##__VA_ARGS__);

#define GIVEN_UINT32(name, cond, ...)                                          \
    GIVEN_UNSIGNED(uint32_t, name, cond, ##__VA_ARGS__);

#define GIVEN_UINT64(name, cond, ...)                                          \
    GIVEN_UNSIGNED(uint64_t, name, cond, ##__VA_ARGS__);

#define GIVEN_UINT_ARRAY(name, asize, cond, ...)                               \
    GIVEN_UNSIGNED_ARRAY(unsigned, name, asize, cond, ##__VA_ARGS__)

#define GIVEN_UINT8_ARRAY(name, asize, cond, ...)                              \
    GIVEN_UNSIGNED_ARRAY(uint8_t, name, asize, cond, ##__VA_ARGS__)

#define GIVEN_UINT16_ARRAY(name, asize, cond, ...)                             \
    GIVEN_UNSIGNED_ARRAY(uint16_t, name, asize, cond, ##__VA_ARGS__)

#define GIVEN_UINT32_ARRAY(name, asize, cond, ...)                             \
    GIVEN_UNSIGNED_ARRAY(uint32_t, name, asize, cond, ##__VA_ARGS__)

#define GIVEN_UINT64_ARRAY(name, asize, cond, ...)                             \
    GIVEN_UNSIGNED_ARRAY(uint64_t, name, asize, cond, ##__VA_ARGS__)

#define ASSERT_UINT(got, cond, exp)                                            \
    do                                                                         \
    {                                                                          \
        qcc_uint got_val = got;                                                \
        qcc_uint exp_val = exp;                                                \
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
