#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "hashmap.h"

#define ARR_CMP(ARR1, LEN1, ARR2, LEN2) \
    for (int i = 0; i < LEN1; i++) { \
        for (int k = 0; k < LEN2; k++) { \
            if (strcmp(ARR1[i], ARR2[k]) == 0) \
                goto Same_Arr_val; \
        } \
        assert(0); \
    Same_Arr_val:; \
    }

char* randstring(size_t length) {
    static char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!";
    char *str = NULL;
    if (length) {
        str = malloc(sizeof(char) * (length + 1));
        if (str) {
            for (int n = 0;n < length;n++) {
                int key = rand() % (int)(sizeof(charset) - 1);
                str[n] = charset[key];
            }
            str[length] = '\0';
        }
    }
    return str;
}

typedef unsigned long hash_t;
hash_t prehash(char* str);

static char** rand_keys(int len) {
    char** keys = malloc(sizeof(char*)*len);
    for (int i = 0; i < len; i++) {
        keys[i] = randstring(10);
    }
    return keys;
}

static char** collition_keys(size_t str_len,
                             size_t mapsize,
                             hash_t (*hash_fn)(char*),
                             int len)
{
    int k = 0;
    char** arr = malloc(sizeof(char*)*len);
    arr[k++] = randstring(str_len);
    hash_t h, base_hash = hash_fn(arr[0]);
    char* str;

    while (k != len) {
        str = randstring(str_len);
        h = hash_fn(str);
        if ((h % mapsize) == (base_hash % mapsize))
            arr[k++] = str;
        else
            free(str);
    }
    return arr;
}

void free_string_arr(char** arr, int len) {
    for (int i = 0; i < len; i++)
        free(arr[i]);
    free(arr);
}

void test_prehash() {
    hash_t a = prehash("abc");
    hash_t b = prehash("cba");
    hash_t c = prehash("bca");

    assert(a != b);
    assert(c != a);
    assert(c != b);
}

void TestMap() {
	Map* map = New_Map();

    char* val = "this is a value";
	Map_put(map, "key", val);
    assert(strcmp(val, (char*)Map_get(map, "key")) == 0);

    val = "updated value stored in the map";
	Map_put(map, "key", val);
    assert(strcmp(val, (char*)Map_get(map, "key")) == 0);

	Map_close(map);
}

void test_Map_delete() {
    Map* m = New_Map();
    int val = 9001;
    Map_put(m, "key1", &val);
    assert(*(int*)Map_get(m, "key1") == val);

    Map_delete(m, "key1");
    int* res = (int*)Map_get(m, "key1");
    assert(res == NULL);
    Map_close(m);
}

#pragma pack(1)
struct node {
	char*    key;
	MapValue value;
	unsigned char height;

	struct
	node*  right, * left;
	hash_t _hash_val;
};

