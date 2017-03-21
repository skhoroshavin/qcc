
#include "stream.h"
#include "arena.h"

#include <assert.h>
#include <memory.h>
#include <stdlib.h>

struct qcc_stream_data
qcc_stream_data_process(struct qcc_stream_data data, struct qcc_arena *arena,
                        const struct qcc_stream_node *begin,
                        const struct qcc_stream_node *end,
                        qcc_stream_data_process_fn proc)
{
    struct qcc_stream_data result;
    qcc_stream_data_init(&result);

    unsigned in_range = 0;
    unsigned is_changed = 0;
    for (struct qcc_stream_node *node = data.first; node; node = node->next)
    {
        if (node == begin) in_range = 1;
        if (node == end) in_range = 0;

        qcc_uint value = node->value;
        if (in_range)
        {
            unsigned skip = proc(&value);
            is_changed = skip || (value != node->value);
            if (skip) continue;
        }

        QCC_ARENA_POD(arena, qcc_stream_node, copy);
        copy->value = value;
        qcc_stream_data_push_back(&result, copy);
    }

    return is_changed ? result : data;
}

static void _generate_data(struct qcc_stream *self)
{
    QCC_ARENA_POD(self->arena, qcc_stream_node, node);
    node->value = (qcc_uint)rand() + (((qcc_uint)rand()) << 32);
    qcc_stream_data_push_back(&self->data, node);
}

void qcc_stream_init(struct qcc_stream *self,
                     const struct qcc_stream_data *data,
                     struct qcc_arena *arena)
{
    self->arena = arena;
    if (data)
    {
        self->data = *data;
        self->is_writing = 0;
    }
    else
    {
        qcc_stream_data_init(&self->data);
        _generate_data(self);
        self->is_writing = 1;
    }
    self->node = self->data.first;
    qcc_interval_data_init(&self->interval_data);
    qcc_interval_data_init(&self->interval_stack);
    self->is_overrun = 0;
}

void qcc_stream_begin(struct qcc_stream *self)
{
    QCC_ARENA_POD(self->arena, qcc_stream_interval, interval);
    interval->begin = self->node;
    qcc_interval_data_push_front(&self->interval_stack, interval);
}

void qcc_stream_end(struct qcc_stream *self)
{
    struct qcc_stream_interval *interval = self->interval_stack.first;
    qcc_interval_data_pop_front(&self->interval_stack);
    interval->end = self->node;
    if (interval->begin != interval->end)
        qcc_interval_data_push_back(&self->interval_data, interval);
}

qcc_uint qcc_stream_get(struct qcc_stream *self)
{
    if (self->is_writing)
        _generate_data(self);
    else
    {
        if (!self->node) self->is_overrun = 1;
        if (self->is_overrun) return 0;
    }

    qcc_stream_begin(self);
    qcc_uint result = self->node->value;
    self->node = self->node->next;
    qcc_stream_end(self);
    return result;
}
