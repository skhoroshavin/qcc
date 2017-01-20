
#include "qcc.h"

typedef struct qcc_test_context *qcc_test_context_ptr;

qcc_test_context_ptr qcc_gen_qcc_test_context_ptr(struct qcc_test_context *ctx)
{
    qcc_test_context_ptr res = qcc_arena_alloc(&ctx->arena, sizeof(*res));
    qcc_arena_add_object(&ctx->arena, res,
                         (qcc_destroy_fn)qcc_test_context_done);
    qcc_test_context_init(res);
    return res;
}

TEST(empty_context)
{
    GIVEN(qcc_test_context_ptr, ctx);

    ASSERT(ctx->result == QCC_TEST_SUCCEED);
    ASSERT(ctx->error == 0);
}

TEST(context_fail)
{
    GIVEN(qcc_test_context_ptr, ctx);

    qcc_test_context_fail(ctx, "Code: %d, message: %s", 42, "FAIL");
    ASSERT_UINT(ctx->result, ==, QCC_TEST_FAIL);
    ASSERT(ctx->error != 0);
    ASSERT_STR_EQ(ctx->error, "Code: 42, message: FAIL");
}

TEST(context_params)
{
    GIVEN(qcc_test_context_ptr, ctx);
    qcc_test_context_register_param(ctx, "code: %d", 42);
    qcc_test_context_register_param(ctx, "message: %s", "Hello");

    ASSERT_UINT(ctx->result, ==, QCC_TEST_SUCCEED);
    ASSERT(ctx->error == 0);

    struct qcc_test_param *param = ctx->param;
    ASSERT(param != 0);
    ASSERT_STR_EQ(param->value, "code: 42");

    param = param->next;
    ASSERT(param != 0);
    ASSERT_STR_EQ(param->value, "message: Hello");

    param = param->next;
    ASSERT(param == 0);
}

TEST_SUITE(test_context)
{
    RUN_TEST(empty_context);
    RUN_TEST(context_fail);
    RUN_TEST(context_params);
}
