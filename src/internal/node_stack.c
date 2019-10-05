#include "internal/node_stack.h"
#include <stdlib.h>

struct stack_node* create_stack_node(void)
{
    struct stack_node* s = malloc(sizeof(struct stack_node));
    s->next = NULL;
    s->node = NULL;
    return s;
}

void push(struct stack_node** stack, struct node* n)
{
    if ((*stack) != NULL && (*stack)->node == NULL)
    {
        (*stack)->node = n;
        (*stack)->next = NULL;
        return;
    }

    struct stack_node* new = malloc(sizeof(struct stack_node));
    new->next = *stack;
    new->node = n;
    *stack = new;
}

struct node* pop(struct stack_node** stack)
{
    struct stack_node* tmp = *stack;
    *stack = (*stack)->next;
    tmp->next = NULL;
    struct node* value = tmp->node;
    free(tmp);
    return value;
}

void free_stack(struct stack_node* stack)
{
    if (stack->next)
        free_stack(stack->next);
    free(stack);
}