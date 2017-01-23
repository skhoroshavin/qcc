
#include "test_context.h"
#include <memory.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void qcc_test_context_init(struct qcc_test_context *ctx,
                           struct qcc_test_env *env)
{
    ctx->result = QCC_TEST_SUCCEED;
    ctx->error = 0;
    ctx->is_randomized = 0;
    ctx->param = 0;
    ctx->last_param = 0;
    ctx->env = env;
    ctx->arena = &env->arena;
}

void qcc_test_context_done(struct qcc_test_context *ctx)
{
    qcc_arena_reset(ctx->arena);
}

void qcc_test_context_fail(struct qcc_test_context *ctx, const char *fmt, ...)
{
    ctx->result = QCC_TEST_FAIL;
    va_list args;
    va_start(args, fmt);
    ctx->error = qcc_arena_vsprintf(ctx->arena, fmt, args);
    va_end(args);
}

void qcc_test_context_reset(struct qcc_test_context *ctx)
{
    ctx->result = QCC_TEST_SUCCEED;
    ctx->error = 0;
    ctx->is_randomized = 0;
    ctx->param = 0;
    ctx->last_param = 0;
    qcc_arena_reset(ctx->arena);
}

void qcc_test_context_rand(struct qcc_test_context *ctx, void *data,
                           size_t size)
{
    ctx->is_randomized = 1;
    uint8_t *byte = data;
    for (size_t i = 0; i != size; ++i)
        byte[i] = rand();
}

unsigned qcc_test_context_rand_value(struct qcc_test_context *ctx)
{
    unsigned res;
    qcc_test_context_rand(ctx, &res, sizeof(res));
    return res;
}

void qcc_test_context_register_param(struct qcc_test_context *ctx,
                                     const char *fmt, ...)
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
