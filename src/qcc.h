
#pragma once

#include "qcc_uint.h"
#include <memory.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*qcc_test_fn)(struct qcc_test_context *);
typedef void (*qcc_suite_fn)(struct qcc_test_env *);

void qcc_run_test(struct qcc_test_env *env, const char *name,
                  qcc_test_fn test_fn);
void qcc_run_suite(struct qcc_test_env *env, const char *name,
                   qcc_suite_fn suite_fn);
int qcc_main(int argc, const char *argv[], qcc_suite_fn main_suite);

#define TEST(name) static void name(struct qcc_test_context *_ctx)
#define RUN_TEST(name) qcc_run_test(_env, #name, name)

#define TEST_SUITE(name) void name(struct qcc_test_env *_env)
#define RUN_SUITE(name) qcc_run_suite(_env, #name, name)

#define TEST_MAIN()                                                            \
    void qcc_test_main(struct qcc_test_env *_env);                             \
    int main(int argc, const char *argv[])                                     \
    {                                                                          \
        return qcc_main(argc, argv, qcc_test_main);                            \
    }                                                                          \
    void qcc_test_main(struct qcc_test_env *_env)

#define GIVEN_DATA(name, size)                                                 \
    void *name = qcc_arena_alloc(_ctx->arena, size);                           \
    qcc_test_context_rand(_ctx, name, size);

#define ASSUME(cond)                                                           \
    do                                                                         \
    {                                                                          \
        if (!(cond))                                                           \
        {                                                                      \
            _ctx->result = QCC_TEST_SKIP;                                      \
            return;                                                            \
        }                                                                      \
    } while (0)

#define ASSERT(cond)                                                           \
    do                                                                         \
    {                                                                          \
        ++_ctx->env->total_asserts;                                            \
        if (!(cond))                                                           \
        {                                                                      \
            qcc_test_context_fail(                                             \
                _ctx, "Assertion \"%s\" failed in %s (%s, line %d)\n", #cond,  \
                __FUNCTION__, __FILE__, __LINE__);                             \
            return;                                                            \
        }                                                                      \
    } while (0)

#define ASSERT_STR_EQ(got, expected)                                           \
    do                                                                         \
    {                                                                          \
        ++_ctx->env->total_asserts;                                            \
        if (strcmp(got, expected) != 0)                                        \
        {                                                                      \
            qcc_test_context_fail(                                             \
                _ctx,                                                          \
                "Assertion \"%s\" == \"%s\" failed in %s (%s, line %d)\n",     \
                (got), (expected), __FUNCTION__, __FILE__, __LINE__);          \
            return;                                                            \
        }                                                                      \
    } while (0)

#define ASSERT_MEM_EQ(got, expected, size)                                     \
    do                                                                         \
    {                                                                          \
        ++_ctx->env->total_asserts;                                            \
        if (memcmp(got, expected, size) != 0)                                  \
        {                                                                      \
            qcc_test_context_fail(                                             \
                _ctx, "Assertion \"%s == %s\" failed in %s (%s, line %d)\n",   \
                #got, #expected, __FUNCTION__, __FILE__, __LINE__);            \
            return;                                                            \
        }                                                                      \
    } while (0)

#ifdef __cplusplus
}
#endif
