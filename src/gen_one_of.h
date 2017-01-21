
#pragma once

#include "generator.h"

#ifdef __cplusplus
extern "C" {
#endif

struct qcc_generator *qcc_gen_one_of(struct qcc_test_context *ctx, ...);

#ifdef __cplusplus
}
#endif
