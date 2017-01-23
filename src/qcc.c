
#include "qcc.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void qcc_run_test(struct qcc_test_env *env, const char *name,
                  qcc_test_fn test_fn)
{
    char arena_data[32768];
    struct qcc_arena arena;
    qcc_arena_init(&arena, arena_data, sizeof(arena_data));

    struct qcc_test_context ctx;
    qcc_test_context_init(&ctx, env, &arena);

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
    struct qcc_test_env local_env;
    qcc_test_env_init(&local_env);
    printf("Suite %s:\n", name);
    suite_fn(&local_env);
    printf("%d failing out of %d total tests\n", local_env.failed_tests,
           local_env.total_tests);
    env->total_tests += local_env.total_tests;
    env->failed_tests += local_env.failed_tests;
    env->total_asserts += local_env.total_asserts;
}

int qcc_main(int argc, const char *argv[], qcc_suite_fn main_suite)
{
    (void)argc;
    (void)argv;

    struct qcc_test_env env;
    qcc_test_env_init(&env);
    main_suite(&env);
    printf("Summary: %u total tests run, %u tests failed, %u asserts checked\n",
           env.total_tests, env.failed_tests, env.total_asserts);
    return env.failed_tests;
}
