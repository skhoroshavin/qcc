
#include "test_gen.h"

unsigned qcc_gen_unsigned(struct qcc_test_context *ctx, unsigned min,
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
