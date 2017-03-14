
#include "context.h"
#include <memory.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void qcc_context_init(struct qcc_context *ctx, struct qcc_stream *stream,
                      struct qcc_arena *arena)
{
    ctx->result = QCC_TEST_SUCCEED;
    ctx->error = 0;
    ctx->stream = stream;
    ctx->param = 0;
    ctx->last_param = 0;
    ctx->arena = arena;
}

void qcc_context_fail(struct qcc_context *ctx, const char *fmt, ...)
{
    ctx->result = QCC_TEST_FAIL;
    va_list args;
    va_start(args, fmt);
    ctx->error = qcc_arena_vsprintf(ctx->arena, fmt, args);
    va_end(args);
}

void qcc_context_register_param(struct qcc_context *ctx, const char *fmt, ...)
{
    QCC_ARENA_POD(ctx->arena, qcc_test_param, param);
    param->next = 0;

    va_list args;
    va_start(args, fmt);
    param->value = qcc_arena_vsprintf(ctx->arena, fmt, args);
    va_end(args);

    if (ctx->param)
    {
        ctx->last_param->next = param;
        ctx->last_param = param;
    }
    else
    {
        ctx->param = param;
        ctx->last_param = param;
    }
}
