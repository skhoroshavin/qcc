
#pragma once

#include "interval.h"

QCC_BEGIN_HEADER

enum qcc_stream_mode
{
    QCC_STREAM_READ,
    QCC_STREAM_WRITE
};

struct qcc_stream
{
    enum qcc_stream_mode mode;
    qcc_uint *data;
    size_t size;

    size_t pos;
    struct qcc_interval_builder *intervals;
};

void qcc_stream_init(struct qcc_stream *self, enum qcc_stream_mode mode,
                     qcc_uint *data, size_t size,
                     struct qcc_interval_builder *intervals);

void qcc_stream_begin(struct qcc_stream *self);
void qcc_stream_end(struct qcc_stream *self);
qcc_uint qcc_stream_get(struct qcc_stream *self);
int qcc_stream_is_overrun(struct qcc_stream *self);

QCC_END_HEADER
