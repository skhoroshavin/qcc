
#include "generator.h"

#include <assert.h>

/*
 * Generator
 */

void qcc_generate(qcc_generator_ptr self, void *data)
{
    qcc_stream_begin(self->context->stream);
    self->generate(self, data);
    qcc_stream_end(self->context->stream);
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

static void qcc_generate_value_from(qcc_generator_ptr self, void *data)
{
    struct qcc_generator_value_from *value_from =
        (struct qcc_generator_value_from *)self;

    size_t index = qcc_stream_get(self->context->stream) % value_from->count;
    memcpy(data, value_from->data + index * value_from->stride,
           self->type_size);
}

qcc_generator_ptr qcc_gen_value_from(struct qcc_context *ctx, size_t item_size,
                                     size_t item_stride, const void *data,
                                     size_t count)
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
    qcc_generator_ptr *generators;
};

static void qcc_generate_one_of(qcc_generator_ptr self, void *data)
{
    struct qcc_generator_one_of *one_of = (struct qcc_generator_one_of *)self;

    size_t index =
        qcc_stream_get(self->context->stream) % one_of->generator_count;
    qcc_generate(one_of->generators[index], data);
}

qcc_generator_ptr qcc_gen_one_of(struct qcc_context *ctx, ...)
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
        qcc_generator_ptr gen = va_arg(args, qcc_generator_ptr);
        if (!gen) break;

        if (res->base.type_size)
            assert(res->base.type_size == gen->type_size);
        else
            res->base.type_size = gen->type_size;

        ++res->generator_count;
        qcc_arena_append_array(ctx->arena, &gen, sizeof(gen));
    }
    va_end(args);
    res->generators = (qcc_generator_ptr *)qcc_arena_end_array(ctx->arena);

    return &res->base;
}

/*
 * Generator transform
 */

struct qcc_generator_transform
{
    struct qcc_generator base;
    qcc_generator_ptr src_gen;
    qcc_transform_fn transform;
    const void *params;
};

static void qcc_generate_transform(qcc_generator_ptr self, void *data)
{
    struct qcc_generator_transform *transform =
        (struct qcc_generator_transform *)self;

    uint8_t tmp[128];
    assert(transform->src_gen->type_size <= 128);
    qcc_generate(transform->src_gen, tmp);
    transform->transform(transform->params, tmp, transform->src_gen->type_size,
                         data, self->type_size);
}

qcc_generator_ptr qcc_gen_transform(struct qcc_context *ctx, size_t item_size,
                                    qcc_generator_ptr src_gen,
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
