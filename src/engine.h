
#pragma once

#include "arena.h"
#include "logger.h"

QCC_BEGIN_HEADER

struct qcc_context;

struct qcc_engine
{
    /* Settings */
    unsigned max_tries;
    unsigned required_successes;

    /* Statistics */
    unsigned total_tests;
    unsigned failed_tests;

    /* Buffer */
    void *buf_data;
    size_t buf_size;

    /* Logger */
    struct qcc_logger *logger;
};

typedef void (*qcc_test_fn)(struct qcc_context *);

void qcc_engine_init(struct qcc_engine *eng, void *buffer, size_t buf_size,
                     struct qcc_logger *logger);
void qcc_engine_failure(struct qcc_engine *eng);
void qcc_engine_run_test(struct qcc_engine *eng, const char *name,
                         qcc_test_fn test_fn);

QCC_END_HEADER
