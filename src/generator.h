
#pragma once

#include "test_context.h"

QCC_BEGIN_HEADER

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

struct qcc_generator *qcc_gen_value_from(struct qcc_test_context *ctx,
                                         const void *data, size_t size);
struct qcc_generator *qcc_gen_one_of(struct qcc_test_context *ctx, ...);

QCC_END_HEADER
