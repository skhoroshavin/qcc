
#include "qcc.h"

typedef struct qcc_test_context *qcc_test_context_ptr;

qcc_test_context_ptr qcc_gen_qcc_test_context_ptr(struct qcc_test_context *ctx)
{
    qcc_test_context_ptr res = qcc_arena_alloc(&ctx->arena, sizeof(*res));
    qcc_arena_add_object(&ctx->arena, res, (qcc_destroy_fn)qcc_arena_done);
    qcc_test_context_init(res);
    return res;
}

TEST(empty_context)
{
    GIVEN(qcc_test_context_ptr, ctx);

    ASSERT(ctx->result == QCC_TEST_SUCCEED);
    ASSERT(ctx->error == 0);
}

TEST_SUITE(test_context) { RUN_TEST(empty_context); }
