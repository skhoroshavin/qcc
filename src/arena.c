
#include "arena.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void qcc_arena_init(struct qcc_arena *arena, void *buffer, size_t size)
{
    arena->buffer_start = buffer;
    arena->buffer_end = arena->buffer_start + size;
    arena->free_memory = arena->buffer_start;
    arena->objects = (struct qcc_arena_object *)arena->buffer_end;
    arena->array_start = 0;
    arena->array_end = 0;
}

void qcc_arena_done(struct qcc_arena *arena) { qcc_arena_reset(arena); }

void qcc_arena_reset(struct qcc_arena *arena)
{
    while ((uint8_t *)arena->objects < arena->buffer_end)
    {
        arena->objects->dtor(arena->objects->ptr);
        ++arena->objects;
    }
    arena->free_memory = arena->buffer_start;
}

size_t qcc_arena_memory_available(const struct qcc_arena *arena)
{
    return (uint8_t *)arena->objects - arena->free_memory;
}

void *qcc_arena_alloc(struct qcc_arena *arena, size_t size)
{
    if (qcc_arena_memory_available(arena) < size) return 0;
    if (arena->array_start) return 0;

    void *ptr = arena->free_memory;
    arena->free_memory += size;
    return ptr;
}

void *qcc_arena_copy(struct qcc_arena *arena, const void *data, size_t size)
{
    void *copy = qcc_arena_alloc(arena, size);
    if (!copy) return 0;
    memcpy(copy, data, size);
    return copy;
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
    char *str = (char *)arena->free_memory;

    va_list tmp;
    va_copy(tmp, args);
    unsigned len = vsnprintf(str, qcc_arena_memory_available(arena), fmt, tmp);
    va_end(tmp);

    if (len < qcc_arena_memory_available(arena))
    {
        arena->free_memory += len + 1;
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

void qcc_arena_begin_array(struct qcc_arena *arena)
{
    arena->array_start = arena->free_memory;
    arena->array_end = arena->array_start;
}

void *qcc_arena_append_array(struct qcc_arena *arena, void *data, size_t size)
{
    if (size > qcc_arena_memory_available(arena)) return 0;

    void *item = arena->array_end;
    if (data) memcpy(item, data, size);
    arena->array_end += size;
    return item;
}

void *qcc_arena_end_array(struct qcc_arena *arena)
{
    void *res = arena->array_start;
    arena->free_memory = arena->array_end;
    arena->array_start = 0;
    arena->array_end = 0;
    return res;
}
