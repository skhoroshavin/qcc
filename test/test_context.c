
#include "qcc.h"

#define GIVEN_TEST_CONTEXT(name)                                               \
    struct qcc_test_context *name =                                            \
        qcc_arena_alloc(&_ctx->arena, sizeof(struct qcc_test_context));        \
    qcc_arena_add_object(&_ctx->arena, name,                                   \
                         (qcc_destroy_fn)qcc_test_context_done);               \
    qcc_test_context_init(name);

TEST(empty_context)
{
    GIVEN_TEST_CONTEXT(ctx);

    ASSERT(ctx->result == QCC_TEST_SUCCEED);
    ASSERT(ctx->error == 0);
}

TEST_SUITE(test_context) { RUN_TEST(empty_context); }
