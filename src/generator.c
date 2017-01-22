
#include "generator.h"

#include <assert.h>

void qcc_generate(struct qcc_generator *self, struct qcc_test_context *ctx,
                  void *data, size_t size)
{
    self->generate(self, ctx, data, size);
}

/*
 * Generator value_from
 */

struct qcc_generator_value_from
{
    struct qcc_generator base;
    const void *data;
    size_t size;
};

static void qcc_generate_value_from(struct qcc_generator *self,
                                    struct qcc_test_context *ctx, void *data,
                                    size_t size)
{
    struct qcc_generator_value_from *value_of =
        (struct qcc_generator_value_from *)self;

    size_t count = value_of->size / size;
    assert(count * size == value_of->size);

    size_t index = qcc_test_context_rand(ctx) % count;
    memcpy(data, (char *)value_of->data + index * size, size);
}

struct qcc_generator *qcc_gen_value_from(struct qcc_test_context *ctx,
                                         const void *data, size_t size)
{
    QCC_ARENA_POD(&ctx->arena, qcc_generator_value_from, res);
    res->base.generate = qcc_generate_value_from;
    res->data = data;
    res->size = size;
    return &res->base;
}

/*
 * Generator one_of
 */

struct qcc_generator_one_of
{
    struct qcc_generator base;
    unsigned generator_count;
    struct qcc_generator *generators;
};

static void qcc_generate_one_of(struct qcc_generator *self,
                                struct qcc_test_context *ctx, void *data,
                                size_t size)
{
    struct qcc_generator_one_of *one_of = (struct qcc_generator_one_of *)self;
    unsigned index = qcc_test_context_rand(ctx) % one_of->generator_count;
    qcc_generate(one_of->generators + index, ctx, data, size);
}

struct qcc_generator *qcc_gen_one_of(struct qcc_test_context *ctx, ...)
{
    QCC_ARENA_POD(&ctx->arena, qcc_generator_one_of, res);
    res->base.generate = qcc_generate_one_of;
    res->generator_count = 0;

    va_list args;
    va_start(args, ctx);
    qcc_arena_begin_array(&ctx->arena);
    while (1)
    {
        struct qcc_generator *gen = va_arg(args, struct qcc_generator *);
        if (!gen) break;

        ++res->generator_count;
        qcc_arena_append_array(&ctx->arena, &gen, sizeof(gen));
    }
    va_end(args);
    res->generators = qcc_arena_end_array(&ctx->arena);

    return &res->base;
}
