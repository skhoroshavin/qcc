
#include "qcc.h"

TEST(gen_value_from)
{
    GIVEN_UINT(test_size, in_range, 1, 64);
    GIVEN_DATA(test_data, test_size);

    struct qcc_generator *gen = qcc_gen_value_from(_ctx, test_data, test_size);
    void *res = qcc_arena_alloc(&_ctx->arena, test_size);
    qcc_generate(gen, _ctx, res, test_size);
    ASSERT_MEM_EQ(res, test_data, test_size);
}

TEST_SUITE(generator) { RUN_TEST(gen_value_from); }
