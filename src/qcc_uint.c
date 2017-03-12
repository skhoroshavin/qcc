
#include "qcc_uint.h"

#include <assert.h>
#include <inttypes.h>

struct _generator_uint_in_range
{
    struct qcc_generator base;
    qcc_uint min;
    qcc_uint max;
};

static qcc_uint _limit_uint(size_t size, qcc_uint value)
{
    return value > qcc_uint_max(size) ? qcc_uint_max(size) : value;
}

static void _generate_uint_in_range(struct qcc_generator *self, void *data)
{
    qcc_uint min = ((struct _generator_uint_in_range *)self)->min;
    qcc_uint max = ((struct _generator_uint_in_range *)self)->max;

    qcc_uint result;
    qcc_context_rand(self->context, &result, sizeof(result));
    qcc_uint range = max - min + 1;
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
                                                size_t size, qcc_uint min,
                                                qcc_uint max)
{
    QCC_ARENA_POD(ctx->arena, _generator_uint_in_range, res);
    res->base.context = ctx;
    res->base.type_size = size;
    res->base.generate = _generate_uint_in_range;
    res->min = _limit_uint(size, min);
    res->max = _limit_uint(size, max);
    return &res->base;
}

struct qcc_generator *qcc_gen_uint_equal_to(struct qcc_context *ctx,
                                            size_t size, qcc_uint value)
{
    return _gen_uint_in_range(ctx, size, value, value);
}

struct qcc_generator *qcc_gen_uint_in_range(struct qcc_context *ctx,
                                            size_t size, qcc_uint min,
                                            qcc_uint max)
{
    min = _limit_uint(size, min);
    max = _limit_uint(size, max);
    assert(min <= max);

    if (max - min < 20) return _gen_uint_in_range(ctx, size, min, max);

    qcc_uint avg = min / 2 + max / 2;
    return qcc_gen_one_of(ctx, _gen_uint_in_range(ctx, size, min, min + 2),
                          _gen_uint_in_range(ctx, size, max - 2, max),
                          _gen_uint_in_range(ctx, size, avg - 1, avg + 1),
                          _gen_uint_in_range(ctx, size, min, min + 10),
                          _gen_uint_in_range(ctx, size, max - 10, max),
                          _gen_uint_in_range(ctx, size, avg - 5, avg + 5),
                          _gen_uint_in_range(ctx, size, min, max), (void *)0);
}

struct qcc_generator *qcc_gen_uint_less_than(struct qcc_context *ctx,
                                             size_t size, qcc_uint value)
{
    return qcc_gen_uint_in_range(ctx, size, 0, value - 1);
}

struct qcc_generator *qcc_gen_uint_greater_than(struct qcc_context *ctx,
                                                size_t size, qcc_uint value)
{
    return qcc_gen_uint_in_range(ctx, size, value + 1, QCC_UINT_MAX);
}

struct qcc_generator *qcc_gen_uint_not_less_than(struct qcc_context *ctx,
                                                 size_t size, qcc_uint value)
{
    return qcc_gen_uint_in_range(ctx, size, value, QCC_UINT_MAX);
}

struct qcc_generator *qcc_gen_uint_not_greater_than(struct qcc_context *ctx,
                                                    size_t size, qcc_uint value)
{
    return qcc_gen_uint_in_range(ctx, size, 0, value);
}

struct qcc_generator *qcc_gen_uint_not_equal_to(struct qcc_context *ctx,
                                                size_t size, qcc_uint value)
{
    if (value == 0) return qcc_gen_uint_greater_than(ctx, size, 0);
    if (value == qcc_uint_max(size))
        return qcc_gen_uint_less_than(ctx, size, qcc_uint_max(size));
    return qcc_gen_one_of(ctx, qcc_gen_uint_less_than(ctx, size, value),
                          qcc_gen_uint_greater_than(ctx, size, value),
                          (void *)0);
}

struct qcc_generator *qcc_gen_uint_any(struct qcc_context *ctx, size_t size)
{
    return qcc_gen_uint_in_range(ctx, size, 0, QCC_UINT_MAX);
}

qcc_uint qcc_generate_uint(struct qcc_generator *gen)
{
    switch (gen->type_size)
    {
    case 1:
    {
        uint8_t res;
        qcc_generate(gen, &res);
        return res;
    }
    case 2:
    {
        uint16_t res;
        qcc_generate(gen, &res);
        return res;
    }
    case 4:
    {
        uint32_t res;
        qcc_generate(gen, &res);
        return res;
    }
    case 8:
    {
        uint64_t res;
        qcc_generate(gen, &res);
        return res;
    }
    }
    /* TODO: Raise error */
    return 0;
}
