
#pragma once

#include "qcc.h"

void *qcc_rand_ptr(struct qcc_context *ctx);
struct qcc_arena *qcc_rand_arena(struct qcc_context *ctx);
struct qcc_engine *qcc_rand_engine(struct qcc_context *ctx);
struct qcc_interval_builder *qcc_rand_interval_builder(struct qcc_context *ctx);
struct qcc_stream *qcc_rand_stream(struct qcc_context *ctx);
struct qcc_context *qcc_rand_context(struct qcc_context *ctx);

#define GIVEN_PTR(name)                                                        \
    void *name = qcc_rand_ptr(_ctx);                                           \
    qcc_context_register_param(_ctx, "%s: 0x%zx", #name, (size_t)name);

#define GIVEN_ARENA(name)                                                      \
    struct qcc_arena *name = qcc_rand_arena(_ctx);                             \
    size_t name##_size = qcc_arena_memory_available(name);                     \
    qcc_context_register_param(_ctx, "%s: qcc_arena %zu bytes", #name,         \
                               name##_size);

#define GIVEN_ENGINE(name)                                                     \
    struct qcc_engine *name = qcc_rand_engine(_ctx);                           \
    qcc_test_context_register_param(_ctx, "%s: qcc_engine", #name);

#define GIVEN_CONTEXT(name)                                                    \
    struct qcc_context *name = qcc_rand_context(_ctx);                         \
    qcc_context_register_param(_ctx, "%s: qcc_test_context", #name);

/*
 * Implementation
 */

void *qcc_rand_ptr(struct qcc_context *ctx)
{
    void *ptr;
    qcc_generate(qcc_gen_uint_any(ctx, sizeof(ptr)), &ptr);
    return ptr;
}

struct qcc_arena *qcc_rand_arena(struct qcc_context *ctx)
{
    const size_t size = 8192;
    void *data = qcc_arena_alloc(ctx->arena, size);
    QCC_ARENA_OBJ(ctx->arena, qcc_arena, result, data, size);
    return result;
}

struct qcc_engine *qcc_rand_engine(struct qcc_context *ctx)
{
    const size_t size = 8192;
    void *data = qcc_arena_alloc(ctx->arena, size);
    QCC_ARENA_POD(ctx->arena, qcc_engine, result);
    qcc_engine_init(result, data, size);
    return result;
}

struct qcc_interval_builder *qcc_rand_interval_builder(struct qcc_context *ctx)
{
    const size_t size = 1024;
    void *buffer = qcc_arena_alloc(ctx->arena, size);
    QCC_ARENA_POD(ctx->arena, qcc_interval_builder, result);
    qcc_interval_builder_init(result, buffer, size);
    return result;
}

struct qcc_stream *qcc_rand_stream(struct qcc_context *ctx)
{
    const size_t size = 256;
    qcc_uint *buffer = qcc_arena_alloc(ctx->arena, size*sizeof(qcc_uint));
    struct qcc_interval_builder *intervals = qcc_rand_interval_builder(ctx);
    QCC_ARENA_POD(ctx->arena, qcc_stream, result);
    qcc_stream_init(result, QCC_STREAM_WRITE, buffer, size, intervals);
    return result;
}

struct qcc_context *qcc_rand_context(struct qcc_context *ctx)
{
    struct qcc_stream *stream = qcc_rand_stream(ctx);
    struct qcc_arena *arena = qcc_rand_arena(ctx);
    QCC_ARENA_POD(ctx->arena, qcc_context, result);
    qcc_context_init(result, stream, arena);
    return result;
}
