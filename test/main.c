
#include "qcc.h"

TEST(default_context)
{
    struct qcc_test_context ctx;
    qcc_test_context_init(&ctx);
    ASSERT(ctx.result == QCC_TEST_SUCCEED);
    ASSERT(ctx.message == 0);
}

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    struct qcc_test_stats _stats;
    qcc_test_stats_init(&_stats);
    RUN_TEST(default_context);
    return _stats.failing;
}
