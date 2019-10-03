#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashmap.c" // to test static methods
#define HASHMAP_C_INC
#define AUTOTEST
#include "tests/utest.h"
// test.h must come after utest.h
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

#define HASHMAP_MAIN_TEST
#include "tests/c/internal.c"
#include "tests/c/map_test.c"

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
TEST(collitions) {
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
}

static void delete_node(struct node** root, hash_t k_hash)
{
    *root = _delete_node(*root, k_hash, 0);
}

// clang-format off
TEST(delete_node0) {
    struct node* root;
    root = newnode(0);
    int i;
    for (i = 1; i <= 30; i++)
        insert_node(&root, newnode(i));

    delete_node(&root, 23);
    assert_eq(24UL, root->right->_hash_val);
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
}

TEST(delete_node1) {
    struct node* root;
    root = newnode(0);
    const int n = 10;
    int i;
    for (i = 1; i < n; i++)
        insert_node(&root, newnode(i));
    for (i = 1; i < n; i++)
        delete_node(&root, i);
    assert_eq(0, height(root));
    assert_eq((hash_t)0, root->_hash_val);
    delete_tree(root);
}

/**
 * This test will fail if data is lost when deleting the root
 * node of a bst.
 */
TEST(delete_root) {
    struct node* root = newnode(50);
    insert_node(&root, newnode(25));
    insert_node(&root, newnode(75));
    insert_node(&root, newnode(20));
    insert_node(&root, newnode(30));
    insert_node(&root, newnode(70));
    insert_node(&root, newnode(80));

    delete_node(&root, 50);
    assert_eq(70UL, root->_hash_val);
    assert_eq(75UL, root->right->_hash_val);
    assert_eq(25UL, root->left->_hash_val);

    delete_node(&root, 30);
    assert_eq(NULL, root->left->right);
    assert(root->height == 2);
    assert(root->left->_hash_val == 25);
    assert(root->_hash_val == 70);

    delete_node(&root, 70);
    eq(root->_hash_val, 75UL);
    delete_node(&root, 75);
    eq(root->_hash_val, 25UL);
    delete_node(&root, 25);
    eq(root->_hash_val, 80UL);
    delete_node(&root, 80);
    eq(root->_hash_val, 20UL);
    delete_node(&root, 20);
    assert(root == NULL);
}

TEST(delete_node) {
    struct node* root = newnode(25);
    int nodes[] = { 12, 27, 5, 20, 26, 4, 7 };
    int i;
    for (i = 0; i < 7; i++)
        insert_node(&root, newnode(nodes[i]));

    /* delete nodes with no children */
    insert_node(&root, newnode(28));
    assert_eq(28UL, root->right->right->_hash_val);
    delete_node(&root, 28);
    assert_eq(NULL, root->right->right);
    insert_node(&root, newnode(19));
    assert_eq(19UL, root->left->right->left->_hash_val);
    assert(search(root, 19) != NULL);

    /* delete node with uneven child heights */
    assert_eq(12UL, root->left->_hash_val);
    assert_eq(20UL, root->left->right->_hash_val);
    assert_eq(5UL, root->left->left->_hash_val);
    delete_node(&root, 12);

    assert_eq(19UL, root->left->_hash_val);
    assert_eq(20UL, root->left->right->_hash_val);
    assert_eq(5UL, root->left->left->_hash_val);

    insert_node(&root, newnode(6));
    assert_eq(6UL, root->left->left->right->_hash_val);
    delete_node(&root, 5);

    assert_eq(6UL, root->left->left->_hash_val);
    assert_eq(4UL, root->left->left->left->_hash_val);
    delete_node(&root, 7);
    assert_eq(19UL, root->left->_hash_val);
    delete_node(&root, 20);
    assert_eq(6UL, root->left->_hash_val);
    delete_tree(root);
}

TEST(avl_insert) {
    int vals[] = { 3, 7, 4, 2 };
    struct node* root = newnode(5);
    int i;
    for (i = 0; i < 4; i++)
        insert_node(&root, newnode(vals[i]));
    assert_eq(2, root->height);

    assert_eq(5UL, root->_hash_val);
    delete_tree(root);
    root = NULL;
}

TEST(avl_balence) {
    struct node* root = newnode(41);
    int vals[] = { 65, 20, 50, 26, 11, 29, 23 };
    int i;
    for (i = 0; i < 7; i++)
        insert_node(&root, newnode(vals[i]));

    insert_node(&root, newnode(55));
    assert_eq(55UL, root->right->_hash_val);
    assert_eq(65UL, root->right->right->_hash_val);
    assert_eq(root->right->left->_hash_val, (hash_t)50);
    delete_tree(root);

    root = newnode(41);
    int newvals[] = { 65, 20, 50, 29, 11, 26 };
    for (i = 0; i < 6; i++)
        insert_node(&root, newnode(newvals[i]));
    assert_eq(29UL, root->left->right->_hash_val);

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
}

TEST(test) {
    char* keys[] = { "one", "two", "three", "four", "five", "six", "seven", "eight" };
    Map* m = create_map(2);
    int a = 1;
    int i;

    for (i = 0; i < 8; i++)
        map_put(m, keys[i], &a);
    map_close(m);
    assert_eq(0, 0);
}

