
#pragma once

#include "generator.h"

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

struct qcc_generator *qcc_gen_array_of(struct qcc_test_context *ctx,
                                       struct qcc_array_size size,
                                       struct qcc_generator *elem_gen,
                                       size_t elem_size);
