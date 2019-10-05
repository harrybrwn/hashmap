#ifndef _INTERNAL_NODE_STACK_H
#define _INTERNAL_NODE_STACK_H

#include "hashmap.h"
#include "internal/node.h"

struct node_stack
{
    struct node* node;
    struct node_stack* next;
    char* key;
    mapval_t val;
};

struct node_stack* create_node_stack(void);
struct node* pop(struct node_stack**);
void push(struct node_stack**, struct node*);
void free_stack(struct node_stack*);
void push_tree(struct node_stack**, struct node*);

#endif /* _INTERNAL_NODE_STACK_H */