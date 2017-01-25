
#include "generator.h"

#include <assert.h>

void qcc_generate(struct qcc_generator *self, struct qcc_context *ctx,
                  void *data)
{
    self->generate(self, ctx, data);
}

/*
 * Generator value_from
 */

struct qcc_generator_value_from
{
    struct qcc_generator base;
    const void *data;
    size_t count;
};

static void qcc_generate_value_from(struct qcc_generator *self,
                                    struct qcc_context *ctx, void *data)
{
    struct qcc_generator_value_from *value_from =
        (struct qcc_generator_value_from *)self;

    size_t index = qcc_context_rand_value(ctx) % value_from->count;
    memcpy(data, (char *)value_from->data + index * self->type_size,
           self->type_size);
}

struct qcc_generator *qcc_gen_value_from(struct qcc_context *ctx,
                                         size_t item_size, const void *data,
                                         size_t count)
{
    QCC_ARENA_POD(ctx->arena, qcc_generator_value_from, res);
    res->base.type_size = item_size;
    res->base.generate = qcc_generate_value_from;
    res->data = data;
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

static void qcc_generate_one_of(struct qcc_generator *self,
                                struct qcc_context *ctx, void *data)
{
    struct qcc_generator_one_of *one_of = (struct qcc_generator_one_of *)self;
    unsigned index = qcc_context_rand_value(ctx) % one_of->generator_count;
    qcc_generate(one_of->generators[index], ctx, data);
}

struct qcc_generator *qcc_gen_one_of(struct qcc_context *ctx, ...)
{
    QCC_ARENA_POD(ctx->arena, qcc_generator_one_of, res);
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
    res->generators = qcc_arena_end_array(ctx->arena);

    return &res->base;
}
