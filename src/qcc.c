
#include "qcc.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void qcc_run_suite(struct qcc_engine *eng, const char *name,
                   qcc_suite_fn suite_fn)
{
    qcc_engine_log(eng, "Suite %s:\n", name);
    suite_fn(eng);
}

int qcc_main(int argc, const char *argv[], qcc_suite_fn main_suite)
{
    (void)argc;
    (void)argv;

    char arena_data[32768];
    struct qcc_engine eng;
    qcc_engine_init(&eng, arena_data, sizeof(arena_data));
    main_suite(&eng);
    qcc_engine_log(&eng, "Summary: %u total tests run, %u tests failed\n",
                   eng.total_tests, eng.failed_tests);

    int res = eng.failed_tests;
    qcc_engine_done(&eng);

    return res;
}
