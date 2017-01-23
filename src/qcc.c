
#include "qcc.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void qcc_run_test(struct qcc_test_env *env, const char *name,
                  qcc_test_fn test_fn)
{
    struct qcc_test_context ctx;
    qcc_test_context_init(&ctx, env);

    for (unsigned i = 0; i < 1000; ++i)
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

            ++env->total_tests;
            ++env->failed_tests;
            break;
        }

        if (!ctx.is_randomized) break;
    }

    if (ctx.result == QCC_TEST_SUCCEED)
    {
        printf("  %s: PASSED\n", name);
        ++env->total_tests;
    }

    qcc_test_context_done(&ctx);
}

void qcc_run_suite(struct qcc_test_env *env, const char *name,
                   qcc_suite_fn suite_fn)
{
    printf("Suite %s:\n", name);
    suite_fn(env);
}

int qcc_main(int argc, const char *argv[], qcc_suite_fn main_suite)
{
    (void)argc;
    (void)argv;

    char arena_data[32768];
    struct qcc_test_env env;
    qcc_test_env_init(&env, arena_data, sizeof(arena_data));
    main_suite(&env);
    printf(
        "Summary: %u total tests run, %u tests failed, %u assertions checked\n",
        env.total_tests, env.failed_tests, env.total_asserts);

    int res = env.failed_tests;
    qcc_test_env_done(&env);

    return res;
}
