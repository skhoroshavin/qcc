
#pragma once

enum qcc_test_result
{
    QCC_TEST_SUCCEED = 0,
    QCC_TEST_FAIL,
    QCC_TEST_SKIP
};

struct qcc_test_context
{
    enum qcc_test_result result;
    char *error;
};

void qcc_test_context_init(struct qcc_test_context *ctx);
void qcc_test_context_done(struct qcc_test_context *ctx);

void qcc_test_context_fail(struct qcc_test_context *ctx, const char *fmt, ...);
