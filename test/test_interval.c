
#include "test_qcc.h"

TEST(interval_builder)
{
    struct qcc_interval buffer[1024];
    struct qcc_interval_builder builder;

    qcc_interval_builder_init(&builder, buffer, countof(buffer));
    ASSERT_UINT(builder.pos, ==, 0);

    qcc_interval_builder_begin(&builder, 0);
    ASSERT_UINT(builder.pos, ==, 0);

    qcc_interval_builder_end(&builder, 1);
    ASSERT_UINT(builder.pos, ==, 1);
    ASSERT_UINT(buffer[0].begin, ==, 0);
    ASSERT_UINT(buffer[0].end, ==, 1);

    qcc_interval_builder_begin(&builder, 5);
    qcc_interval_builder_end(&builder, 5);
    ASSERT_UINT(builder.pos, ==, 1);
    ASSERT_UINT(buffer[0].begin, ==, 0);
    ASSERT_UINT(buffer[0].end, ==, 1);

    qcc_interval_builder_begin(&builder, 1);
    qcc_interval_builder_begin(&builder, 3);
    qcc_interval_builder_end(&builder, 4);
    qcc_interval_builder_end(&builder, 10);
    ASSERT_UINT(builder.pos, ==, 3);
    ASSERT_UINT(buffer[0].begin, ==, 0);
    ASSERT_UINT(buffer[0].end, ==, 1);
    ASSERT_UINT(buffer[1].begin, ==, 3);
    ASSERT_UINT(buffer[1].end, ==, 4);
    ASSERT_UINT(buffer[2].begin, ==, 1);
    ASSERT_UINT(buffer[2].end, ==, 10);
}

TEST_MAIN() { RUN_TEST(interval_builder); }
