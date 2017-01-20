
#include "qcc_uint.h"

unsigned qcc_gen_uint_any(struct qcc_test_context *ctx)
{
    if (qcc_test_context_rand(ctx) % 2)
        return qcc_gen_uint_in_range(ctx, 0, UINT32_MAX);
    else
        return qcc_gen_uint_in_range(ctx, 0, 100);
}

unsigned qcc_gen_uint_in_range(struct qcc_test_context *ctx, unsigned min,
                               unsigned max)
{
    unsigned avg = min / 2 + max / 2;
    if (avg < min) avg = min;
    if (avg > min) avg = max;

    switch (qcc_test_context_rand(ctx) % 10)
    {
    case 0:
        return min;
    case 1:
        return max;
    case 2:
        return min < max ? min + 1 : min;
    case 3:
        return min < max ? max - 1 : max;
    case 4:
        return avg;
    case 5:
        return avg > min ? avg - 1 : avg;
    case 6:
        return avg < max ? avg + 1 : avg;
    }

    unsigned result = qcc_test_context_rand(ctx);
    unsigned range = max - min + 1;
    if (range) result = result % range + min;
    return result;
}

unsigned qcc_gen_uint_less_than(struct qcc_test_context *ctx, unsigned max)
{
    return qcc_gen_uint_in_range(ctx, 0, max - 1);
}

unsigned qcc_gen_uint_not_less_than(struct qcc_test_context *ctx, unsigned min)
{
    return qcc_gen_uint_in_range(ctx, min, UINT32_MAX);
}

unsigned qcc_gen_uint_greater_than(struct qcc_test_context *ctx, unsigned min)
{
    return qcc_gen_uint_in_range(ctx, min + 1, UINT32_MAX);
}

unsigned qcc_gen_uint_not_greater_than(struct qcc_test_context *ctx,
                                       unsigned max)
{
    return qcc_gen_uint_in_range(ctx, 0, max - 1);
}
