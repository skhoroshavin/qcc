
#pragma once

#include "test_context.h"

#ifdef __cplusplus
extern "C" {
#endif

struct qcc_generator;
typedef void (*qcc_generate_fn)(struct qcc_generator *self,
                                struct qcc_test_context *ctx, void *data,
                                size_t size);

struct qcc_generator
{
    qcc_generate_fn generate;
};

void qcc_generate(struct qcc_generator *self, struct qcc_test_context *ctx,
                  void *data, size_t size);

struct qcc_array
{
    void *data;
    size_t size;
};

struct qcc_array_params
{
    size_t min_size;
    size_t max_size;
};

struct qcc_array_params qcc_array_any();
struct qcc_array_params qcc_array_non_empty();

struct qcc_generator *qcc_gen_value(struct qcc_test_context *ctx,
                                    const void *data, size_t size);
struct qcc_generator *qcc_gen_one_of(struct qcc_test_context *ctx, ...);
struct qcc_generator *qcc_gen_array_of(struct qcc_test_context *ctx,
                                       struct qcc_generator *elem_gen,
                                       size_t elem_size,
                                       struct qcc_array_params params);

#ifdef __cplusplus
}
#endif
