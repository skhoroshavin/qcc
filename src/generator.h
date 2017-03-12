
#pragma once

#include "context.h"

QCC_BEGIN_HEADER

struct qcc_generator;
typedef const struct qcc_generator *qcc_generator_ptr;
typedef void (*qcc_generate_fn)(qcc_generator_ptr self, void *data);

struct qcc_generator
{
    struct qcc_context *context;
    size_t type_size;
    qcc_generate_fn generate;
};

void qcc_generate(qcc_generator_ptr self, void *data);

qcc_generator_ptr qcc_gen_value_from(struct qcc_context *ctx, size_t item_size,
                                     size_t item_stride, const void *data,
                                     size_t count);
qcc_generator_ptr qcc_gen_one_of(struct qcc_context *ctx, ...);

typedef void (*qcc_transform_fn)(const void *params, const void *src_data,
                                 size_t src_size, void *dst_data,
                                 size_t dst_size);
qcc_generator_ptr qcc_gen_transform(struct qcc_context *ctx, size_t item_size,
                                    qcc_generator_ptr src_gen,
                                    qcc_transform_fn transform,
                                    const void *params);

QCC_END_HEADER
