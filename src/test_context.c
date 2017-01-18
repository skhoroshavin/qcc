
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
}
