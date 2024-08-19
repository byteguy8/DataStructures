#include "slinkedlist.h"
#include <string.h>

static void _remove_node_(struct slinkedlist_node *before, struct slinkedlist_node *actual, struct slinkedlist *list, void **out_value)
{
    if (before)
        before->next = actual->next;
    else
        list->head = actual->next;

    if (list->tail == actual)
        list->tail = before;

    list->size--;

    if (list->destroy_value)
        list->destroy_value(actual->value);

    // If not helper destroy_value function is present, then
    // check if the user wants to receive the value in the node
    // that will be destroyed. If so, then pass it.
    if (out_value && !list->destroy_value)
        *out_value = actual->value;

    memset(actual, 0, sizeof(struct slinkedlist_node));
    free(actual);
}

struct slinkedlist *slinkedlist_create(void (*destroy_value)(void *ptr))
{
    struct slinkedlist *list = (struct slinkedlist *)malloc(sizeof(struct slinkedlist));

    if (!list)
        return NULL;

    list->size = 0;
    list->head = NULL;
    list->tail = NULL;
    list->destroy_value = destroy_value;

    return list;
}

void slinkedlist_destroy(struct slinkedlist *list)
{
    if (!list)
        return;

    struct slinkedlist_node *node = list->head;

    while (node)
    {
        struct slinkedlist_node *next = node->next;

        if (list->destroy_value)
            list->destroy_value(node->value);

        memset(node, 0, sizeof(struct slinkedlist_node));
        free(node);

        node = next;
    }

    memset(list, 0, sizeof(struct slinkedlist));
    free(list);
}

int slinkedlist_insert(void *value, struct slinkedlist *list, struct slinkedlist_node **out_node)
{
    struct slinkedlist_node *node = (struct slinkedlist_node *)malloc(sizeof(struct slinkedlist_node));

    if (!node)
        return SLINKEDLIST_ERR_ALLOC;

    node->next = NULL;
    node->value = value;

    // If size is zero, head must be NULL. In that case, we make head point to the first element.
    if (!list->head)
        list->head = node;

    // If size is greater than 0 (already exists at least an item in the list) tail will contains
    // the last item in the list. In that case, we make tail point to now, the last element.
    if (list->tail)
        list->tail->next = node;

    list->size++;
    // tail is always modified to point to the recently added (last) item in the list
    list->tail = node;

    if (out_node)
        *out_node = node;

    return SLINKEDLIST_OK;
}

int slinkedlist_insert_at(size_t index, void *value, struct slinkedlist *list, struct slinkedlist_node **out_node)
{
    if (index >= list->size)
        return SLINKEDLIST_ERR_OUTBOUNDS;

    struct slinkedlist_node *before = NULL;
    struct slinkedlist_node *actual = list->head;

    for (size_t i = 0; i < index; i++)
    {
        before = actual;
        actual = actual->next;
    }

    struct slinkedlist_node *node = (struct slinkedlist_node *)malloc(sizeof(struct slinkedlist_node));

    if (!node)
        return SLINKEDLIST_ERR_ALLOC;

    // Updating back links
    if (before)
        before->next = node;
    else
        list->head = node;

    // Updating front links
    node->next = actual;
    // Updating value
    node->value = value;

    list->size++;

    if (out_node)
        *out_node = node;

    return SLINKEDLIST_OK;
}

int slinkedlist_remove_index(size_t index, struct slinkedlist *list, void **out_value)
{
    if (index >= list->size)
        return SLINKEDLIST_ERR_OUTBOUNDS;

    struct slinkedlist_node *before = NULL;
    struct slinkedlist_node *actual = list->head;

    for (size_t i = 0; i < index; i++)
    {
        before = actual;
        actual = actual->next;
    }

    _remove_node_(before, actual, list, out_value);

    return SLINKEDLIST_OK;
}

int slinkedlist_remove_node(struct slinkedlist_node *node, struct slinkedlist *list, void **out_value)
{
    int not_valid = 1;
    struct slinkedlist_node *before = list->head;
    struct slinkedlist_node *actual = list->head;

    while (actual)
    {
        if (actual == node)
        {
            not_valid = 0;
            break;
        }

        before = actual;
        actual = actual->next;
    }

    if (not_valid)
        return SLINKEDLIST_ERR_INVALIDPTR;

    _remove_node_(before, actual, list, out_value);

    return SLINKEDLIST_OK;
}