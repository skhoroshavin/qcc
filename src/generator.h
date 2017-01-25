
#pragma once

#include "context.h"

QCC_BEGIN_HEADER

struct qcc_generator;
typedef void (*qcc_generate_fn)(struct qcc_generator *self,
                                struct qcc_context *ctx, void *data);

struct qcc_generator
{
    size_t type_size;
    qcc_generate_fn generate;
};

void qcc_generate(struct qcc_generator *self, struct qcc_context *ctx,
                  void *data);

struct qcc_generator *qcc_gen_value_from(struct qcc_context *ctx,
                                         size_t item_size, const void *data,
                                         size_t count);
struct qcc_generator *qcc_gen_one_of(struct qcc_context *ctx, ...);

QCC_END_HEADER
