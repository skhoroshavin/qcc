
#define test_t uint8_t
#include "_test_qcc_uint.c"

#define test_t uint16_t
#include "_test_qcc_uint.c"

#define test_t uint32_t
#include "_test_qcc_uint.c"

#define test_t uint64_t
#include "_test_qcc_uint.c"

TEST_MAIN()
{
    RUN_GROUP_P(qcc_uint, uint8_t);
    RUN_GROUP_P(qcc_uint, uint16_t);
    RUN_GROUP_P(qcc_uint, uint32_t);
    RUN_GROUP_P(qcc_uint, uint64_t);
}
