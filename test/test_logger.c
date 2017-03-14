
#include "test_qcc.h"

TEST(logger)
{
    char buffer[13];
    struct qcc_logger logger;
    qcc_logger_init(&logger, buffer, sizeof(buffer));

    qcc_logger_printf(&logger, "Hello");
    ASSERT_STR_EQ(logger.log_data, "Hello");

    qcc_logger_printf(&logger, ", world!!!");
    ASSERT_STR_EQ(logger.log_data, "Hello, world");

    qcc_logger_printf(&logger, "dumped");
    ASSERT_STR_EQ(logger.log_data, "Hello, world");
}

TEST_MAIN() { RUN_TEST(logger); }
