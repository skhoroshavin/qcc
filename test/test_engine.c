
#include "test_qcc.h"

TEST(test_success) { (void)_ctx; }

TEST(test_failure)
{
    GIVEN_UNSIGNED(unsigned, value, any);
    ASSERT_UINT(value, >=, 5);
}

TEST(engine)
{
    char log_data[128];
    struct qcc_logger logger;
    qcc_logger_init(&logger, log_data, sizeof(log_data));

    char buffer[1024];
    struct qcc_engine engine;
    qcc_engine_init(&engine, buffer, sizeof(buffer), &logger);
    ASSERT_UINT(engine.total_tests, ==, 0);
    ASSERT_UINT(engine.failed_tests, ==, 0);

    qcc_engine_run_test(&engine, "test_success", &test_success);
    ASSERT_UINT(engine.total_tests, ==, 1);
    ASSERT_UINT(engine.failed_tests, ==, 0);

    qcc_engine_run_test(&engine, "test_failure", &test_failure);
    ASSERT_UINT(engine.total_tests, ==, 2);
    ASSERT_UINT(engine.failed_tests, ==, 1);
}

TEST_MAIN() { RUN_TEST(engine); }
