
#include "logger.h"
#include <stdarg.h>
#include <stdio.h>

void qcc_logger_init(struct qcc_logger *self, char *log_buffer,
                     size_t max_log_size)
{
    self->log_data = log_buffer;
    self->log_size = max_log_size;
    self->log_pos = 0;
    self->log_data[0] = 0;
}

void qcc_logger_printf(struct qcc_logger *self, const char *fmt, ...)
{
    size_t max_size = self->log_size - self->log_pos - 2;

    va_list args;
    va_start(args, fmt);
    size_t size =
        vsnprintf(self->log_data + self->log_pos, max_size, fmt, args);
    va_end(args);

    self->log_pos += size;
    self->log_data[self->log_pos] = 0;
}
