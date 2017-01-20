
#include "qcc.h"

TEST(empty_context)
{
    QCC_ARENA_OBJ(&_ctx->arena, qcc_test_context, ctx);

    ASSERT(ctx->result == QCC_TEST_SUCCEED);
    ASSERT(ctx->error == 0);
}

TEST(context_fail)
{
    QCC_ARENA_OBJ(&_ctx->arena, qcc_test_context, ctx);

    qcc_test_context_fail(ctx, "Code: %d, message: %s", 42, "FAIL");
    ASSERT_UINT(ctx->result, ==, QCC_TEST_FAIL);
    ASSERT(ctx->error != 0);
    ASSERT_STR_EQ(ctx->error, "Code: 42, message: FAIL");
}

TEST(context_params)
{
    QCC_ARENA_OBJ(&_ctx->arena, qcc_test_context, ctx);
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
