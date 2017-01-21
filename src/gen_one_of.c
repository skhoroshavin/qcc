
#include "gen_one_of.h"

#include <assert.h>
#include <stdarg.h>

struct qcc_generator_one_of
{
    struct qcc_generator base;
    unsigned generator_count;
    struct qcc_generator *generators;
};

static void qcc_gen_one_of_example(struct qcc_generator *self,
                                   struct qcc_test_context *ctx, void *data)
{
    struct qcc_generator_one_of *one_of = (struct qcc_generator_one_of *)self;
    unsigned index = qcc_test_context_rand(ctx) % one_of->generator_count;
    qcc_gen_example(one_of->generators + index, ctx, data);
}

struct qcc_generator *qcc_gen_one_of(struct qcc_test_context *ctx, ...)
{
    QCC_ARENA_POD(&ctx->arena, qcc_generator_one_of, res);
    res->base.example = qcc_gen_one_of_example;
    res->base.size = 0;
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
