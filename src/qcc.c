
#include "qcc.h"

#include <stdarg.h>
#include <stdio.h>

void qcc_test_context_init(struct qcc_test_context *ctx)
{
    ctx->result = QCC_TEST_SUCCEED;
    ctx->message = 0;
}

void qcc_test_stats_init(struct qcc_test_stats *stats)
{
    stats->total = 0;
    stats->failing = 0;
}

void qcc_assert(struct qcc_test_context *ctx, int cond, const char *fmt, ...)
{
    if (ctx->result != QCC_TEST_SUCCEED)
        return;
    if (!cond)
    {
        ctx->result = QCC_TEST_FAIL;
        va_list args;
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
    }
}

void qcc_assume(struct qcc_test_context *ctx, int cond)
{
    if (ctx->result != QCC_TEST_SUCCEED)
        return;
    if (!cond)
        ctx->result = QCC_TEST_SKIP;
}

void qcc_run_test(struct qcc_test_stats *stats, const char *name,
                  qcc_test_fn test_fn)
{
    struct qcc_test_context ctx;
    qcc_test_context_init(&ctx);
    test_fn(&ctx);
    switch (ctx.result)
    {
    case QCC_TEST_SUCCEED:
        ++stats->total;
        break;
    case QCC_TEST_FAIL:
        ++stats->total;
        ++stats->failing;
        printf("Failed test %s\n", name);
        break;
    case QCC_TEST_SKIP:
        break;
    }
}
