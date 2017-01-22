
#pragma once

#include <memory.h>
#include <stdarg.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

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
    uint8_t *free_memory;
    struct qcc_arena_object *objects;
    uint8_t *array_start;
    uint8_t *array_end;
};

void qcc_arena_init(struct qcc_arena *arena, size_t max_size);
void qcc_arena_done(struct qcc_arena *arena);
void qcc_arena_reset(struct qcc_arena *arena);
size_t qcc_arena_memory_available(const struct qcc_arena *arena);

void *qcc_arena_alloc(struct qcc_arena *arena, size_t size);
void *qcc_arena_copy(struct qcc_arena *arena, const void *data, size_t size);
unsigned qcc_arena_add_object(struct qcc_arena *arena, void *ptr,
                              qcc_destroy_fn dtor);

const char *qcc_arena_vsprintf(struct qcc_arena *arena, const char *fmt,
                               va_list args);
const char *qcc_arena_sprintf(struct qcc_arena *arena, const char *fmt, ...);

void qcc_arena_begin_array(struct qcc_arena *arena);
void *qcc_arena_append_array(struct qcc_arena *arena, void *data, size_t size);
void *qcc_arena_end_array(struct qcc_arena *arena);

#define QCC_ARENA_POD(arena, type, name)                                       \
    struct type *name =                                                        \
        (struct type *)qcc_arena_alloc(arena, sizeof(struct type));

#define QCC_ARENA_OBJ(arena, type, name, ...)                                  \
    QCC_ARENA_POD(arena, type, name)                                           \
    qcc_arena_add_object(arena, name, (qcc_destroy_fn)type##_done);            \
    type##_init(name, ##__VA_ARGS__);

#ifdef __cplusplus
}
#endif
