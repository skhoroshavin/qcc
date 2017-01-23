
#include "engine.h"
#include "test_context.h"
#include <stdio.h>

static void _log_proc(void *data, const char *msg)
{
    (void)data;
    printf("%s", msg);
}

void qcc_engine_init(struct qcc_engine *eng, void *buffer, size_t buf_size)
{
    eng->max_tries = 1000;
    eng->required_successes = 100;
    eng->log_data = 0;
    eng->log_proc = _log_proc;
    eng->total_tests = 0;
    eng->failed_tests = 0;
    qcc_arena_init(&eng->arena, buffer, buf_size);
}

void qcc_engine_done(struct qcc_engine *eng) { qcc_arena_done(&eng->arena); }

void qcc_engine_log(struct qcc_engine *eng, const char *fmt, ...)
{
    if (!eng->log_proc) return;

    char buf[1024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);

    eng->log_proc(eng->log_data, buf);
}

void qcc_engine_success(struct qcc_engine *eng, const char *name)
{
    ++eng->total_tests;
    qcc_engine_log(eng, "  %s: PASSED\n", name);
}

void qcc_engine_failure(struct qcc_engine *eng, const char *name)
{
    ++eng->failed_tests;
    ++eng->total_tests;
    qcc_engine_log(eng, "  %s: FAILED\n", name);
}

static enum qcc_test_result
_run_test_once(struct qcc_engine *eng, const char *name, qcc_test_fn test_fn)
{
    struct qcc_test_context ctx;
    qcc_test_context_init(&ctx, eng);
    test_fn(&ctx);
    enum qcc_test_result result = ctx.result;
    if (ctx.result == QCC_TEST_FAIL)
    {
        qcc_engine_failure(eng, name);
        for (struct qcc_test_param *param = ctx.param; param;
             param = param->next)
            qcc_engine_log(eng, "    %s\n", param->value);
        qcc_engine_log(eng, "    %s\n", ctx.error);
    }
    qcc_test_context_done(&ctx);
    return result;
}

void qcc_engine_run_test(struct qcc_engine *eng, const char *name,
                         qcc_test_fn test_fn)
{
    unsigned successes = 0;
    for (unsigned i = 0; i != eng->max_tries; ++i)
    {
        switch (_run_test_once(eng, name, test_fn))
        {
        case QCC_TEST_SUCCEED:
            ++successes;
            break;
        case QCC_TEST_FAIL:
            return;
        case QCC_TEST_SKIP:
            break;
        }

        if (successes == eng->required_successes) break;
    }

    if (successes == 0)
    {
        qcc_engine_failure(eng, name);
        qcc_engine_log(eng, "    No input was accepted by test as valid\n");
        return;
    }

    qcc_engine_success(eng, name);
    if (successes < eng->required_successes)
        qcc_engine_log(
            eng, "   Warning: Only %u successful runs out of %u were made\n",
            successes, eng->required_successes);
}
