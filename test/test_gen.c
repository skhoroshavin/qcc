
#include "qcc.h"

TEST(gen_unsigned)
{
    GIVEN_UINT_IN_RANGE(min, 0, UINT32_MAX);
    GIVEN_UINT_IN_RANGE(max, min, UINT32_MAX);

    unsigned value = qcc_gen_unsigned(_ctx, min, max);
    ASSERT_UINT(value, >=, min);
    ASSERT_UINT(value, <=, max);
}

TEST_SUITE(test_gen) { RUN_TEST(gen_unsigned); }
