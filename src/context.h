
#pragma once

#include "engine.h"
#include "stream.h"

QCC_BEGIN_HEADER

enum qcc_test_result
{
    QCC_TEST_SUCCEED = 0,
    QCC_TEST_FAIL,
    QCC_TEST_SKIP,
    QCC_TEST_OVERRUN
};

struct qcc_test_param
{
    const char *value;
    struct qcc_test_param *next;
};

struct qcc_context
{
    enum qcc_test_result result;
    const char *error;
    struct qcc_stream *stream;
    struct qcc_test_param *param;
    struct qcc_test_param *last_param;
    struct qcc_arena *arena;
};

void qcc_context_init(struct qcc_context *ctx, struct qcc_stream *stream,
                      struct qcc_arena *arena);
void qcc_context_fail(struct qcc_context *ctx, const char *fmt, ...) QCC_PRINTF;
void qcc_context_register_param(struct qcc_context *ctx, const char *fmt,
                                ...) QCC_PRINTF;

QCC_END_HEADER
