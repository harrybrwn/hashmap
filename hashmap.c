#include <stdlib.h>
#include <stdio.h>

#include "hashmap.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned long hash_t;

struct node {
	char* key;
	MapValue value;

	struct node* _right, * _left;
	hash_t       _hash_val;
};

// 'djb2' by Dan Bernstein
hash_t hash(char* str) {
	hash_t hash = 5381;
	int    c;

	while ((c = *str++))
		hash = ((hash << 5) + hash) + c;

	return hash;
}

static Map* create_map(size_t);
static void delete_tree(struct node*);

Map* New_Map() {
	return create_map(32);
}

void Map_close(Map* m) {
	for (int i = 0; i < m->__size; i++) {
		delete_tree(m->__data[i]);
	}
	free(m->__data);
	free(m);
}

static void insert_node(struct node* root, struct node* new) {
	if (new->_hash_val < root->_hash_val) {
		if (root->_left != NULL) {
			return insert_node(root->_left, new);
		}
		root->_left = new;
	} else if (new->_hash_val > root->_hash_val) {
		if (root->_right != NULL) {
			return insert_node(root->_right, new);
		}
		root->_right = new;
	}
}

static void print_node(struct node* leaf) {
	char* left_key;
	char* right_key;

	if (leaf->_left != NULL)
		left_key = leaf->_left->key;
	else
		left_key = "nil";

	if (leaf->_right != NULL)
		right_key = leaf->_right->key;
	else
		right_key = "nil";

	printf("        (%s)\n", leaf->key);
	printf("       /          \\\n");
	printf("     (%s)      (%s)\n", left_key, right_key);
}

static void print_tree(struct node* root) {
	print_node(root);

	if (root->_left != NULL)
		print_tree(root->_left);
	if (root->_right != NULL)
		print_tree(root->_right);
}

void print_map(Map* m) {
	for (int i = 0; i < m->__size; i++) {
		if (m->__data[i] == NULL) {
			printf("nil ");
			continue;
		}
		printf("%s ", m->__data[i]->key);
	}
	printf("\n");

	for (int i = 0; i < m->__size; i++) {
		if (m->__data[i] == NULL) {
			continue;
		}
		printf("\n%s\n", m->__data[i]->key);
		print_tree(m->__data[i]);
		printf("\n\n");
	}
}

struct node* search(struct node* root, hash_t key_hash) {
	if (root->_hash_val == key_hash) {
		return root;
	}

	if (key_hash < root->_hash_val) {
		return search(root->_left, key_hash);
	} else if (key_hash > root->_hash_val) {
		return search(root->_right, key_hash);
	}
	return NULL;
}

static void delete_tree(struct node* leaf) {
	if (leaf != NULL) {
		delete_tree(leaf->_right);
		delete_tree(leaf->_left);
		free(leaf);
	}
}

static struct node* _new_node(char* key, MapValue val, hash_t key_hash) {
	struct node* n = malloc(sizeof(struct node));
	n->key = key;
	n->value = val;
	n->_left = NULL;
	n->_right = NULL;
	n->_hash_val = key_hash;
	return n;
}

static void add_node(Map* m, struct node* node, int index) {
	struct node* head_node = m->__data[index];
	/*
	 *  If the node at the index is empty, put the node there. If there is a
	 *  node at that index but the raw hash value is the same then the key was
	 *  the same and we are going to free the old node and remplace it. If the
	 *  raw hash is different then we have a hash collition and we are inserting
	 *  a node into the binary tree rooted at the 'head_node'
	 */
	if (head_node == NULL) {
		m->__data[index] = node;
	} else if (head_node->_hash_val == node->_hash_val) {
		/*
		 *  Getting two hash values that are the same is extremly unlikly given
		 *  different inputs. This is different that getting a hash collition
		 *  which is after you take the modulus of the hash.
		 *  If the hash value of the two is the same then we are going to free
		 *  the memory from the old node and replace it with the new one.
		 */
		free(m->__data[index]);
		m->__data[index] = node;
	} else {
		insert_node(head_node, node);
	}
}

void put(Map* m, char* key, MapValue val) {
	hash_t key_hash = hash(key);
	int    index = key_hash % m->__size;
	add_node(m, _new_node(key, val, key_hash), index);
}

MapValue get(Map* m, char* key) {
	hash_t k_hash = hash(key);
	int    index = k_hash % m->__size;

	struct node* root = m->__data[index];
	if (root == NULL) {
		return NULL;
	}

	if (k_hash != root->_hash_val) {
		struct node* n = search(root, k_hash);
		if (n == NULL) {
			return NULL;
		}
		return n->value;
	}
	return root->value;
}

static void delete_leaf(struct node** leaf, hash_t key_hash) {
	if ((*leaf) == NULL) {
		return;
	}
	if ((*leaf)->_hash_val == key_hash) {
		free(*leaf);
		(*leaf) = NULL;
		return;
	}

	if (key_hash < (*leaf)->_hash_val) {
		return delete_leaf(&(*leaf)->_left, key_hash);
	} else if (key_hash > (*leaf)->_hash_val) {
		return delete_leaf(&(*leaf)->_left, key_hash);
	}
}

void delete(Map* m, char* key) {
	hash_t k_hash = hash(key);
	int    index = k_hash % m->__size;

	struct node* root = m->__data[index];
	if (root == NULL) {
		return;
	}

	if (k_hash != root->_hash_val) {
		delete_leaf(&root, k_hash);
	} else {
		free(m->__data[index]);
		m->__data[index] = NULL;
	}
}

static void copy_nodes(Map* m, struct node* n) {
	if (n->_left != NULL) {
		copy_nodes(m, n->_left);
	}
	if (n->_right != NULL) {
		copy_nodes(m, n->_right);
	}

	int index = n->_hash_val % m->__size;
	add_node(m, _new_node(n->key, n->value, n->_hash_val), index);
}

void Map_resize(Map** old_m, size_t size) {
	Map* new_m = create_map(size);

	struct node* tmp;
	for (int i = 0; i < (*old_m)->__size; i++) {
		tmp = (*old_m)->__data[i];
		if (tmp != NULL) {
			copy_nodes(new_m, tmp);
		}
	}
	Map_close(*old_m);
	(*old_m) = new_m;
}

static Map* create_map(size_t size) {
	Map* m = malloc(sizeof(Map));
	m->__size = size;
	m->__data = malloc(sizeof(struct node*) * m->__size);
	for (int i = 0; i < size; i++)
		m->__data[i] = NULL;
	return m;
}

#ifdef __cplusplus
}
#endif
