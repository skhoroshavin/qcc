
#include "logger.h"
#include <stdarg.h>
#include <stdio.h>

void qcc_logger_init(struct qcc_logger *self, char *log_buffer,
                     size_t max_log_size)
{
    self->log_data = log_buffer;
    self->log_size = max_log_size;
    qcc_logger_reset(self);
}

void qcc_logger_printf(struct qcc_logger *self, const char *fmt, ...)
{
    char *buffer = self->log_data + self->log_pos;
    size_t max_size = self->log_size - self->log_pos;

    va_list args;
    va_start(args, fmt);
    size_t size = vsnprintf(buffer, max_size, fmt, args);
    va_end(args);

    self->log_pos += size;
    *(buffer + size) = 0;
}

void qcc_logger_reset(struct qcc_logger *self)
{
    self->log_pos = 0;
    self->log_data[0] = 0;
}
