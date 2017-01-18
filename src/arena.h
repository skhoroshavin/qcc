
#pragma once

#include <memory.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef void (*qcc_destroy_fn)(void *);

struct qcc_arena_obj
{
    void *ptr;
    qcc_destroy_fn dtor;
};

struct qcc_arena
{
    struct qcc_arena_obj *objects;
    size_t size;
    size_t max_size;
};

void qcc_arena_init(struct qcc_arena *arena, size_t max_size);
void qcc_arena_done(struct qcc_arena *arena);
void *qcc_arena_alloc(struct qcc_arena *arena, size_t size,
                      qcc_destroy_fn dtor);
void qcc_arena_reset(struct qcc_arena *arena);

#ifdef __cplusplus
}
#endif // __cplusplus
