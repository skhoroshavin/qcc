
#include "test_env.h"
#include "test_context.h"
#include <stdio.h>

void qcc_test_env_init(struct qcc_test_env *env, void *buffer, size_t buf_size)
{
    env->max_tries = 1000;
    env->required_successes = 100;
    env->total_tests = 0;
    env->failed_tests = 0;
    qcc_arena_init(&env->arena, buffer, buf_size);
}

void qcc_test_env_done(struct qcc_test_env *env)
{
    qcc_arena_done(&env->arena);
}

void qcc_test_env_report_success(struct qcc_test_env *env, const char *name)
{
    ++env->total_tests;
    printf("  %s: PASSED\n", name);
}

void qcc_test_env_report_failure(struct qcc_test_env *env, const char *name)
{
    ++env->failed_tests;
    ++env->total_tests;
    printf("  %s: FAILED\n", name);
}

static enum qcc_test_result
_run_test_once(struct qcc_test_env *env, const char *name, qcc_test_fn test_fn)
{
    struct qcc_test_context ctx;
    qcc_test_context_init(&ctx, env);
    test_fn(&ctx);
    enum qcc_test_result result = ctx.result;
    if (ctx.result == QCC_TEST_FAIL)
    {
        qcc_test_env_report_failure(env, name);
        for (struct qcc_test_param *param = ctx.param; param;
             param = param->next)
            printf("    %s\n", param->value);
        printf("    %s\n", ctx.error);
    }
    qcc_test_context_done(&ctx);
    return result;
}

void qcc_test_env_run_test(struct qcc_test_env *env, const char *name,
                           qcc_test_fn test_fn)
{
    unsigned successes = 0;
    for (unsigned i = 0; i != env->max_tries; ++i)
    {
        switch (_run_test_once(env, name, test_fn))
        {
        case QCC_TEST_SUCCEED:
            ++successes;
            break;
        case QCC_TEST_FAIL:
            return;
        case QCC_TEST_SKIP:
            break;
        }

        if (successes == env->required_successes) break;
    }

    if (successes == 0)
    {
        qcc_test_env_report_failure(env, name);
        printf("    No input was accepted by test as valid\n");
        return;
    }

    qcc_test_env_report_success(env, name);
    if (successes < env->required_successes)
        printf("   Warning: Only %u successful runs out of %u were made\n",
               successes, env->required_successes);
}
