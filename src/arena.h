
#pragma once

#include <memory.h>
#include <stdarg.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef void (*qcc_destroy_fn)(void *);
struct qcc_arena_object
{
    void *ptr;
    qcc_destroy_fn dtor;
};

struct qcc_arena
{
    uint8_t *buffer_start;
    uint8_t *buffer_end;
    uint8_t *available_memory;
    struct qcc_arena_object *objects;
};

void qcc_arena_init(struct qcc_arena *arena, size_t max_size);
void qcc_arena_done(struct qcc_arena *arena);
size_t qcc_arena_memory_available(const struct qcc_arena *arena);
void *qcc_arena_alloc(struct qcc_arena *arena, size_t size);
unsigned qcc_arena_add_object(struct qcc_arena *arena, void *ptr,
                              qcc_destroy_fn dtor);
const char *qcc_arena_vsprintf(struct qcc_arena *arena, const char *fmt,
                               va_list args);
const char *qcc_arena_sprintf(struct qcc_arena *arena, const char *fmt, ...);
void qcc_arena_reset(struct qcc_arena *arena);

#ifdef __cplusplus
}
#endif // __cplusplus
