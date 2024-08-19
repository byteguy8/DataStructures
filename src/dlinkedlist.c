#include "dlinkedlist.h"
#include <string.h>

struct dlinkedlist *dlinkedlist_create(void (*destroy_value)(void *value))
{
    struct dlinkedlist *list = (struct dlinkedlist *)malloc(sizeof(struct dlinkedlist));

    if (!list)
        return NULL;

    list->size = 0;
    list->head = NULL;
    list->tail = NULL;
    list->destroy_value = destroy_value;

    return list;
}

void dlinkedlist_destroy(struct dlinkedlist *list)
{
    if (!list)
        return;

    struct dlinkedlist_node *node = list->head;

    while (node)
    {
        struct dlinkedlist_node *next = node->next;

        if (list->destroy_value)
            list->destroy_value(node->value);

        memset(node, 0, sizeof(struct dlinkedlist_node));
        free(node);

        node = next;
    }

    memset(list, 0, sizeof(struct dlinkedlist));
    free(list);
}

int dlinkedlist_insert(void *value, struct dlinkedlist *list, struct dlinkedlist_node **out_node)
{
    struct dlinkedlist_node *node = (struct dlinkedlist_node *)malloc(sizeof(struct dlinkedlist_node));

    if (!node)
        return DLINKEDLIST_ERR_ALLOC;

    node->value = value;
    node->previous = NULL;
    node->next = NULL;
    node->list = list;

    if (!list->head)
        list->head = node;

    if (list->tail)
    {
        list->tail->next = node;
        node->previous = list->tail;
    }

    list->size++;
    list->tail = node;

    if (out_node)
        *out_node = node;

    return DLINKEDLIST_OK;
}

int dlinkedlist_insert_index(size_t index, void *value, struct dlinkedlist *list, struct dlinkedlist_node **out_node)
{
    if (index >= list->size)
        return DLINKEDLIST_ERR_OUTBOUNDS;

    struct dlinkedlist_node *node = list->head;

    for (size_t i = 0; i < index; i++)
        node = node->next;

    return dlinkedlist_insert_node(node, value, out_node);
}

int dlinkedlist_insert_node(struct dlinkedlist_node *node, void *value, struct dlinkedlist_node **out_node)
{
    struct dlinkedlist_node *new_node = (struct dlinkedlist_node *)malloc(sizeof(struct dlinkedlist_node));

    if (!new_node)
        return DLINKEDLIST_ERR_ALLOC;

    if (node->previous)
        node->previous->next = new_node;

    node->previous = new_node;

    struct dlinkedlist *list = node->list;

    new_node->value = value;
    new_node->previous = node->previous;
    new_node->next = node;
    new_node->list = list;

    list->size++;

    if (node == list->head)
        list->head = new_node;

    if (node == list->tail)
        list->tail = new_node;

    if (out_node)
        *out_node = node;

    return DLINKEDLIST_OK;
}

int dlinkedlist_remove_index(size_t index, struct dlinkedlist *list, void **out_value)
{
    if (index >= list->size)
        return DLINKEDLIST_ERR_OUTBOUNDS;

    struct dlinkedlist_node *node = list->head;

    for (size_t i = 0; i < index; i++)
        node = node->next;

    dlinkedlist_remove_node(node, out_value);

    return DLINKEDLIST_OK;
}

void dlinkedlist_remove_node(struct dlinkedlist_node *node, void **out_value)
{
    struct dlinkedlist *list = node->list;

    if (node->previous)
        node->previous->next = node->next;

    if (node->next)
        node->next->previous = node->previous;

    if (node == list->head)
        list->head = node->next;

    if (node == list->tail)
        list->tail = node->previous;

    list->size--;

    if (out_value)
        *out_value = node->value;
    else if (list->destroy_value)
        list->destroy_value(node->value);

    memset(node, 0, sizeof(struct dlinkedlist_node));
    free(node);
}