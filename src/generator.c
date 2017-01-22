
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

struct qcc_generator_value
{
    struct qcc_generator base;
    const void *data;
    size_t size;
};

static void qcc_generate_value(struct qcc_generator *self,
                               struct qcc_test_context *ctx, void *data,
                               size_t size)
{
    (void)ctx;
    struct qcc_generator_value *value = (struct qcc_generator_value *)self;

    assert(size == value->size);
    memcpy(data, value->data, size);
}

struct qcc_generator *qcc_gen_value(struct qcc_test_context *ctx,
                                    const void *data, size_t size)
{
    QCC_ARENA_POD(&ctx->arena, qcc_generator_value, res);
    res->base.generate = qcc_generate_value;
    res->data = qcc_arena_copy(&ctx->arena, data, size);
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

/*
 * Generator array_of
 */

struct qcc_array_params qcc_array_any()
{
    struct qcc_array_params params;
    params.min_size = 0;
    params.max_size = 64;
    return params;
}

struct qcc_array_params qcc_array_non_empty()
{
    struct qcc_array_params params;
    params.min_size = 0;
    params.max_size = 64;
    return params;
}

struct qcc_generator_array_of
{
    struct qcc_generator base;
    struct qcc_generator *elem_gen;
    size_t elem_size;
    struct qcc_array_params params;
};

static void qcc_generate_array_of(struct qcc_generator *self,
                                  struct qcc_test_context *ctx, void *data,
                                  size_t size)
{
    struct qcc_generator_array_of *array_of =
        (struct qcc_generator_array_of *)self;

    assert(size == sizeof(struct qcc_array));
    struct qcc_array *array = (struct qcc_array *)data;

    size_t min_size = array_of->params.min_size;
    size_t max_size = array_of->params.max_size;
    array->size =
        min_size + qcc_test_context_rand(ctx) % (max_size - min_size + 1);

    array->data =
        qcc_arena_alloc(&ctx->arena, array->size * array_of->elem_size);
    for (size_t i = 0; i < array->size; ++i)
    {
        void *elem_data = (char *)array->data + i * array_of->elem_size;
        qcc_generate(array_of->elem_gen, ctx, elem_data, array_of->elem_size);
    }

    array->size = 0;
}

struct qcc_generator *qcc_gen_array_of(struct qcc_test_context *ctx,
                                       struct qcc_generator *elem_gen,
                                       size_t elem_size,
                                       struct qcc_array_params params)
{
    QCC_ARENA_POD(&ctx->arena, qcc_generator_array_of, res);
    res->base.generate = qcc_generate_array_of;
    res->elem_gen = elem_gen;
    res->elem_size = elem_size;
    res->params = params;
    return &res->base;
}
