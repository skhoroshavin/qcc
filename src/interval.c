
#include "interval.h"
#include <assert.h>
#include <memory.h>

void qcc_interval_builder_init(struct qcc_interval_builder *self,
                               struct qcc_interval *data, size_t size)
{
    self->data = data;
    self->size = size;
    self->pos = 0;
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
    assert(self->size > self->pos);

    --self->depth;
    if (pos == self->starts[self->depth]) return;

    struct qcc_interval *interval = self->data + self->pos;
    interval->begin = self->starts[self->depth];
    interval->end = pos;
    if (self->pos && (memcmp(interval - 1, interval, sizeof(*interval)) == 0))
        return;
    ++self->pos;
}
