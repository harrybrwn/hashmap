#ifndef _INTERNAL_NODE_STACK_H
#define _INTERNAL_NODE_STACK_H

#include "hashmap.h"

struct node_stack
{
    struct node* node;
    struct node_stack* next;
    char* key;
    mapval_t val;
};

#ifndef NODE_DEFINED
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
#endif

struct node_stack* create_node_stack(void);
struct node* pop(struct node_stack**);
void push(struct node_stack**, struct node*);
void free_stack(struct node_stack*);
void push_tree(struct node_stack**, struct node*);

#endif /* _INTERNAL_NODE_STACK_H */