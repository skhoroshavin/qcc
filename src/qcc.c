
#include "qcc.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

int qcc_main(int argc, const char *argv[], qcc_main_fn main_fn)
{
    (void)argc;
    (void)argv;

    const size_t arena_size = 4 * 1024 * 1024;
    char *arena_data = (char *)malloc(arena_size);

    struct qcc_arena arena;
    qcc_arena_init(&arena, arena_data, arena_size);

    struct qcc_engine eng;
    qcc_engine_init(&eng, &arena);

    main_fn(&eng);

    qcc_logger_dump(&eng.logger, "%s\n");
    printf("Summary: %u total tests run, %u tests failed\n", eng.total_tests,
           eng.failed_tests);

    qcc_arena_done(&arena);
    free(arena_data);

    return eng.failed_tests;
}
