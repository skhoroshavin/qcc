
#include "qcc.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void qcc_test_stats_init(struct qcc_test_stats *stats)
{
    stats->total = 0;
    stats->failing = 0;
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
        printf("  %s: PASSED\n", name);
        ++stats->total;
        break;
    case QCC_TEST_FAIL:
        printf("  %s: FAILED\n", name);
        printf("    %s\n", ctx.error);
        ++stats->total;
        ++stats->failing;
        break;
    case QCC_TEST_SKIP:
        break;
    }
    qcc_test_context_done(&ctx);
}

void qcc_run_suite(struct qcc_test_stats *stats, const char *name,
                   qcc_suite_fn suite_fn)
{
    struct qcc_test_stats local_stats;
    qcc_test_stats_init(&local_stats);
    printf("Suite %s:\n", name);
    suite_fn(&local_stats);
    printf("%d failing out of %d total tests\n", local_stats.failing,
           local_stats.total);
    stats->total += local_stats.total;
    stats->failing += local_stats.failing;
}

int qcc_main(int argc, const char *argv[])
{
    (void)argc;
    (void)argv;

    struct qcc_test_stats stats;
    qcc_test_stats_init(&stats);
    qcc_test_main(&stats);
    printf("Summary: %d total tests run, %d tests failed\n", stats.total,
           stats.failing);
    return stats.failing;
}