TEST(add_node) {
    // this test will break if the prehash changes

    Map* m = create_map(1); // only one bucket in the map
    int one = 1, two = 2, three = 3;
    map_put(m, "one", &one);
    map_put(m, "two", &two);
    map_put(m, "three", &three);
    assert(m->item_count == 3);

    hash_t one_hash = m->__data[0]->_hash_val;
    // printf("\n");
    // printf("%s\n", m->__data[0]->key);
    // printf("%s\n", m->__data[0]->right->key);
    // printf("%s\n", m->__data[0]->left->key);

    assert(strcmp(m->__data[0]->key, "one") == 0);
    assert(1 == *(int*)m->__data[0]->value);
    assert(strcmp(m->__data[0]->left->key, "two") == 0);
    assert(2 == *(int*)m->__data[0]->left->value);
    assert(strcmp(m->__data[0]->right->key, "three") == 0);
    assert(3 == *(int*)m->__data[0]->right->value);

    add_node(m, _new_node("one", &three, one_hash), 0);

    assert_eq(0, strcmp(m->__data[0]->key, "one"));
    assert_eq(m->__data[0]->key, "one");
    assert(3 == *(int*)m->__data[0]->value);

    assert(m->__data[0]->right != NULL);
    assert(m->__data[0]->left != NULL);

    assert(strcmp(m->__data[0]->left->key, "two") == 0);
    assert(2 == *(int*)m->__data[0]->left->value);
    assert(strcmp(m->__data[0]->right->key, "three") == 0);
    assert(3 == *(int*)m->__data[0]->right->value);

    map_free(m);
}

TEST(key_struct_get) {
    struct key k = {"strkey", 6};
    hash_t h1 = prehash("strkey");
    hash_t h2 = prehash_key(k);
    assert_eq(h1, h2);

    Map* map = create_map(31);
    char* val = "test value";
    map_put(map, "strkey", (mapval_t)val);

    char* res = (char*)map_key_get(map, k);
    assert(res != NULL);
    assert_eq(0, strncmp(res, "test value", 10));

    hash_t keykeyhash = prehash_key((struct key){&k, sizeof(k)});
    put_from_hash(map, "", keykeyhash, "keykeyvalue");
    char* keykeyvalue = (char*)get_from_hash(
        map,
        prehash_key((struct key){&k, sizeof(k)})
    );
    assert_eq(0, strcmp(keykeyvalue, "keykeyvalue"));
    assert_eq(keykeyvalue, "keykeyvalue");
    map_free(map);
}

TEST(key_struct_put)
{
    struct key k = {"the answer", 10};
    hash_t h1 = prehash("the answer");
    hash_t h2 = prehash_key(k);
    assert_eq(h1, h2);

    Map* m = new_map();
    int val = 42;
    map_key_put(m, k, &val);
    eq(42, *(int*)map_get(m, "the answer"));
    map_close(m);
}

#ifdef TRASH_KEY
#undef TRASH_KEY
#endif

static Map* tmap;
char **tmap_keys;

void map_free_key_SETUP(void)
{
    tmap = create_map(33);
    tmap_keys = rand_keys(3);
}

void map_free_key_TEARDOWN(void)
{
    free(tmap_keys);
    free(tmap->__data);
    free(tmap);
}

// checking this test with valgrind
TEST(valgrind_map_delete_free_key,
    .setup = map_free_key_SETUP,
    .teardown = map_free_key_TEARDOWN
)
{
    int i;
    int val = 99;
    char *copys[3];
    for (i = 0; i < 3; i++) {
        copys[i] = malloc(strlen(tmap_keys[i]) + 1);
        strcpy(copys[i], tmap_keys[i]);
    }

    for (i = 0; i < 3; i++)
        map_put(tmap, tmap_keys[i], &val);
    for (i = 0; i < 3; i++)
        eq(99, *(int*)map_get(tmap, tmap_keys[i]));
    for (i = 0; i < 3; i++)
        map_delete_free_key(tmap, tmap_keys[i]);
    for (i = 0; i < 3; i++)
        eq(NULL, map_get(tmap, copys[i]));

    for (i = 0; i < 3; i++)
        free(copys[i]);
}

TEST(valgrind_map_clear_free_keys,
    .setup = map_free_key_SETUP,
    .teardown = map_free_key_TEARDOWN
)
{
    int i, val = 55;
    char *copys[3];
    for (i = 0; i < 3; i++) {
        copys[i] = malloc(strlen(tmap_keys[i]) + 1);
        strcpy(copys[i], tmap_keys[i]);
    }

    for (i = 0; i < 3; i++)
        map_put(tmap, tmap_keys[i], &val);

    map_clear_free_keys(tmap);

    for (i = 0; i < 3; i++)
        eq(NULL, map_get(tmap, copys[i]));
    for (i = 0; i < 3; i++)
        free(copys[i]);
}

TEST(node_is_null_after_delete)
{
    struct node* n = newnode(5);
    insert_node(&n, newnode(10));
    assert(n->right != NULL);
    n = _delete_node(n, 10, 0);
    eq(n->left, NULL);
    eq(n->_hash_val, 5UL);
    delete_tree(n);
}

#ifdef TRASH_KEY
#error "TRASH_KEY will break my tests"
#endif

TEST(auto_free_keys, .ignore = 1)
{
    int one = 1;
    Map* m = create_map(21);

    char* k1 = malloc(4);
    strcpy(k1, "key");
    map_put(m, k1, &one);

    char* k2 = malloc(12);
    strncpy(k2, "another_key", 12);
    map_put(m, k2, &one);

    eq(2UL, m->item_count);
    map_delete_free_key(m, "not in map");
    eq(2UL, m->item_count);

    eq(1, *(int*)map_get(m, "key"));
    eq(&one, map_get(m, "key"));
    map_delete_free_key(m, "key");
    // map_delete(m, "key");
    eq(NULL, map_get(m, "key"));

    eq(1, *(int*)map_get(m, "another_key"));
    eq(&one, map_get(m, "another_key"));
    map_clear_free_keys(m); // invalid free here, probably trying to free, "key"
    eq(NULL, map_get(m, "another_key"));

    free(m->__data);
    free(m);
}