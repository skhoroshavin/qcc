
#include "test_qcc.h"
#include <stdio.h>
#include <stdlib.h>

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
    void *data = (void *)0xDEADBEEFCAFE;

    void *ptr = qcc_arena_copy(arena, data, size);
    ASSERT(ptr == 0);
    ASSERT(qcc_arena_memory_available(arena) == arena_size);
}

enum
{
    test_obj_max_count = 4
};
static size_t test_obj_count;
static void *test_obj_data[test_obj_max_count];
static void test_dtor(void *ptr)
{
    if (test_obj_count >= test_obj_max_count) abort();
    test_obj_data[test_obj_count] = ptr;
    ++test_obj_count;
}

TEST(arena_objects)
{
    GIVEN_ARENA(arena);
    GIVEN_PTR(obj1);
    GIVEN_PTR(obj2);

    test_obj_count = 0;
    memset(test_obj_data, 0, sizeof(test_obj_data));
    qcc_arena_add_object(arena, obj1, test_dtor);
    qcc_arena_add_object(arena, obj2, test_dtor);
    ASSERT(test_obj_count == 0);
    ASSERT(qcc_arena_memory_available(arena) ==
           arena_size - 2 * sizeof(struct qcc_arena_object));

    qcc_arena_reset(arena);
    ASSERT(test_obj_count == 2);
    ASSERT(test_obj_data[0] == obj2);
    ASSERT(test_obj_data[1] == obj1);
    ASSERT(qcc_arena_memory_available(arena) == arena_size);

    test_obj_count = 0;
    memset(test_obj_data, 0, sizeof(test_obj_data));
    qcc_arena_reset(arena);
    ASSERT(test_obj_count == 0);
    ASSERT(qcc_arena_memory_available(arena) == arena_size);
}

TEST(arena_sprintf)
{
    GIVEN_ARENA(arena);

    const char *expected_str = "string: Hello, number: 42";
    const size_t expected_size = strlen(expected_str) + 1;
    ASSUME(expected_size < arena_size);

    const char *str =
        qcc_arena_sprintf(arena, "string: %s, number: %d", "Hello", 42);

    ASSERT(str != 0);
    ASSERT_STR_EQ(str, expected_str);
    ASSERT(qcc_arena_memory_available(arena) == arena_size - expected_size);
}

TEST(arena_array)
{
    GIVEN_ARENA(arena);
    GIVEN_UINT_ARRAY(test, smaller_than(arena_size / sizeof(unsigned)), any);
    GIVEN_UINT(alloc_size, not_greater_than, arena_size);

    qcc_arena_begin_array(arena);
    for (size_t i = 0; i < test.size; ++i)
    {
        if (rand() % 2)
        {
            void *ptr = qcc_arena_append_array(arena, 0, sizeof(unsigned));
            ASSERT(ptr);
            *((unsigned *)ptr) = test.data[i];
        }
        else
        {
            void *ptr =
                qcc_arena_append_array(arena, test.data + i, sizeof(unsigned));
            ASSERT(ptr);
            ASSERT_UINT(*((unsigned *)ptr), ==, test.data[i]);
        }
    }

    void *ptr = qcc_arena_end_array(arena);
    ASSERT(ptr);
    ASSERT_MEM_EQ(ptr, test.data, test.size);
}

TEST(arena_array_out_of_mem)
{
    GIVEN_ARENA(arena);
    GIVEN_UINT(alloc_size_good, not_greater_than, arena_size);
    GIVEN_UINT(alloc_size_fail, greater_than, arena_size);
    GIVEN_DATA(test, alloc_size_good);

    qcc_arena_begin_array(arena);
    ASSERT(!qcc_arena_alloc(arena, alloc_size_good));

    void *ptr_good = qcc_arena_append_array(arena, test, alloc_size_good);
    ASSERT(ptr_good);
    ASSERT_MEM_EQ(ptr_good, test, alloc_size_good);
    ASSERT_UINT(qcc_arena_memory_available(arena), ==, arena_size);

    void *ptr_fail = qcc_arena_append_array(arena, 0, alloc_size_fail);
    ASSERT(!ptr_fail);
    ASSERT_UINT(qcc_arena_memory_available(arena), ==, arena_size);

    void *ptr_end = qcc_arena_end_array(arena);
    ASSERT(ptr_end);
    ASSERT_MEM_EQ(ptr_end, test, alloc_size_good);
    ASSERT_UINT(qcc_arena_memory_available(arena), ==,
                arena_size - alloc_size_good);
}

TEST_MAIN()
{
    RUN_TEST(arena_alloc);
    RUN_TEST(arena_alloc_out_of_mem);
    RUN_TEST(arena_copy);
    RUN_TEST(arena_copy_out_of_mem);
    RUN_TEST(arena_objects);
    RUN_TEST(arena_sprintf);
    RUN_TEST(arena_array);
    RUN_TEST(arena_array_out_of_mem);
}
