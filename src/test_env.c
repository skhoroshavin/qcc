
#include "test_env.h"

void qcc_test_env_init(struct qcc_test_env *env)
{
    env->total_tests = 0;
    env->failed_tests = 0;
    env->total_asserts = 0;
}
