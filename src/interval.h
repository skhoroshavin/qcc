
#include "common.h"

QCC_BEGIN_HEADER

#define QCC_MAX_INTERVAL_DEPTH 16

struct qcc_interval
{
    size_t begin;
    size_t end;
};

struct qcc_interval_builder
{
    struct qcc_interval *data;
    size_t size;
    size_t pos;

    size_t starts[QCC_MAX_INTERVAL_DEPTH];
    size_t depth;
};

void qcc_interval_builder_init(struct qcc_interval_builder *self,
                               struct qcc_interval *data, size_t size);
void qcc_interval_builder_begin(struct qcc_interval_builder *self, size_t pos);
void qcc_interval_builder_end(struct qcc_interval_builder *self, size_t pos);

QCC_END_HEADER
