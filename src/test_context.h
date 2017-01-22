
#pragma once

#include "arena.h"

#ifdef __cplusplus
extern "C" {
#endif

enum qcc_test_result
{
    QCC_TEST_SUCCEED = 0,
    QCC_TEST_FAIL,
    QCC_TEST_SKIP
};

struct qcc_test_param
{
    const char *value;
    struct qcc_test_param *next;
};

struct qcc_test_context
{
    enum qcc_test_result result;
    const char *error;
    int is_randomized;
    struct qcc_test_param *param;
    struct qcc_test_param *last_param;
    struct qcc_arena *arena;
};

void qcc_test_context_init(struct qcc_test_context *ctx,
                           struct qcc_arena *arena);
void qcc_test_context_done(struct qcc_test_context *ctx);
void qcc_test_context_fail(struct qcc_test_context *ctx, const char *fmt, ...);
void qcc_test_context_reset(struct qcc_test_context *ctx);
void qcc_test_context_rand(struct qcc_test_context *ctx, void *data,
                           size_t size);
unsigned qcc_test_context_rand_value(struct qcc_test_context *ctx);
void qcc_test_context_register_param(struct qcc_test_context *ctx,
                                     const char *fmt, ...);

#ifdef __cplusplus
}
#endif
