
#pragma once

#include "arena.h"
#include "list.h"

QCC_BEGIN_HEADER

struct qcc_stream_node
{
    struct qcc_stream_node *next;
    qcc_uint value;
};
QCC_DEFINE_LIST(qcc_stream_data, struct qcc_stream_node)

typedef unsigned (*qcc_stream_data_process_fn)(qcc_uint *value);

struct qcc_stream_data
qcc_stream_data_process(struct qcc_stream_data data, struct qcc_arena *arena,
                        const struct qcc_stream_node *begin,
                        const struct qcc_stream_node *end,
                        qcc_stream_data_process_fn proc);

struct qcc_stream_interval
{
    struct qcc_stream_interval *next;

    struct qcc_stream_node *begin;
    struct qcc_stream_node *end;
};
QCC_DEFINE_LIST(qcc_interval_data, struct qcc_stream_interval)

struct qcc_stream
{
    struct qcc_arena *arena;
    struct qcc_stream_data data;
    struct qcc_stream_node *node;
    struct qcc_interval_data interval_data;
    struct qcc_interval_data interval_stack;
    unsigned is_writing;
    unsigned is_overrun;
};

void qcc_stream_init(struct qcc_stream *self,
                     const struct qcc_stream_data *data,
                     struct qcc_arena *arena);
void qcc_stream_begin(struct qcc_stream *self);
void qcc_stream_end(struct qcc_stream *self);
qcc_uint qcc_stream_get(struct qcc_stream *self);
