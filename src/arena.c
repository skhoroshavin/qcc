
#include "arena.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void *qcc_malloc(size_t size) { return malloc(size); }
void qcc_free(void *ptr) { free(ptr); }

void qcc_arena_init(struct qcc_arena *arena, size_t max_size)
{
    arena->buffer_start = qcc_malloc(max_size);
    arena->buffer_end = arena->buffer_start + max_size;
    arena->available_memory = arena->buffer_start;
    arena->objects = (struct qcc_arena_object *)arena->buffer_end;
}

void qcc_arena_done(struct qcc_arena *arena)
{
    qcc_arena_reset(arena);
    qcc_free(arena->buffer_start);
}

size_t qcc_arena_memory_available(const struct qcc_arena *arena)
{
    return (uint8_t *)arena->objects - arena->available_memory;
}

void *qcc_arena_alloc(struct qcc_arena *arena, size_t size)
{
    if (qcc_arena_memory_available(arena) < size) return 0;

    void *ptr = arena->available_memory;
    arena->available_memory += size;
    return ptr;
}

unsigned qcc_arena_add_object(struct qcc_arena *arena, void *ptr,
                              qcc_destroy_fn dtor)
{
    if (qcc_arena_memory_available(arena) < sizeof(struct qcc_arena_object))
        return 0;

    --arena->objects;
    arena->objects->ptr = ptr;
    arena->objects->dtor = dtor;
    return 1;
}

const char *qcc_arena_vsprintf(struct qcc_arena *arena, const char *fmt,
                               va_list args)
{
    char *str = (char *)arena->available_memory;

    va_list tmp;
    va_copy(tmp, args);
    unsigned len = vsnprintf(str, qcc_arena_memory_available(arena), fmt, tmp);
    va_end(tmp);

    if (len < qcc_arena_memory_available(arena))
    {
        arena->available_memory += len + 1;
        return str;
    }

    return 0;
}

const char *qcc_arena_sprintf(struct qcc_arena *arena, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    const char *str = qcc_arena_vsprintf(arena, fmt, args);
    va_end(args);

    return str;
}

void qcc_arena_reset(struct qcc_arena *arena)
{
    while ((uint8_t *)arena->objects < arena->buffer_end)
    {
        arena->objects->dtor(arena->objects->ptr);
        ++arena->objects;
    }
    arena->available_memory = arena->buffer_start;
}
