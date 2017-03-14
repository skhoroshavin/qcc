
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

void qcc_stream_read(struct qcc_stream *self, void *data, size_t size)
{
    if (self->pos + size > self->size)
    {
        memset(data, 0, size);
        self->pos = self->size + 1;
        return;
    }

    if (self->mode == QCC_STREAM_WRITE)
        for (size_t i = 0; i != size; ++i)
            self->data[self->pos + i] = rand();
    memcpy(data, self->data + self->pos, size);
    self->pos += size;
}

unsigned qcc_stream_read_value(struct qcc_stream *self)
{
    unsigned result;
    qcc_stream_read(self, &result, sizeof(result));
    return result;
}

int qcc_stream_is_overrun(struct qcc_stream *self)
{
    return self->pos > self->size;
}
