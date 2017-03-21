
#include "test_qcc.h"

TEST(logger)
{
    GIVEN_ARENA(arena);

    struct qcc_logger logger;
    qcc_logger_init(&logger, arena);
    ASSERT_UNSIGNED(logger.messages.first, ==, 0);
    ASSERT_UNSIGNED(logger.messages.last, ==, 0);

    qcc_logger_printf(&logger, "Hello");
    ASSERT_UNSIGNED(logger.messages.first, ==, logger.messages.last);
    ASSERT_UNSIGNED(logger.messages.first->next, ==, 0);
    ASSERT_STR_EQ(logger.messages.first->data, "Hello");

    qcc_logger_printf(&logger, ", world!!!");
    ASSERT_UNSIGNED(logger.messages.first->next, ==, logger.messages.last);
    ASSERT_UNSIGNED(logger.messages.last->next, ==, 0);
    ASSERT_STR_EQ(logger.messages.first->data, "Hello");
    ASSERT_STR_EQ(logger.messages.last->data, ", world!!!");

    qcc_logger_printf(&logger, "not dumped");
    ASSERT_UNSIGNED(logger.messages.first->next->next, ==,
                    logger.messages.last);
    ASSERT_UNSIGNED(logger.messages.last->next, ==, 0);
    ASSERT_STR_EQ(logger.messages.first->data, "Hello");
    ASSERT_STR_EQ(logger.messages.first->next->data, ", world!!!");
    ASSERT_STR_EQ(logger.messages.last->data, "not dumped");
}

TEST_MAIN() { RUN_TEST(logger); }
