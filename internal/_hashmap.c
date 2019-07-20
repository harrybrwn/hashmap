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

static _inline struct node* min_node(struct node* node)
{
    struct node* curr = node;

    while (curr->left != NULL)
        curr = curr->left;
    return curr;
}

struct node*
_delete_node(struct node* root, hash_t k_hash, int free_key)
{
    if (root == NULL)
        return root;

    if (k_hash < root->_hash_val)
	{
        root->left = _delete_node(root->left, k_hash, free_key);
	}
    else if(k_hash > root->_hash_val)
	{
        root->right = _delete_node(root->right, k_hash, free_key);
	}

    else if (root->_hash_val == k_hash) {
        if(!root->left || !root->right)
        {
            struct node *tmp;
			if (root->left)
				tmp = root->left;
			else
				tmp = root->right;

			if (tmp)
			{
				*root = *tmp;
			}
			else
			{
				tmp = root;
				root = NULL;
			}
			#ifndef TRASH_KEY
			if (free_key)
				free(tmp->key);
			#endif
            free(tmp);
        }
        else /* node has two children */
        {
            struct node* min = min_node(root->right);
            root->_hash_val = min->_hash_val;
		#ifndef TRASH_KEY
			root->key = min->key;
		#endif
			root->value = min->value;
            root->right = _delete_node(root->right, min->_hash_val, free_key);
        }
	}
	if (root == NULL)
      return root;

    root->height = 1 + MAXHEIGHT(root->left, root->right);

	int h_diff = HEIGHT_DIFF(root->left, root->right);

    if (h_diff > 1 && BALENCE(root->left) >= 0) {
		return node_rotateright(root);
	}
	else if (h_diff > 1 && BALENCE(root->left) < 0) {
		root->left = node_rotateleft(root->left);
		return node_rotateright(root);
	}
    else if (h_diff < -1 && BALENCE(root->right) <= 0) {
		return node_rotateleft(root);
	}
	else if (h_diff < -1 && BALENCE(root->right) > 0) {
		root->right = node_rotateright(root->right);
		return node_rotateleft(root);
    }

    return root;
}


void delete_tree(struct node* leaf) {
	if (leaf != NULL) {
		delete_tree(leaf->right);
		delete_tree(leaf->left);
		free(leaf);
	}
}

hash_t djb2(char* str)
{
	hash_t hash = 5381;
	int    c;

	while ((c = *str++))
		hash = ((hash << 5) + hash) + c;

	return hash;
}

hash_t sdbm(char *str)
{
    hash_t hash = 0;
    int c;

    while ((c = *str++))
        hash = c + (hash << 6) + (hash << 16) - hash;

    return hash;
}

hash_t rshash(char *str)
{
 	hash_t a = 63689, b = 378551, hash = 0;
  	int c;

 	while ((c = *str++)) {
    	hash = hash * a + c;
    	a = a * b;
  	}
  	return (hash & 0x7FFFFFFF);
}

hash_t fnv_1(char *str)
{
	hash_t prime = 16777619;
	hash_t hash = 2166136261;
	
	int c;
	while ((c = *str++))
		hash = (hash ^ c) * prime;
	
	return hash;
}
