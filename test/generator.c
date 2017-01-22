
#include "qcc.h"

#define GIVEN_DATA(name, max_size)                                             \
    GIVEN_UINT(name##_size, not_greater_than, max_size);                       \
    void *name##_data = qcc_arena_alloc(&_ctx->arena, name##_size);

TEST(gen_value)
{
    GIVEN_DATA(test, 64);

    struct qcc_generator *gen = qcc_gen_value(_ctx, test_data, test_size);
    uint8_t *res = (uint8_t *)qcc_arena_alloc(&_ctx->arena, test_size);
    for (size_t i = 0; i < test_size; ++i)
        res[i] = qcc_test_context_rand(_ctx) & 0xff;
    qcc_generate(gen, _ctx, res, test_size);
    ASSERT(memcmp(res, test_data, test_size) == 0);
}

TEST_SUITE(generator) { RUN_TEST(gen_value); }
