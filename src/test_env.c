
#include "test_env.h"

void qcc_test_env_init(struct qcc_test_env *env, void *buffer, size_t buf_size)
{
    env->total_tests = 0;
    env->failed_tests = 0;
    env->total_asserts = 0;
    qcc_arena_init(&env->arena, buffer, buf_size);
}

void qcc_test_env_done(struct qcc_test_env *env)
{
    qcc_arena_done(&env->arena);
}
