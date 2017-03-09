
#include "qcc.h"

#define test_t uint8_t
#include "test_qcc_uint.inl"

#define test_t uint16_t
#include "test_qcc_uint.inl"

#define test_t uint32_t
#include "test_qcc_uint.inl"

#define test_t uint64_t
#include "test_qcc_uint.inl"

TEST_MAIN()
{
    RUN_GROUP_P(qcc_uint, uint8_t);
    RUN_GROUP_P(qcc_uint, uint16_t);
    RUN_GROUP_P(qcc_uint, uint32_t);
    RUN_GROUP_P(qcc_uint, uint64_t);
}
