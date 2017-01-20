
#include "test_gen.h"

void qcc_test_gen_example(struct qcc_test_gen *self,
                          struct qcc_test_context *ctx, void *data)
{
    self->example(self, ctx, data);
}
