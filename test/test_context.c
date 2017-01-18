
#include "qcc.h"

TEST(default_context)
{
    struct qcc_test_context ctx;
    qcc_test_context_init(&ctx);
    ASSERT(ctx.result == QCC_TEST_SUCCEED);
    ASSERT(ctx.message == 0);
    qcc_test_context_done(&ctx);
}

TEST(assert_true_succeeds)
{
    struct qcc_test_context ctx;
    qcc_test_context_init(&ctx);
    qcc_assert(&ctx, 1, "Success");
    ASSERT(ctx.result == QCC_TEST_SUCCEED);
    ASSERT(ctx.message == 0);
    qcc_test_context_done(&ctx);
}

TEST(assert_false_fails)
{
    struct qcc_test_context ctx;
    qcc_test_context_init(&ctx);
    qcc_assert(&ctx, 0, "MyFail");
    ASSERT(ctx.result == QCC_TEST_FAIL);
    ASSERT(ctx.message != 0);
    ASSERT_STR_EQ(ctx.message, "MyFail");
    qcc_test_context_done(&ctx);
}

TEST(assert_with_format)
{
    struct qcc_test_context ctx;
    qcc_test_context_init(&ctx);
    qcc_assert(&ctx, 0, "%s", "SomeLongString");
    ASSERT(ctx.result == QCC_TEST_FAIL);
    ASSERT(ctx.message != 0);
    ASSERT_STR_EQ(ctx.message, "SomeLongString");
    qcc_test_context_done(&ctx);
}

TEST(assume_true_succeeds)
{
    struct qcc_test_context ctx;
    qcc_test_context_init(&ctx);
    qcc_assume(&ctx, 1);
    ASSERT(ctx.result == QCC_TEST_SUCCEED);
    ASSERT(ctx.message == 0);
    qcc_test_context_done(&ctx);
}

TEST(assume_false_skips)
{
    struct qcc_test_context ctx;
    qcc_test_context_init(&ctx);
    qcc_assume(&ctx, 0);
    ASSERT(ctx.result == QCC_TEST_SKIP);
    ASSERT(ctx.message == 0);
    qcc_test_context_done(&ctx);
}

TEST_SUITE(test_context)
{
    RUN_TEST(default_context);
    RUN_TEST(assert_true_succeeds);
    RUN_TEST(assert_false_fails);
    RUN_TEST(assert_with_format);
    RUN_TEST(assume_true_succeeds);
    RUN_TEST(assume_false_skips);
}
