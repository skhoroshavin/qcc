
#include "test_context.h"
#include <stdlib.h>

void qcc_test_context_init(struct qcc_test_context *ctx)
{
    ctx->result = QCC_TEST_SUCCEED;
    ctx->message = 0;
}

void qcc_test_context_done(struct qcc_test_context *ctx)
{
    if (ctx->message)
    {
        free(ctx->message);
        ctx->message = 0;
    }
}
