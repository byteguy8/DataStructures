#ifndef _SLINKEDLIST_H_
#define _SLINKEDLIST_H_

#define SLINKEDLIST_OK 0
#define SLINKEDLIST_ERR_ALLOC 1
#define SLINKEDLIST_ERR_OUTBOUNDS 2
#define SLINKEDLIST_ERR_INVALIDPTR 3

#include <stdlib.h>

typedef struct slinkedlist_node
{
    void *value;
    struct slinkedlist_node *next;
} SLinkedListNode;

typedef struct slinkedlist
{
    size_t size;
    struct slinkedlist_node *head;
    struct slinkedlist_node *tail;
    void (*destroy_value)(void *ptr);
} SLinkedList;

struct slinkedlist *slinkedlist_create(void (*destroy_value)(void *ptr));

void slinkedlist_destroy(struct slinkedlist *list);

int slinkedlist_insert(void *value, struct slinkedlist *list, struct slinkedlist_node **out_node);

int slinkedlist_insert_at(size_t index, void *value, struct slinkedlist *list, struct slinkedlist_node **out_node);

int slinkedlist_remove_index(size_t index, struct slinkedlist *list, void **out_value);

int slinkedlist_remove_node(struct slinkedlist_node *node, struct slinkedlist *list, void **out_value);

#endif