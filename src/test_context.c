
#include "test_context.h"
#include <memory.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void qcc_test_context_init(struct qcc_test_context *ctx)
{
    ctx->result = QCC_TEST_SUCCEED;
    ctx->error = 0;
    qcc_arena_init(&ctx->arena, 1024);
}

void qcc_test_context_done(struct qcc_test_context *ctx)
{
    qcc_arena_done(&ctx->arena);

    if (ctx->error)
    {
        free(ctx->error);
        ctx->error = 0;
    }
}

void qcc_test_context_fail(struct qcc_test_context *ctx, const char *fmt, ...)
{
    ctx->result = QCC_TEST_FAIL;

    unsigned msg_len = 2 * strlen(fmt);
    ctx->error = malloc(msg_len);
    while (1)
    {
        va_list args;
        va_start(args, fmt);
        unsigned real_len = vsnprintf(ctx->error, msg_len, fmt, args);
        va_end(args);

        if (real_len < msg_len) break;

        msg_len *= 2;
        ctx->error = realloc(ctx->error, msg_len);
    }
}
