
#include "test_common.h"

void *qcc_rand_ptr(struct qcc_test_context *ctx)
{
    return (void *)(size_t)qcc_rand_uint(ctx, not_equal_to, 0);
}

struct qcc_arena *qcc_rand_arena(struct qcc_test_context *ctx,
                                 unsigned min_size)
{
    unsigned size = qcc_rand_uint(ctx, in_range, min_size,
                                  qcc_arena_memory_available(ctx->arena) / 4);
    void *data = qcc_arena_alloc(ctx->arena, size);

    QCC_ARENA_OBJ(ctx->arena, qcc_arena, result, data, size);
    return result;
}

// struct qcc_engine *qcc_rand_engine(struct qcc_test_context *ctx) {}
