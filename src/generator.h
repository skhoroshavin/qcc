
#pragma once

#include "context.h"

QCC_BEGIN_HEADER

struct qcc_generator;
typedef void (*qcc_generate_fn)(struct qcc_generator *self, void *data);

struct qcc_generator
{
    struct qcc_context *context;
    size_t type_size;
    qcc_generate_fn generate;
};

void qcc_generate(struct qcc_generator *self, void *data);

struct qcc_generator *qcc_gen_value_from(struct qcc_context *ctx,
                                         size_t item_size, size_t item_stride,
                                         const void *data, size_t count);
struct qcc_generator *qcc_gen_one_of(struct qcc_context *ctx, ...);

typedef void (*qcc_transform_fn)(const void *params, const void *src,
                                 const void *dst);
struct qcc_generator *qcc_gen_transform(struct qcc_context *ctx,
                                        size_t item_size,
                                        struct qcc_generator *src_gen,
                                        qcc_transform_fn transform,
                                        const void *params);

QCC_END_HEADER
