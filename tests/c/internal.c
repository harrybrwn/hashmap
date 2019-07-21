#include <stdio.h>
#include "internal/_hashmap.h"
#include "tests/test_common.h"

static struct node* newnode(hash_t val) {
    struct node* n = malloc(sizeof(struct node));
    n->_hash_val = val;
    n->height = 0;
    n->left = NULL;
    n->right = NULL;
    return n;
}

void test()
{
}

void test_macros()
{
	struct node* n = NULL;
	assert(height(n) == -1);
	n = newnode(1);
	assert(height(n) == 0);
}

void test_rotations()
{
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

void test_insert_node()
{
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

void test_search()
{
	struct node* n = newnode(10);
	assert(search(n, 10) == n);
	insert_node(&n, newnode(5));
	insert_node(&n, newnode(15));
	insert_node(&n, newnode(25));
	insert_node(&n, newnode(1));
	
	assert(search(n, 5)->_hash_val = 5);
	assert(search(n, 15)->_hash_val = 15);
	assert(search(n, 25)->_hash_val = 25);
	assert(search(n, 1)->_hash_val = 1);
	delete_tree(n);
}

void test_prehash()
{
	hash_t a = prehash("abc");
    hash_t b = prehash("cba");
    hash_t c = prehash("bca");

    assert(a != b);
    assert(c != a);
    assert(c != b);

	assert(prehash("abc123") == prehash("abc123"));
}

static testfunc tests[] = {
	test,
	test_prehash,
	test_macros,
	test_rotations,
	test_insert_node,
	test_search,
};

int main()
{
	int n = sizeof(tests) / sizeof(testfunc);
	Run(tests, n);
}