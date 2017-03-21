
#pragma once

#include "arena.h"
#include "logger.h"

QCC_BEGIN_HEADER

struct qcc_context;

struct qcc_engine
{
    /* Arena */
    struct qcc_arena arena[2];
    struct qcc_arena *active_arena;
    struct qcc_arena *backup_arena;

    /* Settings */
    unsigned max_tries;
    unsigned required_successes;

    /* Statistics */
    unsigned total_tests;
    unsigned failed_tests;

    /* Internal */
    struct qcc_logger logger;
};

typedef void (*qcc_test_fn)(struct qcc_context *);

void qcc_engine_init(struct qcc_engine *eng, struct qcc_arena *arena);
void qcc_engine_failure(struct qcc_engine *eng);
void qcc_engine_run_test(struct qcc_engine *eng, const char *name,
                         qcc_test_fn test_fn);

QCC_END_HEADER
