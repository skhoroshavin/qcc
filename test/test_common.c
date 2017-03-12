
#include "qcc.h"

TEST(is_unsigned)
{
    ASSERT(qcc_is_unsigned(uint8_t));
    ASSERT(qcc_is_unsigned(uint16_t));
    ASSERT(qcc_is_unsigned(uint32_t));
    ASSERT(qcc_is_unsigned(uint64_t));

    ASSERT(!qcc_is_unsigned(int8_t));
    ASSERT(!qcc_is_unsigned(int16_t));
    ASSERT(!qcc_is_unsigned(int32_t));
    ASSERT(!qcc_is_unsigned(int64_t));
}

TEST(is_float)
{
    ASSERT(!qcc_is_float(uint8_t));
    ASSERT(!qcc_is_float(uint16_t));
    ASSERT(!qcc_is_float(uint32_t));
    ASSERT(!qcc_is_float(uint64_t));
    ASSERT(!qcc_is_float(int8_t));
    ASSERT(!qcc_is_float(int16_t));
    ASSERT(!qcc_is_float(int32_t));
    ASSERT(!qcc_is_float(int64_t));

    ASSERT(qcc_is_float(float));
    ASSERT(qcc_is_float(double));
}

TEST(int_max)
{
    ASSERT(qcc_int_max(1) == INT8_MAX);
    ASSERT(qcc_int_max(2) == INT16_MAX);
    ASSERT(qcc_int_max(4) == INT32_MAX);
    ASSERT(qcc_int_max(8) == INT64_MAX);
}

TEST(int_min)
{
    ASSERT(qcc_int_min(1) == INT8_MIN);
    ASSERT(qcc_int_min(2) == INT16_MIN);
    ASSERT(qcc_int_min(4) == INT32_MIN);
    ASSERT(qcc_int_min(8) == INT64_MIN);
}

TEST(uint_max)
{
    ASSERT(qcc_uint_max(1) == UINT8_MAX);
    ASSERT(qcc_uint_max(2) == UINT16_MAX);
    ASSERT(qcc_uint_max(4) == UINT32_MAX);
    ASSERT(qcc_uint_max(8) == UINT64_MAX);
}

TEST_MAIN()
{
    RUN_TEST(is_unsigned);
    RUN_TEST(is_float);

    RUN_TEST(int_max);
    RUN_TEST(int_min);
    RUN_TEST(uint_max);
}
