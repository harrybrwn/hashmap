#include <stdio.h>

#define COMPILE_TESTS_H
#include "hashmap.c"
#include "tests/test.h"

static struct node* newnode(hash_t val)
{
    struct node* n = malloc(sizeof(struct node));
    n->_hash_val = val;
    n->height = 0;
    n->left = NULL;
    n->right = NULL;
    return n;
}

// clang-format off
TEST(macros, ({
    struct node* n = NULL;
    assert(height(n) == -1);
    n = newnode(1);
    assert(height(n) == 0);
}))

TEST(rotations, ({
    struct node* n = newnode(10);
    n->left = newnode(9);
    n->left->left = newnode(8);
    n = node_rotateright(n);
    assert(n->_hash_val == 9);

    delete_tree(n);
    n = newnode(10);
    n->right = newnode(11);
    n->right->right = newnode(12);
    n = node_rotateleft(n);
    assert(n->_hash_val == 11);
    delete_tree(n);
}))

TEST(insert_node, ({
    struct node* n = newnode(10);
    insert_node(&n, newnode(5));
    assert(n->_hash_val == 10);
    assert(n->left->_hash_val == 5);
    insert_node(&n, newnode(15));
    assert(n->right->_hash_val == 15);

    insert_node(&n, newnode(3));
    insert_node(&n, newnode(9));
    insert_node(&n, newnode(6));
    assert(n->height == 2);
    assert(n->left->height == 1);

    assert(n->_hash_val == 9);
    assert(n->left->_hash_val == 5);
    assert(n->right->_hash_val == 10);
    assert(n->left->left->_hash_val == 3);
    assert(n->left->right->_hash_val == 6);
    assert(n->right->right->_hash_val == 15);

    insert_node(&n, newnode(16));
    assert(n->height == 2);
    assert(n->right->_hash_val == 15);
    assert(n->right->left->_hash_val == 10);
    assert(n->right->right->_hash_val == 16);
    delete_tree(n);
}))

TEST(search, ({
    struct node* n = newnode(10);
    assert_eq(n, search(n, 10));
    insert_node(&n, newnode(5));
    insert_node(&n, newnode(15));
    insert_node(&n, newnode(25));
    insert_node(&n, newnode(1));

    assert_eq(5, search(n, 5)->_hash_val);
    assert_eq(15, search(n, 15)->_hash_val);
    assert_eq(25, search(n, 25)->_hash_val);
    assert_eq(1, search(n, 1)->_hash_val);
    delete_tree(n);
}))

TEST(prehash, ({
    hash_t a = prehash("abc");
    hash_t b = prehash("cba");
    hash_t c = prehash("bca");

    assert(a != b);
    assert(c != a);
    assert(c != b);

    assert_eq(prehash("abc123"), prehash("abc123"));
    assert(prehash(" ") != prehash("  "));

    // assert(prehash("qwerty") == fnv_1a("qwerty"));
    assert(prehash("qwerty") == fnv_1("qwerty"));
}))

// clang-format off
RUN_TEST_SUITE (
    ADD_TEST(macros),
    ADD_TEST(rotations),
    ADD_TEST(insert_node),
    ADD_TEST(search),
    ADD_TEST(prehash)
)