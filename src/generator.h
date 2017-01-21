
#pragma once

#include "test_context.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

struct qcc_generator;
typedef void (*qcc_gen_example_fn)(struct qcc_generator *self,
                                   struct qcc_test_context *ctx, void *data);

struct qcc_generator
{
    size_t size;
    qcc_gen_example_fn example;
};

void qcc_gen_example(struct qcc_generator *self, struct qcc_test_context *ctx,
                     void *data);

#ifdef __cplusplus
}
#endif // __cplusplus
