
#include "qcc.h"

typedef struct qcc_arena *qcc_arena_ptr;

qcc_arena_ptr qcc_gen_qcc_arena_ptr(struct qcc_test_context *ctx, size_t size)
{
    qcc_arena_ptr res = qcc_arena_alloc(&ctx->arena, sizeof(*res));
    qcc_arena_add_object(&ctx->arena, res, (qcc_destroy_fn)qcc_arena_done);
    qcc_arena_init(res, size);
    return res;
}

TEST(empty_arena)
{
    GIVEN(qcc_arena_ptr, arena, 1024);

    ASSERT(qcc_arena_memory_available(arena) == 1024);
}

TEST(arena_alloc)
{
    GIVEN(qcc_arena_ptr, arena, 1024);

    void *ptr = qcc_arena_alloc(arena, 128);

    ASSERT(ptr != 0);
    ASSERT(qcc_arena_memory_available(arena) == 1024 - 128);
}

TEST(arena_out_of_mem)
{
    GIVEN(qcc_arena_ptr, arena, 1024);

    void *ptr = qcc_arena_alloc(arena, 1025);

    ASSERT(ptr == 0);
    ASSERT(qcc_arena_memory_available(arena) == 1024);
}

static void *test_ptr;
static void test_dtor(void *ptr) { test_ptr = ptr; }

TEST(arena_add_object_reset)
{
    GIVEN(qcc_arena_ptr, arena, 1024);

    test_ptr = 0;
    void *ptr = qcc_arena_alloc(arena, 128);
    ASSERT(ptr != 0);

    unsigned res = qcc_arena_add_object(arena, ptr, test_dtor);
    ASSERT(res);
    ASSERT(test_ptr == 0);
    ASSERT(qcc_arena_memory_available(arena) ==
           1024 - 128 - sizeof(struct qcc_arena_object));

    qcc_arena_reset(arena);
    ASSERT(test_ptr == ptr);
    ASSERT(qcc_arena_memory_available(arena) == 1024);

    test_ptr = 0;
    qcc_arena_reset(arena);
    ASSERT(test_ptr == 0);
    ASSERT(qcc_arena_memory_available(arena) == 1024);
}

TEST(arena_sprintf)
{
    GIVEN(qcc_arena_ptr, arena, 1024);

    const char *str =
        qcc_arena_sprintf(arena, "string: %s, number: %d", "Hello", 42);

    ASSERT(str != 0);
    ASSERT_STR_EQ(str, "string: Hello, number: 42");
    ASSERT(qcc_arena_memory_available(arena) == 1024 - strlen(str) - 1);
}

TEST_SUITE(arena)
{
    RUN_TEST(empty_arena);
    RUN_TEST(arena_alloc);
    RUN_TEST(arena_out_of_mem);
    RUN_TEST(arena_add_object_reset);
    RUN_TEST(arena_sprintf);
}
