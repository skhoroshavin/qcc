
#include "test_qcc.h"

TEST(list)
{
    struct qcc_list list;
    struct qcc_list_node a, b, c;

    qcc_list_init(&list);
    ASSERT(!list.first);
    ASSERT(!list.last);

    qcc_list_push_back(&list, &b);
    ASSERT(list.first == &b);
    ASSERT(list.last == &b);
    ASSERT(!b.next);

    qcc_list_push_front(&list, &a);
    ASSERT(list.first == &a);
    ASSERT(list.last == &b);
    ASSERT(a.next == &b);
    ASSERT(!b.next);

    qcc_list_push_back(&list, &c);
    ASSERT(list.first == &a);
    ASSERT(list.last == &c);
    ASSERT(a.next == &b);
    ASSERT(b.next == &c);
    ASSERT(!c.next);

    qcc_list_pop_front(&list);
    ASSERT(list.first == &b);
    ASSERT(list.last == &c);
    ASSERT(b.next == &c);
    ASSERT(!c.next);

    qcc_list_pop_front(&list);
    ASSERT(list.first == &c);
    ASSERT(list.last == &c);
    ASSERT(!c.next);

    qcc_list_pop_front(&list);
    ASSERT(!list.first);
    ASSERT(!list.last);
}

TEST_MAIN() { RUN_TEST(list); }
