
#pragma once

#include "generator.h"

QCC_BEGIN_HEADER

struct qcc_array
{
    void *data;
    size_t size;
};

struct qcc_array_size
{
    size_t min;
    size_t max;
};

extern struct qcc_array_size qcc_array_any_size;
extern struct qcc_array_size qcc_array_non_empty;
struct qcc_array_size qcc_array_fixed_size(size_t size);
struct qcc_array_size qcc_array_smaller_than(size_t size);

qcc_generator_ptr qcc_gen_array_of(struct qcc_context *ctx,
                                   struct qcc_array_size size,
                                   qcc_generator_ptr item_gen);

QCC_END_HEADER
