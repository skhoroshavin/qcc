
#include "qcc.h"

TEST_SUITE(arena);
TEST_SUITE(generator);
TEST_SUITE(qcc_uint);
TEST_SUITE(test_context);

TEST_MAIN()
{
    RUN_SUITE(arena);
    RUN_SUITE(generator);
    RUN_SUITE(qcc_uint);
    RUN_SUITE(test_context);
}

int main(int argc, const char *argv[]) { return qcc_main(argc, argv); }
