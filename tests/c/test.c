#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashmap.c" // to test static methods
#include "tests/test_common.h"

#define ARR_CMP(ARR1, LEN1, ARR2, LEN2)                                                           \
    for (i = 0; (i) < (LEN1); i++)                                                                \
    {                                                                                             \
        for (k = 0; (k) < (LEN2); k++)                                                            \
        {                                                                                         \
            if (strcmp(ARR1[i], ARR2[k]) == 0)                                                    \
                goto Same_Arr_val;                                                                \
        }                                                                                         \
        assertion_failure("ARR_CMP failed: %s:%d\n", __FILE__, __LINE__);                         \
    Same_Arr_val:;                                                                                \
    }

static char** collition_keys(size_t str_len, size_t mapsize, hash_t (*hash_fn)(char*), int len)
{
    int k = 0;
    char** arr = malloc(sizeof(char*) * len);
    arr[k++] = randstring(str_len);
    hash_t h, base_hash = hash_fn(arr[0]);
    char* str;

    while (k != len)
    {
        str = randstring(str_len);
        h = hash_fn(str);
        if ((h % mapsize) == (base_hash % mapsize))
            arr[k++] = str;
        else
            free(str);
    }
    return arr;
}
// clang-format off
TEST(Map, ({
    Map* map = new_map();

    char* val = "this is a value";
    map_put(map, "key", val);
    assert_eq(0, strcmp(val, (char*)map_get(map, "key")));
    val = "updated value stored in the map";
    map_put(map, "key", val);
    assert_eq(0, strcmp(val, (char*)map_get(map, "key")));
    map_close(map);
}))
TEST(map_delete, ({
    Map* m = new_map();
    assert(m->item_count == 0);
    int val = 9001;
    map_put(m, "key1", &val);
    assert(m->item_count == 1);
    assert(*(int*)map_get(m, "key1") == val);

    map_delete(m, "key1");
    assert_eq(0, m->item_count);
    int* res = (int*)map_get(m, "key1");
    assert(res == NULL);
    map_delete(m, "invalid_key");
    assert(m->item_count == 0); // for that weird line in map_delete
    map_close(m);
}))

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

#define SPACE_INCR 20

static void _print_avl(struct node* n, int space, char side)
{
    if (n == NULL)
        return;

    space += SPACE_INCR;
    _print_avl(n->right, space, 'R');

    printf("\n");
    int i;
    for (i = SPACE_INCR; i < space; i++)
        printf(" ");

    printf("(%lu)\n", n->_hash_val);

    _print_avl(n->left, space, 'L');
    if (side == 'L' && n->left == NULL && n->right == NULL)
        printf("\n");
}

void print_avl(struct node* n)
{
    _print_avl(n, 0, 'C');
}
// clang-format off
TEST(collitions, ({
    Map* m = new_map();
    const size_t n = 20;
    /* these keys all collide is a hash table of length 'm->__size' using 'prehash' */
    char** keys = collition_keys(6, m->__size, prehash, n);

    int x[n];
    size_t i;

    for (i = 0; i < n; i++)
    {
        x[i] = i;
        map_put(m, keys[i], &x[i]);
    }

    for (i = 0; i < n; i++)
        assert(((int)i) == *(int*)map_get(m, keys[i]));

    int nonNullKeys = 0;
    for (i = 0; i < m->__size; i++)
    {
        if (m->__data[i] != NULL)
            nonNullKeys++;
    }
    assert_eq(1, nonNullKeys);

    free_string_arr(keys, n);
    map_close(m);
}))

