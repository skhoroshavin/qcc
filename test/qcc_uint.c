
#include "qcc.h"

TEST(gen_uint)
{
    GIVEN_UINT(min, any);
    GIVEN_UINT(max, not_less_than, min);
    GIVEN_UINT(value, in_range, min, max);

    ASSERT_UINT(value, >=, min);
    ASSERT_UINT(value, <=, max);
}

TEST_SUITE(qcc_uint) { RUN_TEST(gen_uint); }
