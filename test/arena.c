
#include "qcc.h"

static const size_t qcc_arena_object_size = sizeof(struct qcc_arena_object);

#define GIVEN_ARENA(name)                                                      \
    char name##_data[16 * qcc_arena_object_size];                              \
    GIVEN_UINT(name##_size, in_range, 2 * qcc_arena_object_size,               \
               sizeof(name##_data));                                           \
    QCC_ARENA_POD(_ctx->arena, qcc_arena, name);                               \
    qcc_arena_init(name, name##_data, name##_size)

TEST(empty_arena)
{
    GIVEN_ARENA(arena);

    ASSERT(qcc_arena_memory_available(arena) == arena_size);
}

TEST(arena_alloc)
{
    GIVEN_ARENA(arena);
    GIVEN_UINT(alloc_size, not_greater_than, qcc_arena_memory_available(arena));

    void *ptr = qcc_arena_alloc(arena, alloc_size);
    ASSERT(ptr != 0);
    ASSERT(qcc_arena_memory_available(arena) == arena_size - alloc_size);
}

TEST(arena_alloc_out_of_mem)
{
    GIVEN_ARENA(arena);
    GIVEN_UINT(alloc_size, greater_than, qcc_arena_memory_available(arena));

    void *ptr = qcc_arena_alloc(arena, alloc_size);
    ASSERT(ptr == 0);
    ASSERT(qcc_arena_memory_available(arena) == arena_size);
}

TEST(arena_copy)
{
    GIVEN_ARENA(arena);
    GIVEN_UINT(size, not_greater_than, qcc_arena_memory_available(arena));
    GIVEN_DATA(data, size);

    void *ptr = qcc_arena_copy(arena, data, size);
    ASSERT(ptr != 0);
    ASSERT(ptr != data);
    ASSERT_MEM_EQ(ptr, data, size);
    ASSERT(qcc_arena_memory_available(arena) == arena_size - size);
}

TEST(arena_copy_out_of_mem)
{
    GIVEN_ARENA(arena);
    GIVEN_UINT(size, greater_than, qcc_arena_memory_available(arena));
    void *data = (void *)0xDEADBEEF;

    void *ptr = qcc_arena_copy(arena, data, size);
    ASSERT(ptr == 0);
    ASSERT(qcc_arena_memory_available(arena) == arena_size);
}

static void *test_ptr;
static void test_dtor(void *ptr) { test_ptr = ptr; }

TEST(arena_add_object_reset)
{
    GIVEN_ARENA(arena);

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
    GIVEN_ARENA(arena);

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
    RUN_TEST(arena_alloc_out_of_mem);
    RUN_TEST(arena_copy);
    RUN_TEST(arena_copy_out_of_mem);
    RUN_TEST(arena_add_object_reset);
    RUN_TEST(arena_sprintf);
}
