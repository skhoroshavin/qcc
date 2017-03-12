
#include "test_qcc.h"

#ifndef test_t
#define test_t unsigned
#endif

TEST_P(uint_limits, test_t)
{
    GIVEN_UNSIGNED(test_t, value, any);
    ASSERT_UINT(value, <=, qcc_uint_max(sizeof(test_t)));
}

TEST_P(uint_gen_large, test_t)
{
    GIVEN_CONTEXT(ctx);

    const qcc_uint limit = qcc_uint_max(sizeof(test_t)) >> 4;
    for (size_t i = 0; i < 100; ++i)
        if (qcc_rand_unsigned(ctx, test_t, any) > limit) return;
    FAIL("Generated uints are not large enough");
}

TEST_P(generate_uint, test_t)
{
    GIVEN_CONTEXT(ctx);
    GIVEN_UNSIGNED(test_t, min, any);
    GIVEN_UNSIGNED(test_t, max, not_less_than, min);

    test_t value = qcc_rand_unsigned(ctx, test_t, in_range, min, max);
    ASSERT_UINT(value, <=, qcc_uint_max(sizeof(test_t)));
}

TEST_P(gen_uint_from_array, test_t)
{
    GIVEN_UNSIGNED_ARRAY(test_t, values, non_empty, any);
    GIVEN_UNSIGNED(test_t, value, from_array, sizeof(test_t), values.data,
                   values.size);

    for (size_t i = 0; i != values.size; ++i)
        if (values.data[i] == value) return;
    FAIL("Generated value is not contained in original array");
}

TEST_P(gen_uint_equal_to, test_t)
{
    GIVEN_UNSIGNED(test_t, limit, any);
    GIVEN_UNSIGNED(test_t, value, equal_to, limit);
    ASSERT_UINT(value, ==, limit);
}

TEST_P(gen_uint_in_range, test_t)
{
    GIVEN_UNSIGNED(test_t, min, any);
    GIVEN_UNSIGNED(test_t, max, not_less_than, min);
    GIVEN_UNSIGNED(test_t, value, in_range, min, max);
    ASSERT_UINT(value, >=, min);
    ASSERT_UINT(value, <=, max);
}

TEST_P(gen_uint_less_than, test_t)
{
    GIVEN_UNSIGNED(test_t, limit, greater_than, 0);
    GIVEN_UNSIGNED(test_t, value, less_than, limit);
    ASSERT_UINT(value, <, limit);
}

TEST_P(gen_uint_greater_than, test_t)
{
    GIVEN_UNSIGNED(test_t, limit, less_than, qcc_uint_max(sizeof(test_t)));
    GIVEN_UNSIGNED(test_t, value, greater_than, limit);
    ASSERT_UINT(value, >, limit);
}

TEST_P(gen_uint_not_less_than, test_t)
{
    GIVEN_UNSIGNED(test_t, limit, any);
    GIVEN_UNSIGNED(test_t, value, not_less_than, limit);
    ASSERT_UINT(value, >=, limit);
}

TEST_P(gen_uint_not_greater_than, test_t)
{
    GIVEN_UNSIGNED(test_t, limit, any);
    GIVEN_UNSIGNED(test_t, value, not_greater_than, limit);
    ASSERT_UINT(value, <=, limit);
}

TEST_P(gen_uint_not_equal_to, test_t)
{
    GIVEN_UNSIGNED(test_t, limit, any);
    GIVEN_UNSIGNED(test_t, value, not_equal_to, limit);
    ASSERT_UINT(value, !=, limit);
}

TEST_P(gen_uint_array, test_t)
{
    GIVEN_UNSIGNED(test_t, min, any);
    GIVEN_UNSIGNED(test_t, max, not_less_than, min);
    GIVEN_UNSIGNED_ARRAY(test_t, test, any_size, in_range, min, max);

    for (size_t i = 0; i != test.size; ++i)
    {
        ASSERT_UINT(test.data[i], >=, min);
        ASSERT_UINT(test.data[i], <=, max);
    }
}

TEST_P(gen_uint_non_empty_array, test_t)
{
    GIVEN_UNSIGNED_ARRAY(test_t, test, non_empty, any);
    ASSERT_UINT(test.size, !=, 0);
}

TEST_P(gen_uint_fixed_size_array, test_t)
{
    GIVEN_UNSIGNED(size_t, size, less_than, 32);
    GIVEN_UNSIGNED_ARRAY(test_t, test, fixed_size(size), any);
    ASSERT_UINT(test.size, ==, size);
}

TEST_GROUP_P(qcc_uint, test_t)
{
    RUN_TEST_P(uint_limits, test_t);
    RUN_TEST_P(uint_gen_large, test_t);
    RUN_TEST_P(generate_uint, test_t);

    RUN_TEST_P(gen_uint_from_array, test_t);
    RUN_TEST_P(gen_uint_equal_to, test_t);
    RUN_TEST_P(gen_uint_in_range, test_t);
    RUN_TEST_P(gen_uint_less_than, test_t);
    RUN_TEST_P(gen_uint_greater_than, test_t);
    RUN_TEST_P(gen_uint_not_less_than, test_t);
    RUN_TEST_P(gen_uint_not_greater_than, test_t);
    RUN_TEST_P(gen_uint_not_equal_to, test_t);
    RUN_TEST_P(gen_uint_array, test_t);
    RUN_TEST_P(gen_uint_non_empty_array, test_t);
    RUN_TEST_P(gen_uint_fixed_size_array, test_t);
}

#undef test_t
