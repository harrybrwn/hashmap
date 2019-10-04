#include "map_iter.h"
#include "hashmap.h"
#include <stdlib.h>

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

static struct stack_node* create_stack_node(void);
static void push_tree(struct stack_node**, struct node*);
static struct node* pop(struct stack_node**);
static void push(struct stack_node**, struct node*);

MapIterator* map_iter(Map* m)
{
    MapIterator* iter = malloc(sizeof(MapIterator));
    iter->pos = 0;
    iter->root = create_stack_node();
    iter->_map = m;
    iter->counter = m->item_count;
    return iter;
}

int iter_done(MapIterator* it)
{
    return it->counter <= 0 && it->root == NULL;
}

int iter_hasnext(MapIterator* it)
{
    return it->counter > 0 || it->root != NULL;
}

int iter_auto_done(MapIterator** it)
{
    if ((*it)->counter <= 0 && (*it)->root == NULL)
    {
        free(*it);
        *it = NULL;
        return 1;
    }
    return 0;
}

static void free_stack(struct stack_node*);

void destroy_iter(MapIterator* it)
{
    if (it->root) {
        free_stack(it->root);
        it->root = NULL;
    }

    free(it);
}

tuple_t iter_next(MapIterator* it)
{
    while (it->pos < it->_map->__size)
    {
        if (it->_map->__data[it->pos] == NULL)
            it->pos++;
        else
        {
            push_tree(&it->root, it->_map->__data[it->pos++]);
            break;
        }
    }

    it->counter--;
    if (it->root == NULL) {
        return (tuple_t){NULL, (mapval_t)0};
    }

    struct node* n = pop(&it->root);
    return (tuple_t){n->key, n->value};
}

char* iter_next_key(MapIterator* it)
{
    return iter_next(it).key;
}

static void push_tree(struct stack_node** stack, struct node* n)
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

static struct stack_node* create_stack_node(void)
{
    struct stack_node* s = malloc(sizeof(struct stack_node));
    s->next = NULL;
    s->node = NULL;
    return s;
}

static void push(struct stack_node** stack, struct node* n)
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

static struct node* pop(struct stack_node** stack)
{
    struct stack_node* tmp = *stack;
    *stack = (*stack)->next;
    tmp->next = NULL;
    struct node* value = tmp->node;
    free(tmp);
    return value;
}

static void free_stack(struct stack_node* stack)
{
    if (stack->next)
        free_stack(stack->next);
    free(stack);
}