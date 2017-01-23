
#pragma once

#include "arena.h"

#ifdef __cplusplus
extern "C" {
#endif

struct qcc_test_env
{
    unsigned total_tests;
    unsigned failed_tests;
    unsigned total_asserts;
    struct qcc_arena arena;
};

void qcc_test_env_init(struct qcc_test_env *env, void *buffer, size_t buf_size);
void qcc_test_env_done(struct qcc_test_env *env);

#ifdef __cplusplus
}
#endif
