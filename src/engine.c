
#include "engine.h"
#include "context.h"
#include "logger.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void qcc_engine_init(struct qcc_engine *eng, void *buffer, size_t buf_size,
                     struct qcc_logger *logger)
{
    eng->max_tries = 1000;
    eng->required_successes = 100;
    eng->total_tests = 0;
    eng->failed_tests = 0;
    eng->buf_data = buffer;
    eng->buf_size = buf_size;
    eng->logger = logger;
}

void qcc_engine_failure(struct qcc_engine *eng)
{
    ++eng->failed_tests;
    ++eng->total_tests;
}

static enum qcc_test_result
_run_test_once(struct qcc_engine *eng, const char *name, qcc_test_fn test_fn,
               enum qcc_stream_mode mode, qcc_uint *test_data,
               size_t *test_size, struct qcc_interval *interval_data,
               size_t *interval_size, char *log_data, size_t *log_size)
{
    struct qcc_arena arena;
    qcc_arena_init(&arena, eng->buf_data, eng->buf_size);

    struct qcc_interval_builder intervals;
    qcc_interval_builder_init(&intervals, interval_data, *interval_size);

    struct qcc_stream stream;
    qcc_stream_init(&stream, mode, test_data, *test_size, &intervals);

    struct qcc_logger logger;
    qcc_logger_init(&logger, log_data, *log_size);

    struct qcc_context ctx;
    qcc_context_init(&ctx, &stream, &arena);
    test_fn(&ctx);
    enum qcc_test_result result = ctx.result;
    if (qcc_stream_is_overrun(&stream)) result = QCC_TEST_OVERRUN;
    if (result == QCC_TEST_FAIL)
    {
        qcc_logger_printf(&logger, "%s: FAILED\n", name);
        for (struct qcc_test_param *param = ctx.param; param;
             param = param->next)
            qcc_logger_printf(&logger, "    %s\n", param->value);
        qcc_logger_printf(&logger, "    %s\n", ctx.error);
    }

    *interval_size = intervals.pos;
    *test_size = stream.pos;
    *log_size = logger.log_pos;

    qcc_arena_done(&arena);
    return result;
}

static size_t _stream_skip_interval(qcc_uint *data, const qcc_uint *old_data,
                                    size_t old_size, size_t begin, size_t end)
{
    if (end - begin == old_size) return -1;

    memcpy(data, old_data, begin * sizeof(qcc_uint));
    memcpy(data + begin, old_data + end, (old_size - end) * sizeof(qcc_uint));
    return old_size - end + begin;
}

static int _is_zero(const qcc_uint *data, size_t begin, size_t end)
{
    for (size_t i = begin; i != end; ++i)
        if (data[i]) return 0;
    return 1;
}

static size_t _stream_zero_interval(qcc_uint *data, const qcc_uint *old_data,
                                    size_t old_size, size_t begin, size_t end)
{
    if (_is_zero(old_data, begin, end)) return -1;

    memcpy(data, old_data, old_size * sizeof(qcc_uint));
    memset(data + begin, 0, (end - begin) * sizeof(qcc_uint));
    return old_size;
}

static int _is_sorted(const qcc_uint *data, size_t begin, size_t end)
{
    for (size_t i = begin; i != end - 1; ++i)
        if (data[i + 1] < data[i]) return 0;
    return 1;
}

static int _stream_sorter(const void *a, const void *b)
{
    if (*(uint64_t *)a > *(uint64_t *)b) return 1;
    if (*(uint64_t *)a < *(uint64_t *)b) return -1;
    return 0;
}

static size_t _stream_sort_interval(qcc_uint *data, const qcc_uint *old_data,
                                    size_t old_size, size_t begin, size_t end)
{
    if (_is_sorted(old_data, begin, end)) return -1;

    memcpy(data, old_data, old_size * sizeof(qcc_uint));
    qsort(data + begin, end - begin, sizeof(qcc_uint), _stream_sorter);
    return old_size;
}

