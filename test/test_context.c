
#include "test_qcc.h"

TEST(empty_context)
{
    GIVEN_CONTEXT(ctx);

    ASSERT(ctx->result == QCC_TEST_SUCCEED);
    ASSERT(ctx->error == 0);
}

TEST(context_fail)
{
    GIVEN_CONTEXT(ctx);

    qcc_context_fail(ctx, "Code: %d, message: %s", 42, "FAIL");
    ASSERT_UNSIGNED(ctx->result, ==, QCC_TEST_FAIL);
    ASSERT(ctx->error != 0);
    ASSERT_STR_EQ(ctx->error, "Code: 42, message: FAIL");
}

TEST(context_params)
{
    GIVEN_CONTEXT(ctx);
    qcc_context_register_param(ctx, "code: %d", 42);
    qcc_context_register_param(ctx, "message: %s", "Hello");

    ASSERT_UNSIGNED(ctx->result, ==, QCC_TEST_SUCCEED);
    ASSERT(ctx->error == 0);

    struct qcc_log_message *message =
        (struct qcc_log_message *)ctx->logger.messages.first;
    ASSERT(message != 0);
    ASSERT_STR_EQ(message->data, "code: 42");

    message = message->next;
    ASSERT(message != 0);
    ASSERT_STR_EQ(message->data, "message: Hello");

    message = message->next;
    ASSERT(message == 0);
}

TEST_MAIN()
{
    RUN_TEST(empty_context);
    RUN_TEST(context_fail);
    RUN_TEST(context_params);
}
