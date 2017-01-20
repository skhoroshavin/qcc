
#include "test_context.h"
#include <memory.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void qcc_test_context_init(struct qcc_test_context *ctx)
{
    ctx->result = QCC_TEST_SUCCEED;
    ctx->error = 0;
    ctx->is_randomized = 0;
    ctx->param = 0;
    qcc_arena_init(&ctx->arena, 1024);
}

void qcc_test_context_done(struct qcc_test_context *ctx)
{
    qcc_arena_done(&ctx->arena);
}

void qcc_test_context_fail(struct qcc_test_context *ctx, const char *fmt, ...)
{
    ctx->result = QCC_TEST_FAIL;
    va_list args;
    va_start(args, fmt);
    ctx->error = qcc_arena_vsprintf(&ctx->arena, fmt, args);
    va_end(args);
}

void qcc_test_context_reset(struct qcc_test_context *ctx)
{
    ctx->result = QCC_TEST_SUCCEED;
    ctx->error = 0;
    ctx->is_randomized = 0;
    ctx->param = 0;
    qcc_arena_reset(&ctx->arena);
}

unsigned qcc_test_context_rand(struct qcc_test_context *ctx)
{
    ctx->is_randomized = 1;
    return rand();
}

void qcc_test_context_register_param(struct qcc_test_context *ctx,
                                     const char *fmt, ...)
{
    struct qcc_test_param *param = (struct qcc_test_param *)qcc_arena_alloc(
        &ctx->arena, sizeof(struct qcc_test_param));

    va_list args;
    va_start(args, fmt);
    param->value = qcc_arena_vsprintf(&ctx->arena, fmt, args);
    va_end(args);

    param->next = ctx->param;
    ctx->param = param;
}
