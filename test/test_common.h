
#pragma once

#include "qcc.h"

void *qcc_rand_ptr(struct qcc_context *ctx);
struct qcc_arena *qcc_rand_arena(struct qcc_context *ctx, unsigned min_size);
struct qcc_engine *qcc_rand_engine(struct qcc_context *ctx);
struct qcc_context *qcc_rand_context(struct qcc_context *ctx);

#define GIVEN_PTR(name)                                                        \
    void *name = qcc_rand_ptr(_ctx);                                           \
    qcc_context_register_param(_ctx, "%s: 0x%zx", #name, (size_t)name);

#define GIVEN_ARENA(name)                                                      \
    struct qcc_arena *name =                                                   \
        qcc_rand_arena(_ctx, 2 * sizeof(struct qcc_arena_object));             \
    size_t name##_size = qcc_arena_memory_available(name);                     \
    qcc_context_register_param(_ctx, "%s: qcc_arena %zu bytes", #name,         \
                               name##_size);

#define GIVEN_ENGINE(name)                                                     \
    struct qcc_engine *name = qcc_rand_engine(_ctx);                           \
    qcc_test_context_register_param(_ctx, "%s: qcc_engine", #name);

#define GIVEN_CONTEXT(name)                                                    \
    struct qcc_context *name = qcc_rand_context(_ctx);                         \
    qcc_context_register_param(_ctx, "%s: qcc_test_context", #name);
