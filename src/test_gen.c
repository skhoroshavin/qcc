
#include "test_gen.h"

unsigned qcc_gen_unsigned(struct qcc_test_context *ctx, unsigned min,
                          unsigned max)
{
    unsigned avg = (min + max) / 2;

    switch (qcc_test_context_rand(ctx) % 16)
    {
    case 0:
        return min;
    case 1:
        return max;
    case 2:
        return min + 1;
    case 3:
        return max - 1;
    case 4:
        return avg;
    case 5:
        return avg + 1;
    case 6:
        return avg - 1;
    }

    unsigned result = qcc_test_context_rand(ctx);
    unsigned range = max - min + 1;
    if (range) result = result % range + min;
    return result;
}
