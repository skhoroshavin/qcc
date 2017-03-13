
#include "interval.h"
#include <assert.h>

void qcc_interval_builder_init(struct qcc_interval_builder *self, void *buffer,
                               size_t size)
{
    self->capacity = size / sizeof(struct qcc_interval);
    self->data = (struct qcc_interval *)buffer;
    self->data += self->capacity;
    self->count = 0;
    self->depth = 0;
}

void qcc_interval_builder_begin(struct qcc_interval_builder *self, size_t pos)
{
    assert(self->depth < QCC_MAX_INTERVAL_DEPTH);
    self->starts[self->depth] = pos;
    ++self->depth;
}

void qcc_interval_builder_end(struct qcc_interval_builder *self, size_t pos)
{
    assert(self->depth > 0);
    assert(self->capacity > self->count);

    --self->depth;
    --self->data;
    ++self->count;

    self->data->begin = self->starts[self->depth];
    self->data->end = pos;
}
