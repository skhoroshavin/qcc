
#include "qcc_array.h"
#include <assert.h>
#include <stdlib.h>

struct qcc_array_size qcc_array_any_size = {.min = 0, .max = 16};
struct qcc_array_size qcc_array_non_empty = {.min = 1, .max = 16};

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
    qcc_generator_ptr item_gen;
};

static void qcc_generate_array_of(qcc_generator_ptr self, void *data)
{
    struct qcc_arena *arena = self->context->arena;
    struct qcc_generator_array_of *array_of =
        (struct qcc_generator_array_of *)self;
    qcc_generator_ptr item_gen = array_of->item_gen;

    struct qcc_array *array = (struct qcc_array *)data;
    array->data =
        qcc_arena_alloc(arena, item_gen->type_size * array_of->size.max);
    array->size = array_of->size.min;

    uint8_t *item = (uint8_t *)array->data;
    for (size_t i = 0; i != array_of->size.min; ++i)
    {
        qcc_generate(item_gen, item);
        item += item_gen->type_size;
    }

    while (array->size < array_of->size.max)
    {
        if ((qcc_context_rand_value(self->context) & 0x3) == 0) break;
        qcc_generate(item_gen, item);
        item += item_gen->type_size;
        ++array->size;
    }
}

qcc_generator_ptr qcc_gen_array_of(struct qcc_context *ctx,
                                   struct qcc_array_size size,
                                   qcc_generator_ptr item_gen)
{
    QCC_ARENA_POD(ctx->arena, qcc_generator_array_of, res);
    res->base.context = ctx;
    res->base.type_size = sizeof(struct qcc_array);
    res->base.generate = qcc_generate_array_of;
    res->size = size;
    res->item_gen = item_gen;
    return &res->base;
}
