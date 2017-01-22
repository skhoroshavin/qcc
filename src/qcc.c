
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
    char arena_data[32768];
    struct qcc_arena arena;
    qcc_arena_init(&arena, arena_data, sizeof(arena_data));

    struct qcc_test_context ctx;
    qcc_test_context_init(&ctx, &arena);

    for (unsigned i = 0; i < 200; ++i)
    {
        qcc_test_context_reset(&ctx);
        test_fn(&ctx);
        if (ctx.result == QCC_TEST_FAIL)
        {
            printf("  %s: FAILED\n", name);

            for (struct qcc_test_param *param = ctx.param; param;
                 param = param->next)
                printf("    %s\n", param->value);
            printf("    %s\n", ctx.error);

            ++stats->total;
            ++stats->failing;
            break;
        }

        if (!ctx.is_randomized) break;
    }

    if (ctx.result == QCC_TEST_SUCCEED)
    {
        printf("  %s: PASSED\n", name);
        ++stats->total;
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

int qcc_main(int argc, const char *argv[], qcc_suite_fn main_suite)
{
    (void)argc;
    (void)argv;

    struct qcc_test_stats stats;
    qcc_test_stats_init(&stats);
    main_suite(&stats);
    printf("Summary: %d total tests run, %d tests failed\n", stats.total,
           stats.failing);
    return stats.failing;
}
