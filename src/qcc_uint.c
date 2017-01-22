
#include "qcc_uint.h"

#include <assert.h>

struct _generator_uint_in_range
{
    struct qcc_generator base;
    unsigned min;
    unsigned max;
};

static void _generate_uint_in_range(struct qcc_generator *self,
                                    struct qcc_test_context *ctx, void *data,
                                    size_t size)
{
    assert(size == sizeof(unsigned)); // LCOV_EXCL_BR_LINE

    unsigned min = ((struct _generator_uint_in_range *)self)->min;
    unsigned max = ((struct _generator_uint_in_range *)self)->max;
    unsigned *res = (unsigned *)data;

    unsigned result = qcc_test_context_rand_value(ctx);
    unsigned range = max - min + 1;
    if (range) result = result % range + min;
    *res = result;
}

static struct qcc_generator *_gen_uint_in_range(struct qcc_test_context *ctx,
                                                unsigned min, unsigned max)
{
    QCC_ARENA_POD(ctx->arena, _generator_uint_in_range, res);
    res->base.generate = _generate_uint_in_range;
    res->min = min;
    res->max = max;
    return &res->base;
}

struct qcc_generator *qcc_gen_uint_equal_to(struct qcc_test_context *ctx,
                                            unsigned value)
{
    return qcc_gen_value_from(
        ctx, qcc_arena_copy(ctx->arena, &value, sizeof(value)), sizeof(value));
}

struct qcc_generator *qcc_gen_uint_in_range(struct qcc_test_context *ctx,
                                            unsigned min, unsigned max)
{
    assert(min <= max); // LCOV_EXCL_BR_LINE

    if (max - min < 20) return _gen_uint_in_range(ctx, min, max);

    unsigned avg = min / 2 + max / 2;
    return qcc_gen_one_of(ctx, _gen_uint_in_range(ctx, min, min + 2),
                          _gen_uint_in_range(ctx, max - 2, max),
                          _gen_uint_in_range(ctx, avg - 1, avg + 1),
                          _gen_uint_in_range(ctx, min, min + 10),
                          _gen_uint_in_range(ctx, max - 10, max),
                          _gen_uint_in_range(ctx, avg - 5, avg + 5),
                          _gen_uint_in_range(ctx, min, max), 0);
}

struct qcc_generator *qcc_gen_uint_less_than(struct qcc_test_context *ctx,
                                             unsigned value)
{
    return qcc_gen_uint_in_range(ctx, 0, value - 1);
}

struct qcc_generator *qcc_gen_uint_greater_than(struct qcc_test_context *ctx,
                                                unsigned value)
{
    return qcc_gen_uint_in_range(ctx, value + 1, UINT32_MAX);
}

struct qcc_generator *qcc_gen_uint_not_less_than(struct qcc_test_context *ctx,
                                                 unsigned value)
{
    return qcc_gen_uint_in_range(ctx, value, UINT32_MAX);
}

struct qcc_generator *
qcc_gen_uint_not_greater_than(struct qcc_test_context *ctx, unsigned value)
{
    return qcc_gen_uint_in_range(ctx, 0, value);
}

struct qcc_generator *qcc_gen_uint_not_equal_to(struct qcc_test_context *ctx,
                                                unsigned value)
{
    if (value == 0) return qcc_gen_uint_greater_than(ctx, 0);
    if (value == UINT32_MAX) return qcc_gen_uint_less_than(ctx, UINT32_MAX);
    return qcc_gen_one_of(ctx, qcc_gen_uint_less_than(ctx, value),
                          qcc_gen_uint_greater_than(ctx, value), 0);
}

struct qcc_generator *qcc_gen_uint_any(struct qcc_test_context *ctx)
{
    return qcc_gen_uint_in_range(ctx, 0, UINT32_MAX);
}
