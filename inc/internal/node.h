#ifndef _INTERNAL_NODE_H
#define _INTERNAL_NODE_H

#include "hashmap.h"

struct node
{
#ifndef TRASH_KEY
    char* key;
#endif
    mapval_t value;

    /**
     * Using unsigned char to save space
     *
     * The height of a tree is log2(n), and the max height of
     * this implementation is 255, which means that the maximum number
     * of nodes in each tree is 2^255 or about 5.79e76, more than
     * enough wiggle room seeing as though that is almost as many
     * atoms in the observable universe, I think one byte will be enough.
     */

    unsigned char height;

    struct node *right, *left;
    hash_t _hash_val;
};

#endif /* _INTERNAL_NODE_H */