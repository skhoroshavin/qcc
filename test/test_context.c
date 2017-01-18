
#include "qcc.h"

TEST(default_context)
{
    struct qcc_test_context ctx;
    qcc_test_context_init(&ctx);
    ASSERT(ctx.result == QCC_TEST_SUCCEED);
    ASSERT(ctx.error == 0);
    qcc_test_context_done(&ctx);
}

TEST_SUITE(test_context) { RUN_TEST(default_context); }
