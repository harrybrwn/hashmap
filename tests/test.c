#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "hashmap.h"
#include "tests/test_common.h"

#define ARR_CMP(ARR1, LEN1, ARR2, LEN2) \
    for (int i = 0; i < LEN1; i++) { \
        for (int k = 0; k < LEN2; k++) { \
            if (strcmp(ARR1[i], ARR2[k]) == 0) \
                goto Same_Arr_val; \
        } \
        assert(0); \
    Same_Arr_val:; \
    }

typedef unsigned long hash_t;

hash_t _prehash(char* str) {
    hash_t hash = 5381;
	int    c;

	while ((c = *str++))
		hash = ((hash << 5) + hash) + c;

	return hash;
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

void test_prehash() {
    hash_t a = _prehash("abc");
    hash_t b = _prehash("cba");
    hash_t c = _prehash("bca");

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
    /* these keys all collide is a hash table of length 'm->__size' using 'prehash' */
    char** keys = collition_keys(6, m->__size, _prehash, n);

	int x[n];
	for (int i = 0; i < n; i++) {
		x[i] = i;
		Map_put(m, keys[i], &x[i]);
	}

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

void test_Map_keys()
{
	Map* m = New_Map();
	int n = 39;
	char** keys = rand_keys(n);
	int data[n];
	for (int i = 0; i < n; i++)
	{
		data[i] = i;
		Map_put(m, keys[i], &data[i]);
	}
	assert(m->item_count == n);
	
	char* mapkeys[m->item_count];
	Map_keys(m, mapkeys);
	ARR_CMP(mapkeys, m->item_count, keys, n);
	Map_close(m);
	free_string_arr(keys, n);
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

struct node* pop_min(struct node** root);
struct node* pop_max(struct node** root);

void test_minmax() {
	struct node* root = newnode(10);
	size_t nodes[8] = {5, 15, 6, 4, 14, 16,
					   13, 17};

	for (int i = 0; i < 8; i++)
		insert_node(&root, newnode(nodes[i]));

	struct node* max = pop_max(&root);
	assert(max != NULL);
	assert(max->left == NULL);
	assert(max->right == NULL);
	assert(max->_hash_val == 17);
	assert(root->right->right->right == NULL);
	free(max);

	max = pop_min(&root->right);
	assert(max != NULL);
	assert(max->left == NULL);
	assert(max->right == NULL);
	assert(max->_hash_val == 13);
	assert(root->right->left->left == NULL);
	free(max);

	struct node* min = pop_min(&root);
	assert(min->left == NULL);
	assert(min->right == NULL);
	assert(min->_hash_val == 4);
	assert(root->left->left == NULL);
	free(min);
	delete_tree(root);

	root = newnode(10);
	max = pop_max(&root);
	assert(max->_hash_val == 10);
	free(max);
	free(root);

	root = newnode(99);
	max = pop_min(&root);
	assert(max->_hash_val == 99);
	assert(root == NULL);
	free(max);
}

static struct node* search(struct node* root, hash_t key_hash)
{
	if (!root)
		return NULL;
	if (root->_hash_val == key_hash)
		return root;

	if (key_hash < root->_hash_val)
		return search(root->left, key_hash);
	else if (key_hash > root->_hash_val)
		return search(root->right, key_hash);
	return NULL;
}

void delete_node(struct node** leaf, hash_t key_hash);

void test_delete_node0()
{
	struct node* root;
	root = newnode(0);
	for (int i = 1; i <= 30; i++)
		insert_node(&root, newnode(i));

	delete_node(&root, 23);
	assert(root->right->_hash_val == 24);
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
	for (int i = 1; i <= n; i++)
		insert_node(&root, newnode(i));

	for (int i = 1; i <= n; i++)
	{
		delete_node(&root, i);
		assert(search(root, 0) != NULL);
	}
	assert(root->_hash_val == 0);
	assert(root->left == NULL);
	assert(root->right == NULL);
	free(root);
}

void test_delete_node1()
{
	struct node* root;
	root = newnode(0);
	int n = 10;
	for (int i = 1; i < n; i++)
		insert_node(&root, newnode(i));
	for (int i = 1; i < n; i++)
		delete_node(&root, i);
	delete_tree(root);
}

/**
 * This test will fail if data is lost when deleting the root
 * node of a bst.
 */
void test_delete_root()
{
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
	assert(root->_hash_val == 70);
	assert(root->right->_hash_val == 75);
	assert(root->left->_hash_val == 25);
	
	delete_node(&root, 30);
	assert(root->left->right == NULL);
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
}

void test_delete_node()
{
	struct node* root = newnode(25);
	int nodes[] = {12, 27, 5, 20, 26, 4, 7};
	for (int i = 0; i < 7; i++)
		insert_node(&root, newnode(nodes[i]));

	/* delete nodes with no children */
	insert_node(&root, newnode(28));
	assert(root->right->right->_hash_val == 28);
	delete_node(&root, 28);
	assert(root->right->right == NULL);
	insert_node(&root, newnode(19));
	assert(root->left->right->left->_hash_val == 19);
	assert(search(root, 19) != NULL);

	/* delete node with uneven child heights */
	assert(root->left->_hash_val == 12);
	assert(root->left->right->_hash_val == 20);
	assert(root->left->left->_hash_val == 5);
	delete_node(&root, 12);

	assert(root->left->_hash_val == 19);
	assert(root->left->right->_hash_val == 20);
	assert(root->left->left->_hash_val == 5);

	insert_node(&root, newnode(6));
	assert(root->left->left->right->_hash_val == 6);
	delete_node(&root, 5);

	assert(root->left->left->_hash_val == 6);
	assert(root->left->left->left->_hash_val == 4);
	delete_node(&root, 7);
	assert(root->left->_hash_val == 19);
	delete_node(&root, 20);
	assert(root->left->_hash_val == 6);
	delete_tree(root);
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

void test() {

}

int main() {
    test();
	test_collitions();
    test_prehash();
	test_delete_node0();
	test_delete_node1();
	test_delete_root();
	test_delete_node();
 
    TestMap();
    test_Map_delete();
	test_Map_resize();
	test_Map_keys();

	test_avl_insert();
    test_avl_balence();

    printf("OK %s\n", __FILE__);
}
