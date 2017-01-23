
#include "qcc.h"

#define GIVEN_TEST_CONTEXT(name)                                               \
    char name##_data[8192];                                                    \
    struct qcc_test_env name##_env;                                            \
    qcc_test_env_init(&name##_env, name##_data, sizeof(name##_data));          \
    QCC_ARENA_OBJ(_ctx->arena, qcc_test_context, name, &name##_env);

TEST(empty_context)
{
    GIVEN_TEST_CONTEXT(ctx);

    ASSERT(ctx->result == QCC_TEST_SUCCEED);
    ASSERT(ctx->error == 0);
}

TEST(context_fail)
{
    GIVEN_TEST_CONTEXT(ctx);

    qcc_test_context_fail(ctx, "Code: %d, message: %s", 42, "FAIL");
    ASSERT_UINT(ctx->result, ==, QCC_TEST_FAIL);
    ASSERT(ctx->error != 0);
    ASSERT_STR_EQ(ctx->error, "Code: 42, message: FAIL");
}

TEST(context_params)
{
    GIVEN_TEST_CONTEXT(ctx);
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
