
#include "qcc.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

int qcc_main(int argc, const char *argv[], qcc_main_fn main_fn)
{
    (void)argc;
    (void)argv;

    char log_data[65536];
    struct qcc_logger logger;
    qcc_logger_init(&logger, log_data, sizeof(log_data));

    char arena_data[65536];
    struct qcc_engine eng;
    qcc_engine_init(&eng, arena_data, sizeof(arena_data), &logger);

    main_fn(&eng);

    printf("%s\nSummary: %u total tests run, %u tests failed\n", log_data,
           eng.total_tests, eng.failed_tests);

    int res = eng.failed_tests;

    return res;
}
