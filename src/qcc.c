
#include "qcc.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

int qcc_main(int argc, const char *argv[], qcc_main_fn main_fn)
{
    (void)argc;
    (void)argv;

    const size_t arena_size = 4 * 1024 * 1024;
    const size_t max_log_size = 65536;

    char *log_data = (char *)malloc(max_log_size);
    struct qcc_logger logger;
    qcc_logger_init(&logger, log_data, max_log_size);

    char *arena_data = (char *)malloc(arena_size);
    struct qcc_engine eng;
    qcc_engine_init(&eng, arena_data, arena_size, &logger);

    main_fn(&eng);

    printf("%s\nSummary: %u total tests run, %u tests failed\n", log_data,
           eng.total_tests, eng.failed_tests);

    free(log_data);
    free(arena_data);

    return eng.failed_tests;
}
