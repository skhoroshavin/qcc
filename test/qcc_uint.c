
#include "qcc.h"

TEST(gen_uint_equal_to)
{
    GIVEN_UINT(limit, any);
    GIVEN_UINT(value, equal_to, limit);
    ASSERT_UINT(value, ==, limit);
}

TEST(gen_uint_in_range)
{
    GIVEN_UINT(min, any);
    GIVEN_UINT(max, not_less_than, min);
    GIVEN_UINT(value, in_range, min, max);

    ASSERT_UINT(value, >=, min);
    ASSERT_UINT(value, <=, max);
}

TEST(gen_uint_less_than)
{
    GIVEN_UINT(limit, greater_than, 0);
    GIVEN_UINT(value, less_than, limit);
    ASSERT_UINT(value, <, limit);
}

TEST(gen_uint_greater_than)
{
    GIVEN_UINT(limit, less_than, UINT32_MAX);
    GIVEN_UINT(value, greater_than, limit);
    ASSERT_UINT(value, >, limit);
}

TEST(gen_uint_not_less_than)
{
    GIVEN_UINT(limit, any);
    GIVEN_UINT(value, not_less_than, limit);
    ASSERT_UINT(value, >=, limit);
}

TEST(gen_uint_not_greater_than)
{
    GIVEN_UINT(limit, any);
    GIVEN_UINT(value, not_greater_than, limit);
    ASSERT_UINT(value, <=, limit);
}

TEST(gen_uint_not_equal_to)
{
    GIVEN_UINT(limit, any);
    GIVEN_UINT(value, not_equal_to, limit);
    ASSERT_UINT(value, !=, limit);
}

TEST_SUITE(qcc_uint)
{
    RUN_TEST(gen_uint_equal_to);
    RUN_TEST(gen_uint_in_range);
    RUN_TEST(gen_uint_less_than);
    RUN_TEST(gen_uint_greater_than);
    RUN_TEST(gen_uint_not_less_than);
    RUN_TEST(gen_uint_not_greater_than);
    RUN_TEST(gen_uint_not_equal_to);
}
