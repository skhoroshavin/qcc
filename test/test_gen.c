
#include "qcc.h"

TEST(gen_unsigned)
{
    GIVEN(unsigned, min, 0, UINT32_MAX);
    GIVEN(unsigned, max, min, UINT32_MAX);

    unsigned value = qcc_gen_unsigned(_ctx, min, max);
    ASSERT(value >= min);
    ASSERT(value <= max);
}

TEST_SUITE(test_gen) { RUN_TEST(gen_unsigned); }
