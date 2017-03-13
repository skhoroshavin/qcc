
#include "stream.h"

#include <assert.h>
#include <memory.h>
#include <stdlib.h>

void qcc_stream_init(struct qcc_stream *self, enum qcc_stream_mode mode,
                     void *data, size_t size,
                     struct qcc_interval_builder *intervals)
{
    self->mode = mode;
    self->data = (uint8_t *)data;
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

int qcc_stream_read(struct qcc_stream *self, void *data, size_t size)
{
    if (self->pos + size > self->size) return 0;
    if (self->mode == QCC_STREAM_WRITE)
        for (size_t i = 0; i != size; ++i)
            self->data[self->pos + i] = rand();
    memcpy(data, self->data + self->pos, size);
    self->pos += size;
    return 1;
}

unsigned qcc_stream_read_value(struct qcc_stream *self)
{
    unsigned result;
    qcc_stream_read(self, &result, sizeof(result));
    return result;
}
