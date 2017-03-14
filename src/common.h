
#pragma once

#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
#define QCC_BEGIN_HEADER extern "C" {
#define QCC_END_HEADER }
#else
#define QCC_BEGIN_HEADER
#define QCC_END_HEADER
#endif

QCC_BEGIN_HEADER

#ifdef WIN32
#define QCC_PRINTF
#define QCC_VPRINTF
#else
#define QCC_PRINTF __attribute__((format(printf, 2, 3)))
#define QCC_VPRINTF __attribute__((format_arg(2)))
#endif /* WIN32 */

#ifndef count_of
#define count_of(a) (sizeof(a) / sizeof(a[0]))
#endif /* count_of */

typedef int64_t qcc_int;
typedef uint64_t qcc_uint;

#define QCC_INT_MIN INT64_MIN
#define QCC_INT_MAX INT64_MAX
#define QCC_UINT_MAX UINT64_MAX

#define qcc_is_unsigned(type) (((type)-1) > 0)
#define qcc_is_float(type) (((type)1.2) != 1)

qcc_int qcc_int_max(size_t size);
qcc_int qcc_int_min(size_t size);
qcc_uint qcc_uint_max(size_t size);

QCC_END_HEADER
