
#include "qcc.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

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
    printf("Summary: %u total tests run, %u tests failed\n", env.total_tests,
           env.failed_tests);

    int res = env.failed_tests;
    qcc_test_env_done(&env);

    return res;
}
