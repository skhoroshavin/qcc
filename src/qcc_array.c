
#include "qcc_array.h"
#include <assert.h>

struct qcc_array_size qcc_array_any_size = {.min = 0, .max = 64};
struct qcc_array_size qcc_array_non_empty = {.min = 1, .max = 64};

struct qcc_array_size qcc_array_fixed_size(size_t size)
{
    struct qcc_array_size res;
    res.min = size;
    res.max = size;
    return res;
}

struct qcc_array_size qcc_array_smaller_than(size_t size)
{
    struct qcc_array_size res;
    res.min = 0;
    res.max = size;
    return res;
}

struct qcc_generator_array_of
{
    struct qcc_generator base;
    struct qcc_array_size size;
    struct qcc_generator *item_gen;
    size_t item_size;
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
        qcc_arena_alloc(ctx->arena, array->size * array_of->item_size);
    for (size_t i = 0; i < array->size; ++i)
    {
        void *item_data = (char *)array->data + i * array_of->item_size;
        qcc_generate(array_of->item_gen, ctx, item_data, array_of->item_size);
    }
}

struct qcc_generator *qcc_gen_array_of(struct qcc_test_context *ctx,
                                       struct qcc_array_size size,
                                       struct qcc_generator *item_gen,
                                       size_t item_size)
{
    QCC_ARENA_POD(ctx->arena, qcc_generator_array_of, res);
    res->base.generate = qcc_generate_array_of;
    res->size = size;
    res->item_gen = item_gen;
    res->item_size = item_size;
    return &res->base;
}
