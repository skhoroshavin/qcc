
#pragma once

#include "test_context.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

struct qcc_test_gen;
typedef void (*qcc_test_gen_example_fn)(struct qcc_test_gen *self,
                                        struct qcc_test_context *ctx,
                                        void *data);

struct qcc_test_gen
{
    size_t size;
    qcc_test_gen_example_fn example;
};

void qcc_test_gen_example(struct qcc_test_gen *self,
                          struct qcc_test_context *ctx, void *data);

#ifdef __cplusplus
}
#endif // __cplusplus