static size_t _stream_halve_interval(qcc_uint *data, const qcc_uint *old_data,
                                     size_t old_size, size_t begin, size_t end)
{
    if (_is_zero(old_data, begin, end)) return -1;

    memcpy(data, old_data, old_size * sizeof(qcc_uint));
    for (size_t i = begin; i != end; ++i)
        data[i] = old_data[i] >> 1;
    return old_size;
}

static size_t _stream_dec_interval(qcc_uint *data, const qcc_uint *old_data,
                                   size_t old_size, size_t begin, size_t end)
{
    if (_is_zero(old_data, begin, end)) return -1;

    memcpy(data, old_data, old_size * sizeof(qcc_uint));
    for (size_t i = begin; i != end; ++i)
        data[i] = old_data[i] - 1;
    return old_size;
}

static enum qcc_test_result _run_test_and_shrink(struct qcc_engine *eng,
                                                 const char *name,
                                                 qcc_test_fn test_fn)
{
    typedef size_t (*try_shrink_fn)(qcc_uint *, const qcc_uint *, size_t,
                                    size_t, size_t);
    static const try_shrink_fn try_shrink[] = {
        _stream_skip_interval, _stream_zero_interval, _stream_sort_interval,
        _stream_halve_interval, _stream_dec_interval};

    qcc_uint dbuf[1024];
    struct qcc_interval ibuf[1024];
    char lbuf[65536];

    size_t dbuf_size = countof(dbuf);
    size_t ibuf_size = countof(ibuf);
    size_t lbuf_size = countof(lbuf);

    enum qcc_test_result result =
        _run_test_once(eng, name, test_fn, QCC_STREAM_WRITE, dbuf, &dbuf_size,
                       ibuf, &ibuf_size, lbuf, &lbuf_size);
    if (result != QCC_TEST_FAIL) return result;

    qcc_engine_failure(eng);

    while (1)
    {
        int found_interesting = 0;

        for (size_t num = 0; num != countof(try_shrink); ++num)
        {
            for (size_t i = 0; i != ibuf_size; ++i)
            {
                size_t begin = ibuf[i].begin;
                size_t end = ibuf[i].end;
                assert(begin < end);

                qcc_uint tmp_dbuf[1024];
                struct qcc_interval tmp_ibuf[1024];
                char tmp_lbuf[65536];

                size_t tmp_dbuf_size =
                    try_shrink[num](tmp_dbuf, dbuf, dbuf_size, begin, end);
                if (tmp_dbuf_size > dbuf_size) continue;
                size_t tmp_ibuf_size = countof(tmp_ibuf);
                size_t tmp_lbuf_size = countof(tmp_lbuf);

                if (_run_test_once(eng, name, test_fn, QCC_STREAM_READ,
                                   tmp_dbuf, &tmp_dbuf_size, tmp_ibuf,
                                   &tmp_ibuf_size, tmp_lbuf,
                                   &tmp_lbuf_size) == QCC_TEST_FAIL)
                {
                    found_interesting = 1;

                    memcpy(ibuf, tmp_ibuf, ibuf_size * sizeof(ibuf[0]));
                    memcpy(dbuf, tmp_dbuf, dbuf_size * sizeof(dbuf[0]));
                    strncpy(lbuf, tmp_lbuf, sizeof(lbuf));
                    break;
                }
            }
            if (found_interesting) break;
        }
        if (found_interesting)
        {
            /*
            for (size_t i = 0; i != stream[step].pos; ++i)
                printf("%016" PRIx64 " ", stream[step].data[i]);
            printf("\n\n");
            */
            continue;
        }

        qcc_logger_printf(eng->logger, "%s", lbuf);
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
        qcc_logger_printf(eng->logger, "%s: ABORTED\n", name);
        qcc_logger_printf(eng->logger,
                          "    No input was accepted by test as valid\n");
        return;
    }

    ++eng->total_tests;
    if (successes < eng->required_successes)
    {
        qcc_logger_printf(eng->logger, "%s:\n", name);
        qcc_logger_printf(
            eng->logger,
            "    Warning: Only %u successful runs out of %u were made\n",
            successes, eng->required_successes);
    }
}
