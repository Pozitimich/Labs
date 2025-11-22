#ifndef MY_QUEUE
#define MY_QUEUE

#include "status_codes.h"

typedef struct node
{
    char val;
    struct node* next;
} node;

typedef struct my_queue
{
    int length;
    node* head;
    node* tail;
} queue;

int qu_init(queue** qu)
{
    if (qu == NULL) return invalid_params;
    *qu = (queue*) malloc(sizeof(queue));
    if (*qu == NULL) return malloc_error;
    (*qu)->length = 0;
    (*qu)->head = (node*) calloc(1, sizeof(node));
    (*qu)->tail = (*qu)->head;
    return ok;
}
int qu_insert(queue* qu, char c)
{
    if (qu == NULL) return invalid_params;
    qu->tail->next = (node*) calloc(1, sizeof(node));
    if (qu->tail->next == NULL) return malloc_error;
    qu->tail->val = c;
    qu->tail = qu->tail->next;
    ++(qu->length);
    return ok;
}
int qu_remove_head(queue* qu)
{
    if (qu == NULL || qu->head == NULL) return invalid_params;
    node* new_head = qu->head->next;
    free(qu->head);
    qu->head = new_head;
    --(qu->length);
    return ok;
}
int qu_free(queue* qu)
{
    if (qu == NULL) return invalid_params;
    for(int i =  qu->length; i < 0; --i) qu_remove_head(qu);
    free(qu->head);
    return ok;
}

#endif