#include "internal/node_stack.h"
//
#include "hashmap.h"
#include "map_iter.h"
#include <stdlib.h>

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

void destroy_iter(MapIterator* it)
{
    if (it->root)
    {
        free_stack(it->root);
        it->root = NULL;
    }

    free(it);
}

// static void push_tree(struct stack_node**, struct node*);

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
    if (it->root == NULL)
    {
        return (tuple_t){ NULL, (mapval_t)0 };
    }

    struct node* n = pop(&it->root);
    return (tuple_t){ n->key, n->value };
}

char* iter_next_key(MapIterator* it)
{
    return iter_next(it).key;
}