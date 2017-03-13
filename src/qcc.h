
#pragma once

#include "qcc_uint.h"
#include <memory.h>
#include <string.h>

QCC_BEGIN_HEADER

typedef void (*qcc_main_fn)(struct qcc_engine *);

int qcc_main(int argc, const char *argv[], qcc_main_fn main_fn);

#define TEST(name) static void name(struct qcc_context *_ctx)
#define TEST_GROUP(name) void name(struct qcc_engine *_eng)
#define RUN_TEST(name) qcc_engine_run_test(_eng, #name, name)
#define RUN_GROUP(name) name(_eng)

#define TEST_P(name, param) TEST(PP_CONCAT(name##_, param))
#define TEST_GROUP_P(name, param) TEST_GROUP(PP_CONCAT(name##_, param))
#define RUN_TEST_P(name, param) RUN_TEST(PP_CONCAT(name##_, param))
#define RUN_GROUP_P(name, param) RUN_GROUP(PP_CONCAT(name##_, param))

#define TEST_MAIN()                                                            \
    void qcc_test_main(struct qcc_engine *_eng);                               \
    int main(int argc, const char *argv[])                                     \
    {                                                                          \
        return qcc_main(argc, argv, qcc_test_main);                            \
    }                                                                          \
    void qcc_test_main(struct qcc_engine *_eng)

#define GIVEN_DATA(name, size)                                                 \
    void *name = qcc_arena_alloc(_ctx->arena, size);                           \
    qcc_stream_read(_ctx->stream, name, size);

#define FAIL(fmt, ...)                                                         \
    do                                                                         \
    {                                                                          \
        const char *msg = qcc_arena_sprintf(_ctx->arena, fmt, ##__VA_ARGS__);  \
        qcc_context_fail(_ctx, "\"%s\" in %s (%s, line %d)\n", msg,            \
                         __FUNCTION__, __FILE__, __LINE__);                    \
        return;                                                                \
    } while (0)

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
            qcc_context_fail(_ctx,                                             \
                             "Assertion \"%s\" failed in %s (%s, line %d)\n",  \
                             #cond, __FUNCTION__, __FILE__, __LINE__);         \
            return;                                                            \
        }                                                                      \
    } while (0)

#define ASSERT_STR_EQ(got, expected)                                           \
    do                                                                         \
    {                                                                          \
        if (strcmp(got, expected) != 0)                                        \
        {                                                                      \
            qcc_context_fail(                                                  \
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
            qcc_context_fail(                                                  \
                _ctx, "Assertion \"%s == %s\" failed in %s (%s, line %d)\n",   \
                #got, #expected, __FUNCTION__, __FILE__, __LINE__);            \
            return;                                                            \
        }                                                                      \
    } while (0)

QCC_END_HEADER
