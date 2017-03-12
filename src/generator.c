
#include "generator.h"

#include <assert.h>

void qcc_generate(struct qcc_generator *self, void *data)
{
    self->generate(self, data);
}

/*
 * Generator value_from
 */

struct qcc_generator_value_from
{
    struct qcc_generator base;
    size_t stride;
    const uint8_t *data;
    size_t count;
};

static void qcc_generate_value_from(struct qcc_generator *self, void *data)
{
    struct qcc_generator_value_from *value_from =
        (struct qcc_generator_value_from *)self;

    size_t index = qcc_context_rand_value(self->context) % value_from->count;
    memcpy(data, value_from->data + index * value_from->stride,
           self->type_size);
}

struct qcc_generator *qcc_gen_value_from(struct qcc_context *ctx,
                                         size_t item_size, size_t item_stride,
                                         const void *data, size_t count)
{
    QCC_ARENA_POD(ctx->arena, qcc_generator_value_from, res);
    res->base.context = ctx;
    res->base.type_size = item_size;
    res->base.generate = qcc_generate_value_from;
    res->stride = item_stride;
    res->data = (const uint8_t *)data;
    res->count = count;
    return &res->base;
}

/*
 * Generator one_of
 */

struct qcc_generator_one_of
{
    struct qcc_generator base;
    unsigned generator_count;
    struct qcc_generator **generators;
};

static void qcc_generate_one_of(struct qcc_generator *self, void *data)
{
    struct qcc_generator_one_of *one_of = (struct qcc_generator_one_of *)self;
    unsigned index =
        qcc_context_rand_value(self->context) % one_of->generator_count;
    qcc_generate(one_of->generators[index], data);
}

struct qcc_generator *qcc_gen_one_of(struct qcc_context *ctx, ...)
{
    QCC_ARENA_POD(ctx->arena, qcc_generator_one_of, res);
    res->base.context = ctx;
    res->base.type_size = 0;
    res->base.generate = qcc_generate_one_of;
    res->generator_count = 0;

    va_list args;
    va_start(args, ctx);
    qcc_arena_begin_array(ctx->arena);
    while (1)
    {
        struct qcc_generator *gen = va_arg(args, struct qcc_generator *);
        if (!gen) break;

        if (res->base.type_size)
            assert(res->base.type_size == gen->type_size);
        else
            res->base.type_size = gen->type_size;

        ++res->generator_count;
        qcc_arena_append_array(ctx->arena, &gen, sizeof(gen));
    }
    va_end(args);
    res->generators = (struct qcc_generator **)qcc_arena_end_array(ctx->arena);

    return &res->base;
}

/*
 * Generator transform
 */

struct qcc_generator_transform
{
    struct qcc_generator base;
    struct qcc_generator *src_gen;
    qcc_transform_fn transform;
    const void *params;
};

static void qcc_generate_transform(struct qcc_generator *self, void *data)
{
    struct qcc_generator_transform *transform =
        (struct qcc_generator_transform *)self;

    uint8_t tmp[128];
    assert(transform->src_gen->type_size <= 128);
    qcc_generate(transform->src_gen, tmp);
    transform->transform(transform->params, tmp, data);
}

struct qcc_generator *qcc_gen_transform(struct qcc_context *ctx,
                                        size_t item_size,
                                        struct qcc_generator *src_gen,
                                        qcc_transform_fn transform,
                                        const void *params)
{
    QCC_ARENA_POD(ctx->arena, qcc_generator_transform, res);
    res->base.context = ctx;
    res->base.type_size = item_size;
    res->base.generate = qcc_generate_transform;
    res->src_gen = src_gen;
    res->transform = transform;
    res->params = params;
    return &res->base;
}
