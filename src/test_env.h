
#pragma once

#include "arena.h"

#ifdef __cplusplus
extern "C" {
#endif

struct qcc_test_context;

struct qcc_test_env
{
    /* Settings */
    unsigned max_tries;
    unsigned required_successes;

    /* Statistics */
    unsigned total_tests;
    unsigned failed_tests;

    struct qcc_arena arena;
};

typedef void (*qcc_test_fn)(struct qcc_test_context *);

void qcc_test_env_init(struct qcc_test_env *env, void *buffer, size_t buf_size);
void qcc_test_env_done(struct qcc_test_env *env);
void qcc_test_env_report_success(struct qcc_test_env *env, const char *name);
void qcc_test_env_report_failure(struct qcc_test_env *env, const char *name);
void qcc_test_env_run_test(struct qcc_test_env *env, const char *name,
                           qcc_test_fn test_fn);

#ifdef __cplusplus
}
#endif
