
#include "test_qcc.h"

TEST(test_success) { (void)_ctx; }

TEST(test_failure)
{
    GIVEN_UNSIGNED(unsigned, value, any);
    ASSERT_UNSIGNED(value, >=, 5);
}

TEST(engine)
{
    GIVEN_ARENA(arena);

    struct qcc_engine engine;
    qcc_engine_init(&engine, arena);
    ASSERT_UNSIGNED(engine.total_tests, ==, 0);
    ASSERT_UNSIGNED(engine.failed_tests, ==, 0);

    qcc_engine_run_test(&engine, "test_success", &test_success);
    ASSERT_UNSIGNED(engine.total_tests, ==, 1);
    ASSERT_UNSIGNED(engine.failed_tests, ==, 0);

    qcc_engine_run_test(&engine, "test_failure", &test_failure);
    ASSERT_UNSIGNED(engine.total_tests, ==, 2);
    ASSERT_UNSIGNED(engine.failed_tests, ==, 1);
}

TEST_MAIN() { RUN_TEST(engine); }
