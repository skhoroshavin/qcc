
#include "common.h"

qcc_int qcc_int_max(size_t size) { return qcc_uint_max(size) >> 1; }
qcc_int qcc_int_min(size_t size) { return -qcc_int_max(size) - 1; }

qcc_uint qcc_uint_max(size_t size)
{
    qcc_uint res = 0xff;
    if (size > 1) res = res | (res << 8);
    if (size > 2) res = res | (res << 16);
    if (size > 4) res = res | (res << 32);
    return res;
}
