#include "internal/node_stack.h"
#include <stdlib.h>

struct node_stack* create_node_stack(void)
{
    struct node_stack* s = malloc(sizeof(struct node_stack));
    s->next = NULL;
    s->node = NULL;
    return s;
}

void push(struct node_stack** stack, struct node* n)
{
    if ((*stack) != NULL && (*stack)->node == NULL)
    {
        (*stack)->node = n;
        (*stack)->next = NULL;
        return;
    }

    struct node_stack* new = malloc(sizeof(struct node_stack));
    new->next = *stack;
    new->node = n;
    *stack = new;
}

struct node* pop(struct node_stack** stack)
{
    struct node_stack* tmp = *stack;
    *stack = (*stack)->next;
    tmp->next = NULL;
    struct node* value = tmp->node;
    free(tmp);
    return value;
}

void free_stack(struct node_stack* stack)
{
    if (stack->next)
        free_stack(stack->next);
    free(stack);
}

void push_tree(struct node_stack** stack, struct node* n)
{
    if (n->left != NULL)
    {
        push_tree(stack, n->left);
    }
    if (n->right != NULL)
    {
        push_tree(stack, n->right);
    }
    push(stack, n);
}