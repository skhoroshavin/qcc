
#include "qcc_uint.h"

static unsigned _qcc_uint_in_range(struct qcc_test_context *ctx, unsigned min,
                                   unsigned max)
{
    unsigned result = qcc_test_context_rand(ctx);
    unsigned range = max - min + 1;
    if (range) result = result % range + min;
    return result;
}

struct qcc_test_gen_uint_in_range
{
    struct qcc_generator base;
    unsigned min;
    unsigned max;
};

static void qcc_gen_uint_in_range_example(struct qcc_generator *self,
                                          struct qcc_test_context *ctx,
                                          void *data)
{
    unsigned min = ((struct qcc_test_gen_uint_in_range *)self)->min;
    unsigned max = ((struct qcc_test_gen_uint_in_range *)self)->max;
    unsigned *res = (unsigned *)data;

    unsigned avg = min / 2 + max / 2;
    if (avg < min) avg = min;
    if (avg > max) avg = max;

    switch (qcc_test_context_rand(ctx) % 10)
    {
    case 0:
        *res = min;
        return;
    case 1:
        *res = max;
        return;
    case 2:
        *res = min < max ? min + 1 : min;
        return;
    case 3:
        *res = min < max ? max - 1 : max;
        return;
    case 4:
        *res = avg;
        return;
    case 5:
        *res = avg > min ? avg - 1 : avg;
        return;
    case 6:
        *res = avg < max ? avg + 1 : avg;
        return;
    }

    *res = _qcc_uint_in_range(ctx, min, max);
}

struct qcc_generator *qcc_gen_uint_in_range(struct qcc_test_context *ctx,
                                            unsigned min, unsigned max)
{
    QCC_ARENA_POD(&ctx->arena, qcc_test_gen_uint_in_range, res);
    res->base.size = sizeof(unsigned);
    res->base.example = (qcc_gen_example_fn)qcc_gen_uint_in_range_example;
    res->min = min;
    res->max = max;
    return &res->base;
}

struct qcc_generator *qcc_gen_uint_any(struct qcc_test_context *ctx)
{
    return qcc_gen_uint_in_range(ctx, 0, UINT32_MAX);
}

struct qcc_generator *qcc_gen_uint_less_than(struct qcc_test_context *ctx,
                                             unsigned max)
{
    return qcc_gen_uint_in_range(ctx, 0, max - 1);
}

struct qcc_generator *qcc_gen_uint_not_less_than(struct qcc_test_context *ctx,
                                                 unsigned min)
{
    return qcc_gen_uint_in_range(ctx, min, UINT32_MAX);
}

struct qcc_generator *qcc_gen_uint_greater_than(struct qcc_test_context *ctx,
                                                unsigned min)
{
    return qcc_gen_uint_in_range(ctx, min + 1, UINT32_MAX);
}

struct qcc_generator *
qcc_gen_uint_not_greater_than(struct qcc_test_context *ctx, unsigned max)
{
    return qcc_gen_uint_in_range(ctx, 0, max - 1);
}
