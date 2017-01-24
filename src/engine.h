
#pragma once

#include "arena.h"

QCC_BEGIN_HEADER

struct qcc_context;

struct qcc_engine
{
    /* Settings */
    unsigned max_tries;
    unsigned required_successes;

    /* Logger */
    void *log_data;
    void (*log_proc)(void *data, const char *msg);

    /* Statistics */
    unsigned total_tests;
    unsigned failed_tests;

    struct qcc_arena arena;
};

typedef void (*qcc_test_fn)(struct qcc_context *);

void qcc_engine_init(struct qcc_engine *eng, void *buffer, size_t buf_size);
void qcc_engine_done(struct qcc_engine *eng);
void qcc_engine_log(struct qcc_engine *eng, const char *fmt, ...) QCC_PRINTF;
void qcc_engine_failure(struct qcc_engine *eng, const char *name);
void qcc_engine_run_test(struct qcc_engine *eng, const char *name,
                         qcc_test_fn test_fn);

QCC_END_HEADER