void print_tree(struct node* root, int level, int type) {
    if (root != NULL) {
        for (int i = 0; i < (level*4); i++) printf("%c", ' ');

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

static void _print_avl(struct node* n, int space, char side) {
    if (n == NULL)
        return;

    space += SPACE_INCR;
    _print_avl(n->right, space, 'R');

    printf("\n");
    for (int i = SPACE_INCR; i < space; i++)
        printf(" ");

    printf("(%lu)\n", n->_hash_val);

    _print_avl(n->left, space, 'L');
    if (side == 'L' && n->left == NULL && n->right == NULL)
        printf("\n");
}

void print_avl(struct node* n) {
    _print_avl(n, 0, 'C');
}

void test_collitions() {
	Map* m = New_Map();
	int n = 20;
    // these keys all collide is a hash table of length 'm->__size' using 'prehash'
    char** keys = collition_keys(6, m->__size, prehash, n);

	int x[n];
	for (int i = 0; i < n; i++) {
		x[i] = i;
		Map_put(m, keys[i], &x[i]);
	}
    // int index = prehash(keys[0]) % m->__size;
    // print_avl(m->__data[index]);

    for (int i = 0; i < n; i++)
        assert(i == *(int*)Map_get(m, keys[i]));

    int nonNullKeys = 0;
    for (int i = 0; i < m->__size; i++) {
        if (m->__data[i] != NULL)
            nonNullKeys++;
    }
    assert(nonNullKeys == 1);

    free_string_arr(keys, n);
	Map_close(m);
}

void test_Map_resize() {
    Map* m = New_Map();
    int n = 28;
    char** keys = rand_keys(n);

    int x[n];
    for (int i = 0; i < n; i++) {
        x[i] = i;
        Map_put(m, keys[i], &x[i]);
    }
    Map_resize(&m, n + 1);
    assert(m->__size == n + 1);

    char* mapkeys[m->item_count];
    Map_keys(m, mapkeys);
    ARR_CMP(mapkeys, m->item_count, keys, n);

    Map_resize(&m, 3);
    for (int i = 0; i < n; i++)
        assert(i == *(int*)Map_get(m, keys[i]));
    assert(m->__size == 3);
    free_string_arr(keys, n);
    Map_close(m);
}

void insert_node(struct node** root, struct node* new);

void delete_tree(struct node* leaf) {
	if (leaf != NULL) {
		delete_tree(leaf->right);
		delete_tree(leaf->left);
		free(leaf);
	}
}

static struct node* newnode(hash_t val) {
    struct node* n = malloc(sizeof(struct node));
    n->_hash_val = val;
    n->height = 0;
    n->left = NULL;
    n->right = NULL;
    return n;
}

void test_avl_insert() {
    int vals[] = {3, 7, 4, 2};
    struct node* root = newnode(5);

    for (int i = 0; i < 4; i++) insert_node(&root, newnode(vals[i]));
    assert(root->height == 2);

    assert(root->_hash_val == 5);
    delete_tree(root);
    root = NULL;
}

void test_avl_balence() {
    struct node* root = newnode(41);
    int vals[] = {65, 20, 50, 26, 11, 29, 23};

    for (int i = 0; i < 7; i++)
        insert_node(&root, newnode(vals[i]));

    insert_node(&root, newnode(55));
    assert(root->right->_hash_val == 55);
    assert(root->right->right->_hash_val == 65);
    assert(root->right->left->_hash_val == 50);
    delete_tree(root);
    root = NULL;

    root = newnode(41);
    int newvals[] = {65, 20, 50, 29, 11, 26};
    for (int i = 0; i < 6; i++)
        insert_node(&root, newnode(newvals[i]));
    assert(root->left->right->_hash_val == 29);

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

#include <pthread.h>
#include <unistd.h>

void *test_threads_inner1(void *data) {
    char* res = Map_get((Map*)data, "test");
    printf("testing thread 1\n");
    assert(strcmp(res, "testing multi-threads") == 0);
    return NULL;
}

void* test_threads_inner2(void* data) {
    char* res = Map_get((Map*)data, "test");
    printf("testing thread 2\n");
    assert(strcmp(res, "testing multi-threads") == 0);
    return (void*)69;
}

void test_threads_main() {
    pthread_t thread1, thread2;

    Map* map = New_Map();
    Map_put(map, "test", "testing multi-threads");

    pthread_create(&thread1, NULL, test_threads_inner1, map);
    pthread_create(&thread2, NULL, test_threads_inner2, map);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    printf("main thread\n");
    Map_close(map);
}

#include <unistd.h>

void test() {
    printf("%lu\n", sizeof(size_t));
    printf("Map size: %lu\n", sizeof(Map));
    printf("node size: %lu\n", sizeof(struct node));
}

int main() {
    printf("start\n");
    TestMap();
	test_collitions();
    test_prehash();
    test_Map_delete();
    test_Map_resize();
    test_avl_insert();
    test_avl_balence();
    // test_threads_main();
    test();

    printf("OK %s\n", __FILE__);
}
