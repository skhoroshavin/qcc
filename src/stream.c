
#include "stream.h"

#include <assert.h>
#include <memory.h>
#include <stdlib.h>

void qcc_stream_init(struct qcc_stream *self, enum qcc_stream_mode mode,
                     qcc_uint *data, size_t size,
                     struct qcc_interval_builder *intervals)
{
    self->mode = mode;
    self->data = data;
    self->size = size;
    self->pos = 0;
    self->intervals = intervals;
}

void qcc_stream_begin(struct qcc_stream *self)
{
    assert(self->intervals);
    qcc_interval_builder_begin(self->intervals, self->pos);
}

void qcc_stream_end(struct qcc_stream *self)
{
    assert(self->intervals);
    qcc_interval_builder_end(self->intervals, self->pos);
}

qcc_uint qcc_stream_get(struct qcc_stream *self)
{
    if (self->pos == self->size)
    {
        ++self->pos;
        return 0;
    }

    qcc_stream_begin(self);
    if (self->mode == QCC_STREAM_WRITE)
        self->data[self->pos] = (qcc_uint)rand() + (((qcc_uint)rand()) << 32);
    qcc_uint result = self->data[self->pos];
    ++self->pos;
    qcc_stream_end(self);

    return result;
}

int qcc_stream_is_overrun(struct qcc_stream *self)
{
    return self->pos > self->size;
}
