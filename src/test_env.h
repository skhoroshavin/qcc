
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

struct qcc_test_env
{
    unsigned total_tests;
    unsigned failed_tests;
    unsigned total_asserts;
};

void qcc_test_env_init(struct qcc_test_env *env);

#ifdef __cplusplus
}
#endif
