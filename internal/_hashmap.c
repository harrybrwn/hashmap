#include <stdlib.h>
#include "internal/_hashmap.h"


hash_t prehash(char* str)
{
	hash_t prime = 16777619;
	hash_t hash = 2166136261;

	char c;
	while ((c = *str++))
		hash = (hash ^ c) * prime;

	return hash;
}

struct node* node_rotateleft(struct node* n)
{
	struct node *head;

	head = n->right;
	n->right = head->left;
	head->left = n;

	n->height = MAX(height(n->left), height(n->right)) + 1;
	head->height = MAX(height(head->left), height(head->right)) + 1;
	return head;
}

struct node* node_rotateright(struct node* n)
{
	struct node *head;

	head = n->left;
	n->left = head->right;
	head->right = n;

	n->height = MAX(height(n->left), height(n->right)) + 1;
	head->height = MAX(height(head->left), height(head->right)) + 1;
	return head;
}

void insert_node(struct node** root, struct node* new) {
	/* insert left */
	if (new->_hash_val < (*root)->_hash_val) {
		if ((*root)->left != NULL) {
			insert_node(&(*root)->left, new);

			/* if left side is double-unbalenced... rotate right */
			if (HEIGHT_DIFF((*root)->left, (*root)->right) == 2) {
				balance_left_side(root, new->_hash_val);
			}
		} else
			(*root)->left = new;
	/* insert right */
	} else if (new->_hash_val > (*root)->_hash_val) {
		if ((*root)->right != NULL) {
			insert_node(&(*root)->right, new);

			/* if right side is double-unbalenced... rotate left */
			if (HEIGHT_DIFF((*root)->right, (*root)->left) == 2) {
				balance_right_side(root, new->_hash_val);
			}
		} else
			(*root)->right = new;
	}
	(*root)->height = MAX(height((*root)->left), height((*root)->right)) + 1;
}

struct node* search(struct node* root, hash_t key_hash) {
	if (root->_hash_val == key_hash)
		return root;

	if (key_hash < root->_hash_val)
		return search(root->left, key_hash);
	else if (key_hash > root->_hash_val)
		return search(root->right, key_hash);
	return NULL;
}

void delete_tree(struct node* leaf) {
	if (leaf != NULL) {
		delete_tree(leaf->right);
		delete_tree(leaf->left);
		free(leaf);
	}
}