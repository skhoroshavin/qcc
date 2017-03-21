
#include "logger.h"
#include <stdarg.h>
#include <stdio.h>

static void _append_message(struct qcc_logger *self, const char *data)
{
    QCC_ARENA_POD(self->arena, qcc_log_message, message);
    message->next = 0;
    message->data = data;

    qcc_message_list_push_back(&self->messages, message);
}

void qcc_logger_init(struct qcc_logger *self, struct qcc_arena *arena)
{
    self->arena = arena;
    qcc_message_list_init(&self->messages);
}

void qcc_logger_append(struct qcc_logger *self, const char *fmt,
                       const struct qcc_logger *other)
{
    for (struct qcc_log_message *message = other->messages.first; message;
         message = message->next)
    {
        _append_message(self,
                        qcc_arena_sprintf(self->arena, fmt, message->data));
    }
}

void qcc_logger_printf(struct qcc_logger *self, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    _append_message(self, qcc_arena_vsprintf(self->arena, fmt, args));
    va_end(args);
}

void qcc_logger_vprintf(struct qcc_logger *self, const char *fmt, va_list args)
{
    va_list tmp;
    va_copy(tmp, args);
    _append_message(self, qcc_arena_vsprintf(self->arena, fmt, tmp));
    va_end(tmp);
}

void qcc_logger_dump(struct qcc_logger *self, const char *fmt)
{
    for (struct qcc_log_message *message = self->messages.first; message;
         message = message->next)
    {
        printf(fmt, message->data);
    }
}
