
#include "qcc.h"

TEST_SUITE(arena);
TEST_SUITE(test_context);
TEST_SUITE(test_gen);

TEST_MAIN()
{
    RUN_SUITE(arena);
    RUN_SUITE(test_context);
    RUN_SUITE(test_gen);
}

int main(int argc, const char *argv[]) { return qcc_main(argc, argv); }
