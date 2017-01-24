
#include "qcc.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

int qcc_main(int argc, const char *argv[], qcc_suite_fn main_suite)
{
    (void)argc;
    (void)argv;

    char arena_data[65536];
    struct qcc_engine eng;
    qcc_engine_init(&eng, arena_data, sizeof(arena_data));
    main_suite(&eng);
    qcc_engine_log(&eng, "Summary: %u total tests run, %u tests failed\n",
                   eng.total_tests, eng.failed_tests);

    int res = eng.failed_tests;
    qcc_engine_done(&eng);

    return res;
}
