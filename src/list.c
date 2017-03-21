
#include "list.h"

void qcc_list_node_init(struct qcc_list_node *node) { node->next = 0; }

void qcc_list_init(struct qcc_list *list)
{
    list->first = 0;
    list->last = 0;
}

void qcc_list_push_back(struct qcc_list *list, struct qcc_list_node *node)
{
    if (list->last)
        list->last->next = node;
    else
        list->first = node;

    node->next = 0;
    list->last = node;
}

void qcc_list_push_front(struct qcc_list *list, struct qcc_list_node *node)
{
    if (!list->first) list->last = node;

    node->next = list->first;
    list->first = node;
}

void qcc_list_pop_front(struct qcc_list *list)
{
    list->first = list->first->next;
    if (!list->first) list->last = 0;
}
