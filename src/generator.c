
#include "generator.h"

void qcc_gen_example(struct qcc_generator *self, struct qcc_test_context *ctx,
                     void *data)
{
    self->example(self, ctx, data);
}
