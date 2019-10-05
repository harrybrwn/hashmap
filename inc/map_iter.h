#ifndef _MAP_ITER_H
#define _MAP_ITER_H

#include "hashmap.h"
#include <stddef.h>

struct node_stack;

struct tuple
{
    char* key;
    mapval_t value;
};

typedef struct tuple tuple_t;

typedef struct mapiter
{
    struct node_stack* root;
    size_t pos;
    long int counter;
    Map* _map;
} MapIterator;

MapIterator* map_iter(Map*);

void destroy_iter(MapIterator*);

int iter_done(MapIterator*);

int iter_hasnext(MapIterator*);

tuple_t iter_next(MapIterator*);

char* iter_next_key(MapIterator*);

#endif /* _MAP_ITER_H */