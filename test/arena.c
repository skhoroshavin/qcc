
#include "qcc.h"

typedef struct qcc_arena *qcc_arena_ptr;

qcc_arena_ptr qcc_gen_qcc_arena_ptr(struct qcc_test_context *ctx, size_t size)
{
    qcc_arena_ptr res = qcc_arena_alloc(&ctx->arena, sizeof(*res));
    qcc_arena_add_object(&ctx->arena, res, (qcc_destroy_fn)qcc_arena_done);
    qcc_arena_init(res, size);
    return res;
}

static const size_t qcc_arena_object_size = sizeof(struct qcc_arena_object);

TEST(empty_arena)
{
    GIVEN_UINT(arena_size, not_greater_than, qcc_arena_object_size * 4);
    GIVEN(qcc_arena_ptr, arena, arena_size);

    ASSERT(qcc_arena_memory_available(arena) == arena_size);
}

TEST(arena_alloc)
{
    GIVEN_UINT(arena_size, not_greater_than, qcc_arena_object_size * 4);
    GIVEN(qcc_arena_ptr, arena, arena_size);
    GIVEN_UINT(alloc_size, not_greater_than, arena_size * 2);

    void *ptr = qcc_arena_alloc(arena, alloc_size);
    if (alloc_size > arena_size)
    {
        ASSERT(ptr == 0);
        ASSERT(qcc_arena_memory_available(arena) == arena_size);
    }
    else
    {
        ASSERT(ptr != 0);
        ASSERT(qcc_arena_memory_available(arena) == arena_size - alloc_size);
    }
}

static void *test_ptr;
static void test_dtor(void *ptr) { test_ptr = ptr; }

TEST(arena_add_object_reset)
{
    GIVEN_UINT(arena_size, not_greater_than, qcc_arena_object_size * 4);
    GIVEN(qcc_arena_ptr, arena, arena_size);

    test_ptr = 0;
    void *ptr = (void *)(size_t)qcc_gen_uint_any(_ctx);
    unsigned res = qcc_arena_add_object(arena, ptr, test_dtor);

    if (arena_size < qcc_arena_object_size)
    {
        ASSERT(!res);
        ASSERT(test_ptr == 0);
        ASSERT(qcc_arena_memory_available(arena) == arena_size);
        return;
    }

    ASSERT(res);
    ASSERT(test_ptr == 0);
    ASSERT(qcc_arena_memory_available(arena) ==
           arena_size - qcc_arena_object_size);

    qcc_arena_reset(arena);
    ASSERT(test_ptr == ptr);
    ASSERT(qcc_arena_memory_available(arena) == arena_size);

    test_ptr = 0;
    qcc_arena_reset(arena);
    ASSERT(test_ptr == 0);
    ASSERT(qcc_arena_memory_available(arena) == arena_size);
}

TEST(arena_sprintf)
{
    GIVEN_UINT(arena_size, not_greater_than, qcc_arena_object_size * 4);
    GIVEN(qcc_arena_ptr, arena, arena_size);

    const char *expected_str = "string: Hello, number: 42";
    const size_t expected_size = strlen(expected_str) + 1;

    const char *str =
        qcc_arena_sprintf(arena, "string: %s, number: %d", "Hello", 42);

    if (expected_size > arena_size)
    {
        ASSERT(str == 0);
        ASSERT(qcc_arena_memory_available(arena) == arena_size);
    }
    else
    {
        ASSERT(str != 0);
        ASSERT_STR_EQ(str, expected_str);
        ASSERT(qcc_arena_memory_available(arena) == arena_size - expected_size);
    }
}

TEST_SUITE(arena)
{
    RUN_TEST(empty_arena);
    RUN_TEST(arena_alloc);
    RUN_TEST(arena_add_object_reset);
    RUN_TEST(arena_sprintf);
}
