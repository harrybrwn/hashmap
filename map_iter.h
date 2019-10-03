#ifndef _MAP_ITER_H
#define _MAP_ITER_H

#include "hashmap.h"
#include <stddef.h>

struct stack_node;

struct node
{
    char* key;
    mapval_t val;
    unsigned char height;
    struct node *right, *left;
    hash_t hash;
};

typedef struct mapiter
{
    struct stack_node* root;
    size_t pos;
    long int counter;
    Map* _map;
} MapIterator;

MapIterator* map_iter(Map*);
int iter_done(MapIterator*);
struct node* iter_next(MapIterator*);
char* iter_next_key(MapIterator*);

#endif /* _MAP_ITER_H */