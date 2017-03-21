
#include "engine.h"
#include "context.h"
#include "logger.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void qcc_engine_init(struct qcc_engine *eng, struct qcc_arena *arena)
{
    size_t data_size = qcc_arena_memory_available(arena) / 3;

    void *data = qcc_arena_alloc(arena, data_size);
    qcc_arena_init(eng->arena + 0, data, data_size);

    data = qcc_arena_alloc(arena, data_size);
    qcc_arena_init(eng->arena + 1, data, data_size);

    eng->active_arena = eng->arena + 0;
    eng->backup_arena = eng->arena + 1;

    eng->max_tries = 1000;
    eng->required_successes = 100;
    eng->total_tests = 0;
    eng->failed_tests = 0;

    qcc_logger_init(&eng->logger, arena);
}

void qcc_engine_failure(struct qcc_engine *eng)
{
    ++eng->failed_tests;
    ++eng->total_tests;
}

static enum qcc_test_result
_run_test_once(struct qcc_engine *eng, const char *name, qcc_test_fn test_fn,
               struct qcc_stream *stream, struct qcc_logger *logger)
{
    struct qcc_context ctx;
    qcc_context_init(&ctx, stream, eng->active_arena);
    test_fn(&ctx);
    enum qcc_test_result result = ctx.result;
    if (stream->is_overrun) result = QCC_TEST_OVERRUN;
    if (result == QCC_TEST_FAIL)
    {
        qcc_logger_printf(logger, "%s: FAILED", name);
        qcc_logger_append(logger, "    %s", &ctx.logger);
        qcc_logger_printf(logger, "    %s", ctx.error);
    }
    return result;
}

static unsigned _stream_skip_interval(qcc_uint *value)
{
    (void)value;
    return 1;
}
static unsigned _stream_zero_interval(qcc_uint *value)
{
    *value = 0;
    return 0;
}
static unsigned _stream_halve_interval(qcc_uint *value)
{
    *value >>= 1;
    return 0;
}
static unsigned _stream_dec_interval(qcc_uint *value)
{
    if (*value) *value -= 1;
    return 0;
}

static void _swap_arenas(struct qcc_engine *eng)
{
    struct qcc_arena *tmp = eng->backup_arena;
    eng->backup_arena = eng->active_arena;
    eng->active_arena = tmp;
}

static enum qcc_test_result _run_test_and_shrink(struct qcc_engine *eng,
                                                 const char *name,
                                                 qcc_test_fn test_fn)
{
    static const qcc_stream_data_process_fn try_shrink[] = {
        _stream_skip_interval, _stream_zero_interval, _stream_halve_interval,
        _stream_dec_interval};

    qcc_arena_reset(eng->active_arena);

    struct qcc_stream stream;
    qcc_stream_init(&stream, 0, eng->active_arena);

    struct qcc_logger logger;
    qcc_logger_init(&logger, eng->active_arena);

    enum qcc_test_result result =
        _run_test_once(eng, name, test_fn, &stream, &logger);
    if (result != QCC_TEST_FAIL) return result;

    qcc_engine_failure(eng);
    _swap_arenas(eng);

    while (1)
    {
        int found_interesting = 0;
        qcc_arena_reset(eng->active_arena);

        for (size_t num = 0; num != countof(try_shrink); ++num)
        {
            for (struct qcc_stream_interval *interval =
                     stream.interval_data.first;
                 interval; interval = interval->next)
            {
                struct qcc_stream_data shrinked_data = qcc_stream_data_process(
                    stream.data, eng->active_arena, interval->begin,
                    interval->end, try_shrink[num]);
                if (!memcmp(&stream.data, &shrinked_data, sizeof(stream.data)))
                    continue;

                struct qcc_stream tmp_stream;
                qcc_stream_init(&tmp_stream, &shrinked_data, eng->active_arena);

                struct qcc_logger tmp_logger;
                qcc_logger_init(&tmp_logger, eng->active_arena);

                if (_run_test_once(eng, name, test_fn, &tmp_stream,
                                   &tmp_logger) == QCC_TEST_FAIL)
                {
                    found_interesting = 1;
                    stream = tmp_stream;
                    logger = tmp_logger;
                    _swap_arenas(eng);
                    break;
                }
            }
            if (found_interesting) break;
        }
        if (found_interesting) continue;

        qcc_logger_append(&eng->logger, "%s", &logger);
        return QCC_TEST_FAIL;
    }
}

void qcc_engine_run_test(struct qcc_engine *eng, const char *name,
                         qcc_test_fn test_fn)
{
    unsigned successes = 0;
    for (unsigned i = 0; i != eng->max_tries; ++i)
    {
        switch (_run_test_and_shrink(eng, name, test_fn))
        {
        case QCC_TEST_SUCCEED:
            ++successes;
            break;
        case QCC_TEST_FAIL:
            return;
        case QCC_TEST_SKIP:
        case QCC_TEST_OVERRUN:
            break;
        }

        if (successes == eng->required_successes) break;
    }

    if (successes == 0)
    {
        qcc_engine_failure(eng);
        qcc_logger_printf(&eng->logger, "%s: ABORTED\n", name);
        qcc_logger_printf(&eng->logger,
                          "    No input was accepted by test as valid\n");
        return;
    }

    ++eng->total_tests;
    if (successes < eng->required_successes)
    {
        qcc_logger_printf(&eng->logger, "%s:\n", name);
        qcc_logger_printf(
            &eng->logger,
            "    Warning: Only %u successful runs out of %u were made\n",
            successes, eng->required_successes);
    }
}
