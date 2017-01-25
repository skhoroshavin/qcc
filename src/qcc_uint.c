
#include "qcc_uint.h"

#include <assert.h>

struct _generator_uint_in_range
{
    struct qcc_generator base;
    unsigned min;
    unsigned max;
};

static size_t _uint_type_size(enum qcc_uint_type type)
{
    switch (type)
    {
    case QCC_UINT8:
        return 1;
    case QCC_UINT16:
        return 2;
    case QCC_UINT32:
        return 4;
    case QCC_UINT64:
        return 8;
    }
    return 0;
}

static unsigned _limit_uint(enum qcc_uint_type type, unsigned value)
{
    switch (type)
    {
    case QCC_UINT8:
        return value > UINT8_MAX ? UINT8_MAX : value;
    case QCC_UINT16:
        return value > UINT16_MAX ? UINT16_MAX : value;
    case QCC_UINT32:
        return value > UINT32_MAX ? UINT32_MAX : value;
    default:
        return value;
    }
}

static void _generate_uint_in_range(struct qcc_generator *self,
                                    struct qcc_context *ctx, void *data)
{
    unsigned min = ((struct _generator_uint_in_range *)self)->min;
    unsigned max = ((struct _generator_uint_in_range *)self)->max;

    unsigned result = qcc_context_rand_value(ctx);
    unsigned range = max - min + 1;
    if (range) result = result % range + min;

    switch (self->type_size)
    {
    case 1:
        *((uint8_t *)data) = result;
        return;
    case 2:
        *((uint16_t *)data) = result;
        return;
    case 4:
        *((uint32_t *)data) = result;
        return;
    case 8:
        *((uint64_t *)data) = result;
        return;
    }
}

static struct qcc_generator *_gen_uint_in_range(struct qcc_context *ctx,
                                                enum qcc_uint_type type,
                                                unsigned min, unsigned max)
{
    QCC_ARENA_POD(ctx->arena, _generator_uint_in_range, res);
    res->base.type_size = _uint_type_size(type);
    res->base.generate = _generate_uint_in_range;
    res->min = _limit_uint(type, min);
    res->max = _limit_uint(type, max);
    return &res->base;
}

struct qcc_generator *qcc_gen_uint_equal_to(struct qcc_context *ctx,
                                            enum qcc_uint_type type,
                                            unsigned value)
{
    return _gen_uint_in_range(ctx, type, value, value);
}

struct qcc_generator *qcc_gen_uint_in_range(struct qcc_context *ctx,
                                            enum qcc_uint_type type,
                                            unsigned min, unsigned max)
{
    min = _limit_uint(type, min);
    max = _limit_uint(type, max);
    assert(min <= max);

    if (max - min < 20) return _gen_uint_in_range(ctx, type, min, max);

    unsigned avg = min / 2 + max / 2;
    return qcc_gen_one_of(ctx, _gen_uint_in_range(ctx, type, min, min + 2),
                          _gen_uint_in_range(ctx, type, max - 2, max),
                          _gen_uint_in_range(ctx, type, avg - 1, avg + 1),
                          _gen_uint_in_range(ctx, type, min, min + 10),
                          _gen_uint_in_range(ctx, type, max - 10, max),
                          _gen_uint_in_range(ctx, type, avg - 5, avg + 5),
                          _gen_uint_in_range(ctx, type, min, max), (void *)0);
}

struct qcc_generator *qcc_gen_uint_less_than(struct qcc_context *ctx,
                                             enum qcc_uint_type type,
                                             unsigned value)
{
    return qcc_gen_uint_in_range(ctx, type, 0, value - 1);
}

struct qcc_generator *qcc_gen_uint_greater_than(struct qcc_context *ctx,
                                                enum qcc_uint_type type,
                                                unsigned value)
{
    return qcc_gen_uint_in_range(ctx, type, value + 1, UINT32_MAX);
}

struct qcc_generator *qcc_gen_uint_not_less_than(struct qcc_context *ctx,
                                                 enum qcc_uint_type type,
                                                 unsigned value)
{
    return qcc_gen_uint_in_range(ctx, type, value, UINT32_MAX);
}

struct qcc_generator *qcc_gen_uint_not_greater_than(struct qcc_context *ctx,
                                                    enum qcc_uint_type type,
                                                    unsigned value)
{
    return qcc_gen_uint_in_range(ctx, type, 0, value);
}

struct qcc_generator *qcc_gen_uint_not_equal_to(struct qcc_context *ctx,
                                                enum qcc_uint_type type,
                                                unsigned value)
{
    if (value == 0) return qcc_gen_uint_greater_than(ctx, type, 0);
    if (value == UINT32_MAX)
        return qcc_gen_uint_less_than(ctx, type, UINT32_MAX);
    return qcc_gen_one_of(ctx, qcc_gen_uint_less_than(ctx, type, value),
                          qcc_gen_uint_greater_than(ctx, type, value),
                          (void *)0);
}

struct qcc_generator *qcc_gen_uint_any(struct qcc_context *ctx,
                                       enum qcc_uint_type type)
{
    return qcc_gen_uint_in_range(ctx, type, 0, UINT32_MAX);
}

unsigned qcc_generate_uint(struct qcc_generator *self, struct qcc_context *ctx)
{
    unsigned result;
    qcc_generate(self, ctx, &result);
    return result;
}
