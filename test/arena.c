
#include "qcc.h"

#define GIVEN_ARENA(name, size)                                                \
    struct qcc_arena *name =                                                   \
        qcc_arena_alloc(&_ctx->arena, sizeof(struct qcc_arena),                \
                        (qcc_destroy_fn)qcc_arena_done);                       \
    qcc_arena_init(name, size);

TEST(empty_arena)
{
    GIVEN_ARENA(arena, 1024);

    ASSERT(arena->objects != 0);
    ASSERT(arena->size == 0);
    ASSERT(arena->max_size == 1024);
}

TEST(arena_alloc)
{
    GIVEN_ARENA(arena, 1024);

    void *ptr = qcc_arena_alloc(arena, 4096, 0);

    ASSERT(ptr != 0);
    ASSERT(arena->objects != 0);
    ASSERT(arena->size == 1);
    ASSERT(arena->max_size == 1024);
}

TEST(arena_sprintf)
{
    GIVEN_ARENA(arena, 1024);

    const char *str =
        qcc_arena_sprintf(arena, "string: %s, number: %d", "Hello", 42);

    ASSERT(str != 0);
    ASSERT_STR_EQ(str, "string: Hello, number: 42");
    ASSERT(arena->objects != 0);
    ASSERT(arena->size == 1);
    ASSERT(arena->max_size == 1024);
}

static void *test_ptr;
static void test_dtor(void *ptr) { test_ptr = ptr; }

TEST(arena_reset)
{
    GIVEN_ARENA(arena, 1024);

    test_ptr = 0;
    void *ptr = qcc_arena_alloc(arena, 4096, test_dtor);
    ASSERT(ptr != 0);
    ASSERT(test_ptr == 0);

    qcc_arena_reset(arena);
    ASSERT(test_ptr == ptr);
    ASSERT(arena->objects != 0);
    ASSERT(arena->size == 0);
    ASSERT(arena->max_size == 1024);
}

TEST_SUITE(arena)
{
    RUN_TEST(empty_arena);
    RUN_TEST(arena_alloc);
    RUN_TEST(arena_sprintf);
    RUN_TEST(arena_reset);
}
