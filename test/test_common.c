
#include "qcc.h"

TEST(is_signed_type)
{
    ASSERT(qcc_is_unsigned_type(uint8_t));
    ASSERT(qcc_is_unsigned_type(uint16_t));
    ASSERT(qcc_is_unsigned_type(uint32_t));
    ASSERT(qcc_is_unsigned_type(uint64_t));

    ASSERT(!qcc_is_unsigned_type(int8_t));
    ASSERT(!qcc_is_unsigned_type(int16_t));
    ASSERT(!qcc_is_unsigned_type(int32_t));
    ASSERT(!qcc_is_unsigned_type(int64_t));
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
    RUN_TEST(is_signed_type);

    RUN_TEST(int_max);
    RUN_TEST(int_min);
    RUN_TEST(uint_max);
}
