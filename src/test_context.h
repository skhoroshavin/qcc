
#pragma once

#include "arena.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

enum qcc_test_result
{
    QCC_TEST_SUCCEED = 0,
    QCC_TEST_FAIL,
    QCC_TEST_SKIP
};

struct qcc_test_context
{
    enum qcc_test_result result;
    const char *error;
    struct qcc_arena arena;
};

void qcc_test_context_init(struct qcc_test_context *ctx);
void qcc_test_context_done(struct qcc_test_context *ctx);

unsigned qcc_test_context_rand(struct qcc_test_context *ctx);

#ifdef __cplusplus
}
#endif // __cplusplus
