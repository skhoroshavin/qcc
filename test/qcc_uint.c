
#include "qcc.h"

TEST(gen_uint)
{
    GIVEN_UINT_IN_RANGE(min, 0, UINT32_MAX);
    GIVEN_UINT_IN_RANGE(max, min, UINT32_MAX);

    unsigned value = qcc_gen_unsigned(_ctx, min, max);
    ASSERT_UINT(value, >=, min);
    ASSERT_UINT(value, <=, max);
}

TEST_SUITE(qcc_uint) { RUN_TEST(gen_uint); }