TEST(map_keys, ({
    Map* m = new_map();
    const size_t n = 39;
    char** keys = rand_keys(n);
    int data[n];
    size_t i, k;
    for (i = 0; i < n; i++)
    {
        data[i] = i;
        map_put(m, keys[i], &data[i]);
    }
    assert(n == m->item_count);
    assert_eq(39, n);
    assert_eq(39, m->item_count);

    {
        char* mapkeys[39];
        map_keys(m, mapkeys);
        size_t k;
        ARR_CMP(mapkeys, m->item_count, keys, n);
    }

    map_close(m);
    free_string_arr(keys, n);

    m = create_map(71);
    char** mapkeys = malloc(sizeof(char*) * 71);
    for (i = 0; i < 71; i++)
        mapkeys[i] = NULL;
    for (i = 0; i < 71; i++)
        assert(mapkeys[i] == NULL);
    map_keys(m, mapkeys);
    for (i = 0; i < 71; i++)
        assert(mapkeys[i] == NULL);

    keys = rand_keys(71);
    for (i = 0; i < 71; i++)
        map_put(m, keys[i], keys[i]);

    map_keys(m, mapkeys);
    assert_eq(71, m->item_count);
    ASSERT_STR_ARR_EQ(mapkeys, keys, 71);

    free(mapkeys);
    free_string_arr(keys, 71);
    map_close(m);
    return 0;
}))

// clang-format off
TEST(map_keys2, ({
    Map* m = create_map(11);
    char* ks[] = { "one", "two", "three", "four", "five" };
    int i, k, data[5];
    for (i = 0; i < 5; i++)
    {
        data[i] = i;
        map_put(m, ks[i], &data[i]);
    }

    assert_eq(5, m->item_count);
    char* keys[5];
    map_keys(m, keys);
    ASSERT_STR_ARR_EQ(ks, keys, 5);
    map_close(m);
}))

// clang-format off
TEST(map_resize, ({
    Map* m = new_map();
    const size_t n = 28;
    char** keys = rand_keys(n);

    int x[n];
    size_t i;
    for (i = 0; i < n; i++)
    {
        x[i] = i;
        map_put(m, keys[i], &x[i]);
    }
    map_resize(&m, n + 1);
    assert_eq(n + 1, m->__size);

    map_resize(&m, 3);
    for (i = 0; i < n; i++)
        assert_eq(((int)i), *(int*)map_get(m, keys[i]));
    assert_eq(3, m->__size);
    free_string_arr(keys, n);
    map_close(m);
}))

static struct node* newnode(hash_t val)
{
    struct node* n = malloc(sizeof(struct node));
    n->_hash_val = val;
    n->height = 0;
    n->left = NULL;
    n->right = NULL;
    return n;
}

static void delete_node(struct node** root, hash_t k_hash)
{
    *root = _delete_node(*root, k_hash, 0);
}
// clang-format off
TEST(delete_node0, ({
    struct node* root;
    root = newnode(0);
    int i;
    for (i = 1; i <= 30; i++)
        insert_node(&root, newnode(i));

    delete_node(&root, 23);
    assert_eq(24, root->right->_hash_val);
    delete_node(&root, 22);
    assert(search(root, 20) != NULL);

    delete_node(&root, 17);
    assert(root->right->left->left->_hash_val == 18);
    delete_node(&root, 16);
    assert(root->right->left->left->_hash_val == 18);
    assert(root->right->left->left->left == NULL);
    assert(root->right->left->left->right == NULL);
    delete_tree(root);

    /**
     * tests for deleting the root node
     */
    root = newnode(5);
    insert_node(&root, newnode(3));
    delete_node(&root, 5);
    assert(root != NULL);
    assert(root->_hash_val == 3);
    assert(root->left == NULL);
    assert(root->right == NULL);
    insert_node(&root, newnode(7));
    assert(root->right->_hash_val == 7);
    assert(root->left == NULL);
    delete_node(&root, 3);
    assert(root->_hash_val == 7);
    assert(root->left == NULL);
    assert(root->right == NULL);
    delete_tree(root);

    root = newnode(0);
    int n = 100000;
    for (i = 1; i <= n; i++)
        insert_node(&root, newnode(i));

    for (i = 1; i <= n; i++)
    {
        delete_node(&root, i);
        assert(search(root, 0) != NULL);
    }
    assert(root->_hash_val == 0);
    assert(root->left == NULL);
    assert(root->right == NULL);
    free(root);
}))

