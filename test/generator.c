
#include "qcc.h"

#define GIVEN_DATA(name, max_size)                                             \
    GIVEN_UINT(name##_size, in_range, 1, max_size);                            \
    uint8_t *name##_data = qcc_arena_alloc(&_ctx->arena, name##_size);         \
    qcc_test_context_rand(_ctx, name##_data, name##_size);

TEST(gen_value_from)
{
    GIVEN_DATA(test, 64);

    struct qcc_generator *gen = qcc_gen_value_from(_ctx, test_data, test_size);
    uint8_t *res = (uint8_t *)qcc_arena_alloc(&_ctx->arena, test_size);
    qcc_test_context_rand(_ctx, res, test_size);
    qcc_generate(gen, _ctx, res, test_size);
    ASSERT_MEM_EQ(res, test_data, test_size);
}

TEST_SUITE(generator) { RUN_TEST(gen_value_from); }
