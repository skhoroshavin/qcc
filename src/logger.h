
#pragma once

#include "common.h"

QCC_BEGIN_HEADER

struct qcc_logger
{
    char *log_data;
    size_t log_size;
    size_t log_pos;
};

void qcc_logger_init(struct qcc_logger *self, char *log_data,
                     size_t max_log_size);
void qcc_logger_printf(struct qcc_logger *self, const char *fmt,
                       ...) QCC_PRINTF;

QCC_END_HEADER
