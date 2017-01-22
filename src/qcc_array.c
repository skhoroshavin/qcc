
#include "qcc_array.h"
#include <assert.h>

struct qcc_array_size qcc_array_any()
{
    struct qcc_array_size res;
    res.min = 0;
    res.max = 64;
    return res;
}

struct qcc_array_size qcc_array_non_empty()
{
    struct qcc_array_size res;
    res.min = 1;
    res.max = 64;
    return res;
}

struct qcc_array_size qcc_array_fixed_size(size_t size)
{
    struct qcc_array_size res;
    res.min = size;
    res.max = size;
    return res;
}

struct qcc_generator_array_of
{
    struct qcc_generator base;
    struct qcc_array_size size;
    struct qcc_generator *elem_gen;
    size_t elem_size;
};

static void qcc_generate_array_of(struct qcc_generator *self,
                                  struct qcc_test_context *ctx, void *data,
                                  size_t size)
{
    struct qcc_generator_array_of *array_of =
        (struct qcc_generator_array_of *)self;

    assert(size == sizeof(struct qcc_array)); // LCOV_EXCL_BR_LINE
    struct qcc_array *array = (struct qcc_array *)data;

    array->size = array_of->size.min +
                  qcc_test_context_rand_value(ctx) %
                      (array_of->size.max - array_of->size.min + 1);

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
                                       struct qcc_array_size size,
                                       struct qcc_generator *elem_gen,
                                       size_t elem_size)
{
    QCC_ARENA_POD(&ctx->arena, qcc_generator_array_of, res);
    res->base.generate = qcc_generate_array_of;
    res->size = size;
    res->elem_gen = elem_gen;
    res->elem_size = elem_size;
    return &res->base;
}