TEST(delete_node1, ({
    struct node* root;
    root = newnode(0);
    const int n = 10;
    int i;
    for (i = 1; i < n; i++)
        insert_node(&root, newnode(i));
    for (i = 1; i < n; i++)
        delete_node(&root, i);
    assert_eq(0, height(root));
    assert_eq(0, root->_hash_val);
    delete_tree(root);
}))

// clang-format off
/**
 * This test will fail if data is lost when deleting the root
 * node of a bst.
 */
TEST(delete_root, ({
    /*
            delete(50)...
                       ---> 50
                              /   \
                            25	   75
                       /  \   /  \
             20   30 70  80

            result...
                                    70
                              /    \
                            25     75
                       /  \      \
                     20   30     80
            delete(30)...

                                    70
                               /  \
                             25    75
                            /        \
                       20	     80
     */
    struct node* root = newnode(50);
    insert_node(&root, newnode(25));
    insert_node(&root, newnode(75));
    insert_node(&root, newnode(20));
    insert_node(&root, newnode(30));
    insert_node(&root, newnode(70));
    insert_node(&root, newnode(80));

    delete_node(&root, 50);
    assert_eq(70, root->_hash_val);
    assert_eq(75, root->right->_hash_val);
    assert_eq(25, root->left->_hash_val);

    delete_node(&root, 30);
    assert_eq(NULL, root->left->right);
    assert(root->height == 2);
    assert(root->left->_hash_val == 25);
    assert(root->_hash_val == 70);

    delete_node(&root, 70);
    assert(root->_hash_val == 75);
    delete_node(&root, 75);
    assert(root->_hash_val == 25);
    delete_node(&root, 25);
    assert(root->_hash_val == 80);
    delete_node(&root, 80);
    assert(root->_hash_val == 20);
    delete_node(&root, 20);
    assert(root == NULL);
}))

// clang-format off
TEST(delete_node, ({
    struct node* root = newnode(25);
    int nodes[] = { 12, 27, 5, 20, 26, 4, 7 };
    int i;
    for (i = 0; i < 7; i++)
        insert_node(&root, newnode(nodes[i]));

    /* delete nodes with no children */
    insert_node(&root, newnode(28));
    assert_eq(28, root->right->right->_hash_val);
    delete_node(&root, 28);
    assert_eq(NULL, root->right->right);
    insert_node(&root, newnode(19));
    assert_eq(19, root->left->right->left->_hash_val);
    assert(search(root, 19) != NULL);

    /* delete node with uneven child heights */
    assert_eq(12, root->left->_hash_val);
    assert_eq(20, root->left->right->_hash_val);
    assert_eq(5, root->left->left->_hash_val);
    delete_node(&root, 12);

    assert_eq(19, root->left->_hash_val);
    assert_eq(20, root->left->right->_hash_val);
    assert_eq(5, root->left->left->_hash_val);

    insert_node(&root, newnode(6));
    assert_eq(6, root->left->left->right->_hash_val);
    delete_node(&root, 5);

    assert_eq(6, root->left->left->_hash_val);
    assert_eq(4, root->left->left->left->_hash_val);
    delete_node(&root, 7);
    assert_eq(19, root->left->_hash_val);
    delete_node(&root, 20);
    assert_eq(6, root->left->_hash_val);
    delete_tree(root);
}))

// clang-format off
TEST(avl_insert, ({
    int vals[] = { 3, 7, 4, 2 };
    struct node* root = newnode(5);
    int i;
    for (i = 0; i < 4; i++)
        insert_node(&root, newnode(vals[i]));
    assert_eq(2, root->height);

    assert_eq(5, root->_hash_val);
    delete_tree(root);
    root = NULL;
}))

