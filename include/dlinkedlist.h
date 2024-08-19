#ifndef _DLINKEDLIST_H_
#define _DLINKEDLIST_H_

#define DLINKEDLIST_OK 0
#define DLINKEDLIST_ERR_ALLOC 1
#define DLINKEDLIST_ERR_OUTBOUNDS 2

#include <stdlib.h>

typedef struct dlinkedlist_node
{
    void *value;
    struct dlinkedlist_node *previous;
    struct dlinkedlist_node *next;
    struct dlinkedlist *list;
} DLinkedListNode;

typedef struct dlinkedlist
{
    size_t size;
    struct dlinkedlist_node *head;
    struct dlinkedlist_node *tail;
    void (*destroy_value)(void *value);
} DLinkedList;

struct dlinkedlist *dlinkedlist_create(void (*destroy_value)(void *value));

void dlinkedlist_destroy(struct dlinkedlist *list);

int dlinkedlist_insert(void *value, struct dlinkedlist *list, struct dlinkedlist_node **out_node);

int dlinkedlist_insert_index(size_t index, void *value, struct dlinkedlist *list, struct dlinkedlist_node **out_node);

int dlinkedlist_insert_node(struct dlinkedlist_node *node, void *value, struct dlinkedlist_node **out_node);

int dlinkedlist_remove_index(size_t index, struct dlinkedlist *list, void **out_value);

void dlinkedlist_remove_node(struct dlinkedlist_node *node, void **out_value);

#endif