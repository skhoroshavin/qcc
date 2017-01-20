
#pragma once

#include "test_context.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

struct qcc_test_gen
{
    size_t size;
    size_t align;
    void (*example)(struct qcc_test_gen *self, struct qcc_test_context *ctx,
                    void *data);
};

void qcc_test_gen_example(struct qcc_test_gen *self,
                          struct qcc_test_context *ctx, void *data);

#ifdef __cplusplus
}
#endif // __cplusplus
