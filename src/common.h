
#pragma once

#ifdef __cplusplus
#define QCC_BEGIN_HEADER extern "C" {
#define QCC_END_HEADER }
#else
#define QCC_BEGIN_HEADER
#define QCC_END_HEADER
#endif

QCC_BEGIN_HEADER

#define QCC_PRINTF __attribute__((format(printf, 2, 3)))
#define QCC_VPRINTF __attribute__((format_arg(2)))

QCC_END_HEADER

