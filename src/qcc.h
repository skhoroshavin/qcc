
#pragma once

enum qcc_test_result
{
    QCC_TEST_SUCCEED,
    QCC_TEST_FAIL,
    QCC_TEST_SKIP
};

struct qcc_test_context
{
    enum qcc_test_result result;
    const char *message;
};

void qcc_test_context_init(struct qcc_test_context *ctx);

struct qcc_test_stats
{
    int total;
    int failing;
};

void qcc_test_stats_init(struct qcc_test_stats *stats);

typedef void (*qcc_test_fn)(struct qcc_test_context *);
void qcc_run_test(struct qcc_test_stats *stats, const char *name,
                  qcc_test_fn test_fn);

#define TEST(name) void name(struct qcc_test_context *_ctx)
#define RUN_TEST(name) qcc_run_test(&_stats, #name, name)

void qcc_assert(struct qcc_test_context *ctx, int cond, const char *fmt, ...);
void qcc_assume(struct qcc_test_context *ctx, int cond);

#define ASSERT(cond)                                                           \
    do                                                                         \
    {                                                                          \
        qcc_assert(_ctx, cond,                                                 \
                   "Assertion \"%s\" failed in %s (%s, line %d)\n", #cond,     \
                   __FUNCTION__, __FILE__, __LINE__);                          \
        if (!(cond))                                                           \
            return;                                                            \
    } while (0)

#define ASSUME(cond)                                                           \
    do                                                                         \
    {                                                                          \
        qcc_assume(_ctx, cond);                                                \
        if (!(cond))                                                           \
            return;                                                            \
    } while (0)
