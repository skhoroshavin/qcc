
#pragma once

#include "qcc_uint.h"
#include <memory.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*qcc_suite_fn)(struct qcc_engine *);

int qcc_main(int argc, const char *argv[], qcc_suite_fn main_suite);

#define TEST(name) static void name(struct qcc_test_context *_ctx)
#define RUN_TEST(name) qcc_engine_run_test(_eng, #name, name)

#define TEST_SUITE(name) void name(struct qcc_engine *_eng)
#define RUN_SUITE(name) name(_eng)

#define TEST_MAIN()                                                            \
    void qcc_test_main(struct qcc_engine *_eng);                               \
    int main(int argc, const char *argv[])                                     \
    {                                                                          \
        return qcc_main(argc, argv, qcc_test_main);                            \
    }                                                                          \
    void qcc_test_main(struct qcc_engine *_eng)

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
