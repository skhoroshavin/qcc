
#pragma once

#include "arena.h"
#include "list.h"

QCC_BEGIN_HEADER

struct qcc_log_message
{
    struct qcc_log_message *next;
    const char *data;
};

QCC_DEFINE_LIST(qcc_message_list, struct qcc_log_message)

struct qcc_logger
{
    struct qcc_arena *arena;
    struct qcc_message_list messages;
};

void qcc_logger_init(struct qcc_logger *self, struct qcc_arena *arena);
void qcc_logger_append(struct qcc_logger *self, const char *fmt,
                       const struct qcc_logger *other);
void qcc_logger_printf(struct qcc_logger *self, const char *fmt,
                       ...) QCC_PRINTF;
void qcc_logger_vprintf(struct qcc_logger *self, const char *fmt, va_list args);
void qcc_logger_dump(struct qcc_logger *self, const char *fmt);

QCC_END_HEADER