// clang-format off
TEST(avl_balence, ({
    struct node* root = newnode(41);
    int vals[] = { 65, 20, 50, 26, 11, 29, 23 };
    int i;
    for (i = 0; i < 7; i++)
        insert_node(&root, newnode(vals[i]));

    insert_node(&root, newnode(55));
    assert_eq(55, root->right->_hash_val);
    assert_eq(65, root->right->right->_hash_val);
    assert_eq(root->right->left->_hash_val, 50);
    delete_tree(root);

    root = newnode(41);
    int newvals[] = { 65, 20, 50, 29, 11, 26 };
    for (i = 0; i < 6; i++)
        insert_node(&root, newnode(newvals[i]));
    assert_eq(29, root->left->right->_hash_val);

    insert_node(&root, newnode(23));
    assert(root->left->right->_hash_val == 26);
    delete_tree(root);
    root = NULL;

    root = newnode(5);
    insert_node(&root, newnode(1));
    insert_node(&root, newnode(8));
    insert_node(&root, newnode(3));
    assert(root->left->right->_hash_val == 3);
    insert_node(&root, newnode(4));
    assert(root->_hash_val == 5);
    insert_node(&root, newnode(2));
    assert(root->_hash_val == 3);
    insert_node(&root, newnode(6));
    assert(root->right->right->_hash_val == 8);
    insert_node(&root, newnode(7));
    assert(root->right->right->_hash_val == 7);
    delete_tree(root);
}))

// clang-format off
TEST(map_clear, ({
    Map* m = new_map();
    map_clear(m); /* shouldn't seg-fault when called on emty map */

    int i;
    int data[6];
    char* keys[] = { "one", "two", "three", "four", "five", "six" };

    for (i = 0; i < 6; i++)
    {
        data[i] = i;
        map_put(m, keys[i], &data[i]);
    }
    map_clear(m);
    size_t k;
    for (k = 0; k < m->__size; k++)
    {
        assert_eq(m->__data[k], NULL);
    }
    map_clear(m); /* shouldn't seg-fault when called on emty map */
    map_close(m);
}))
TEST(test, ({
    char* keys[] = { "one", "two", "three", "four", "five", "six", "seven", "eight" };
    Map* m = create_map(2);
    int a = 1;
    int i;

    for (i = 0; i < 8; i++)
        map_put(m, keys[i], &a);
    map_close(m);
    assert_eq(0, 0);
}))
TEST(add_node, ({
    Map* m = create_map(1); // only one bucket in the map
    int one = 1, two = 2, three = 3;
    map_put(m, "one", &one);
    map_put(m, "two", &two);
    map_put(m, "three", &three);
    hash_t one_hash = m->__data[0]->_hash_val;
    // printf("\n");
    // printf("%s\n", m->__data[0]->key);
    // printf("%s\n", m->__data[0]->right->key);
    // printf("%s\n", m->__data[0]->left->key);

    assert(strcmp(m->__data[0]->key, "one") == 0);
    assert(1 == *(int*)m->__data[0]->value);
    assert(strcmp(m->__data[0]->right->key, "two") == 0);
    assert(2 == *(int*)m->__data[0]->right->value);
    assert(strcmp(m->__data[0]->left->key, "three") == 0);
    assert(3 == *(int*)m->__data[0]->left->value);

    add_node(m, _new_node("one", &three, one_hash), 0);

    assert_eq(0, strcmp(m->__data[0]->key, "one"));
    assert(3 == *(int*)m->__data[0]->value);

    assert(m->__data[0]->right != NULL);
    assert(m->__data[0]->left != NULL);

    assert(strcmp(m->__data[0]->right->key, "two") == 0);
    assert(2 == *(int*)m->__data[0]->right->value);
    assert(strcmp(m->__data[0]->left->key, "three") == 0);
    assert(3 == *(int*)m->__data[0]->left->value);
}))

// clang-format off
RUN_TEST_SUITE (
    ADD_TEST(test),
    ADD_TEST(collitions),
    ADD_TEST(delete_node0),
    ADD_TEST(delete_node1),
    ADD_TEST(delete_root),
    ADD_TEST(delete_node),

    ADD_TEST(Map),
    ADD_TEST(map_delete),
    ADD_TEST(map_resize),
    ADD_TEST(map_keys),
    ADD_TEST(map_keys2),
    ADD_TEST(map_clear),
    ADD_TEST(add_node),

    ADD_TEST(avl_insert),
    ADD_TEST(avl_balence)
)