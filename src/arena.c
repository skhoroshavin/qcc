
#include "arena.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void qcc_arena_init(struct qcc_arena *arena, size_t max_size)
{
    arena->objects =
        (struct qcc_arena_obj *)malloc(max_size * sizeof(struct qcc_arena_obj));
    arena->size = 0;
    arena->max_size = max_size;
}

void qcc_arena_done(struct qcc_arena *arena)
{
    qcc_arena_reset(arena);
    free(arena->objects);
}

static void _qcc_arena_push_back(struct qcc_arena *arena, void *ptr,
                                 qcc_destroy_fn dtor)
{
    arena->objects[arena->size].ptr = ptr;
    arena->objects[arena->size].dtor = dtor;
    ++arena->size;
}

void *qcc_arena_alloc(struct qcc_arena *arena, size_t size, qcc_destroy_fn dtor)
{
    if (arena->size == arena->max_size) return 0;

    void *ptr = malloc(size);
    _qcc_arena_push_back(arena, ptr, dtor);
    return ptr;
}

const char *qcc_arena_sprintf(struct qcc_arena *arena, const char *fmt, ...)
{
    if (arena->size == arena->max_size) return 0;

    unsigned len = 2 * strlen(fmt);
    char *str = malloc(len);

    while (1)
    {
        va_list args;
        va_start(args, fmt);
        unsigned real_len = vsnprintf(str, len, fmt, args);
        va_end(args);

        if (real_len < len) break;

        len *= 2;
        str = realloc(str, len);
    }

    _qcc_arena_push_back(arena, str, 0);
    return str;
}

void qcc_arena_reset(struct qcc_arena *arena)
{
    for (size_t i = 0; i != arena->size; ++i)
    {
        void *ptr = arena->objects[i].ptr;
        qcc_destroy_fn dtor = arena->objects[i].dtor;
        if (dtor) dtor(ptr);
        free(ptr);
    }
    arena->size = 0;
}
