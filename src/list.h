
#pragma once

#include "common.h"

QCC_BEGIN_HEADER

struct qcc_list_node
{
    struct qcc_list_node *next;
};

void qcc_list_node_init(struct qcc_list_node *node);

struct qcc_list
{
    struct qcc_list_node *first;
    struct qcc_list_node *last;
};

void qcc_list_init(struct qcc_list *list);
void qcc_list_push_back(struct qcc_list *list, struct qcc_list_node *node);
void qcc_list_push_front(struct qcc_list *list, struct qcc_list_node *node);
void qcc_list_pop_front(struct qcc_list *list);

#define QCC_DEFINE_LIST(name, node_name)                                       \
    struct name                                                                \
    {                                                                          \
        node_name *first;                                                      \
        node_name *last;                                                       \
    };                                                                         \
    inline static void name##_init(struct name *list)                          \
    {                                                                          \
        qcc_list_init((struct qcc_list *)list);                                \
    }                                                                          \
    inline static void name##_push_back(struct name *list, node_name *node)    \
    {                                                                          \
        qcc_list_push_back((struct qcc_list *)list,                            \
                           (struct qcc_list_node *)node);                      \
    }                                                                          \
    inline static void name##_push_front(struct name *list, node_name *node)   \
    {                                                                          \
        qcc_list_push_front((struct qcc_list *)list,                           \
                            (struct qcc_list_node *)node);                     \
    }                                                                          \
    inline static void name##_pop_front(struct name *list)                     \
    {                                                                          \
        qcc_list_pop_front((struct qcc_list *)list);                           \
    }
