#ifndef _INTERNAL_NODE_STACK_H
#define _INTERNAL_NODE_STACK_H

#include "hashmap.h"

struct stack_node
{
    struct node* node;
    struct stack_node* next;
    char* key;
    mapval_t val;
};

struct node
{
#ifndef TRASH_KEY
    char* key;
#endif
    mapval_t value;
    unsigned char height;

    struct node *right, *left;
    hash_t _hash_val;
};

struct stack_node* create_stack_node(void);
struct node* pop(struct stack_node**);
void push(struct stack_node**, struct node*);
void free_stack(struct stack_node*);

#endif /* _INTERNAL_NODE_STACK_H */