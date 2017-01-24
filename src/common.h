
#pragma once

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

QCC_END_HEADER

