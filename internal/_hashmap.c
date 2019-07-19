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