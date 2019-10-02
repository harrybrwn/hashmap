#include <stdio.h>

#ifndef HASHMAP_MAIN_TEST
#include "hashmap.c"

#ifndef AUTOTEST
#define AUTOTEST
#endif
#include "tests/utest.h"

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
#endif /* HASHMAP_MAIN_TEST */

// clang-format off
TEST(macros) {
    struct node* n = NULL;
    assert(height(n) == -1);
    n = newnode(1);
    assert(height(n) == 0);
    delete_tree(n);
}

TEST(rotations) {
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
}

TEST(insert_node) {
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
}

void print_tree(struct node* root, int level, int type)
{
    if (root != NULL)
    {
        int i;
        for (i = 0; i < (level * 4); i++)
            printf("%c", ' ');

        if (type < 0)
            printf("left: {");
        else if (type > 0)
            printf("right: {");
        else if (type == 0)
            printf("root: {");

        printf("val: %lu, ", root->_hash_val);
        printf("height: %d", root->height);
        printf("}\n");

        print_tree(root->left, level + 1, -1);
        print_tree(root->right, level + 1, 1);
    }
}

static void _print_avl(struct node* n, int space, char side, const int incr)
{
    if (n == NULL) return;
    space += incr;
    _print_avl(n->right, space, 'R', incr);
    printf("\n");
    int i;
    for (i = incr; i < space; i++) { printf(" "); }

    // printf("(%lu)\n", n->_hash_val);
    // printf("(%s)\n", n->key);
    printf("(%d)\n", n->height);

    _print_avl(n->left, space, 'L', incr);
    if (side == 'L' && n->left == NULL && n->right == NULL) printf("\n");
}
void print_avl(struct node* n)
{
    _print_avl(n, 0, 'C', 20);
}

TEST(search) {
    struct node* n = newnode(10);
    assert_eq(n, search(n, 10));

    insert_node(&n, newnode(5));
    insert_node(&n, newnode(15));
    insert_node(&n, newnode(25));
    insert_node(&n, newnode(1));

    assert_eq(5UL, search(n, 5)->_hash_val);
    assert_eq(15UL, search(n, 15)->_hash_val);
    assert_eq(25UL, search(n, 25)->_hash_val);
    assert_eq(1UL, search(n, 1)->_hash_val);

    size_t i;
    for (i = 0; i < 50; i++)
        insert_node(&n, newnode((hash_t)i));
    for (i = 0; i < 50; i++)
        eq(i, search(n, i)->_hash_val);
    delete_tree(n);
}

TEST(insert_node_on_root)
{
    struct node* n = newnode(15);
    n->key = "root node";

    struct node* new_root = newnode(15);
    new_root->key = "new root node";

    insert_node(&n, newnode(1));
    insert_node(&n, newnode(30));

    eq(n->_hash_val, 15UL);
    eq(n->key, "root node");

    insert_node(&n, new_root);
    eq(n->_hash_val, 15UL);
    eq(n->key, "new root node");
    eq(n->left->_hash_val, 1UL);
    eq(n->right->_hash_val, 30UL);

    insert_node(&n, newnode(31));
    eq(31UL, n->right->right->_hash_val);

    insert_node(&n, newnode(32));
    eq(31UL, n->right->_hash_val);
    eq(30UL, n->right->left->_hash_val);
    eq(32UL, n->right->right->_hash_val);

    n->right->key = "old thirty one node";
    struct node* thirty_one = newnode(31);
    thirty_one->key = "thirty-one";
    insert_node(&n, thirty_one);
    eq("thirty-one", n->right->key);

    delete_tree(n);
}

TEST(prehash) {
    hash_t a = prehash("abc");
    hash_t b = prehash("cba");
    hash_t c = prehash("bca");

    assert(a != b);
    assert(c != a);
    assert(c != b);

    assert_eq(prehash("abc123"), prehash("abc123"));
    assert(prehash(" ") != prehash("  "));

    assert_eq(prehash("qwerty"), fnv_1("qwerty"));
}

#include <math.h>

TEST(small_map, .ignore=0)
{
    Map* m = create_map(1);
    const int n = 256;
    int vals[n], i;
    char **keys = rand_keys(n);

    vals[0] = 0;
    map_put(m, keys[0], &vals[0]);
    for (i = 1; i < n; i++)
    {
        vals[i] = i*i;
        map_put(m, keys[i], &vals[i]);
    }

    // print_avl(m->__data[0]);

    int h = (int)log2(n);
    int height = m->__data[0]->height;
    assert(h == height || h == height-1);
    // eq(h, height);

    free_string_arr(keys, n);
    map_free(m);
}
