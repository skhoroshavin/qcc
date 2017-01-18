
#include "qcc.h"

TEST(empty_arena)
{
    struct qcc_arena arena;
    qcc_arena_init(&arena, 1024);

    ASSERT(arena.objects != 0);
    ASSERT(arena.size == 0);
    ASSERT(arena.max_size == 1024);

    qcc_arena_done(&arena);
}

TEST(arena_alloc)
{
    struct qcc_arena arena;
    qcc_arena_init(&arena, 1024);

    void *ptr = qcc_arena_alloc(&arena, 4096, 0);

    ASSERT(ptr != 0);
    ASSERT(arena.objects != 0);
    ASSERT(arena.size == 1);
    ASSERT(arena.max_size == 1024);

    qcc_arena_done(&arena);
}

static void *test_ptr;
static void test_dtor(void *ptr) { test_ptr = ptr; }

TEST(arena_reset)
{
    struct qcc_arena arena;
    qcc_arena_init(&arena, 1024);

    test_ptr = 0;
    void *ptr = qcc_arena_alloc(&arena, 4096, test_dtor);
    ASSERT(ptr != 0);
    ASSERT(test_ptr == 0);

    qcc_arena_reset(&arena);
    ASSERT(test_ptr == ptr);

    qcc_arena_done(&arena);
}

TEST_SUITE(arena)
{
    RUN_TEST(empty_arena);
    RUN_TEST(arena_alloc);
    RUN_TEST(arena_reset);
}
