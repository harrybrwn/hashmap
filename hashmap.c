#include <stdlib.h>
#include <stdio.h>
#include "hashmap.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned long hash_t;

struct node {
	char*    key;
	MapValue value;
	int      height;

	struct
	node*  _right, * _left;
	hash_t _hash_val;
};

hash_t djb2(char* str) {
	hash_t hash = 5381;
	int    c;

	while ((c = *str++))
		hash = ((hash << 5) + hash) + c;

	return hash;
}

hash_t sdbm(char *str) {
    hash_t hash = 0;
    int c;

    while ((c = *str++))
        hash = c + (hash << 6) + (hash << 16) - hash;

    return hash;
}

hash_t rshash(char *str) {
 	hash_t a = 63689, b = 378551, hash = 0;
  	int c;

 	while ((c = *str++)) {
    	hash = hash * a + c;
    	a = a * b;
  	}
  	return (hash & 0x7FFFFFFF);
}

hash_t prehash(char* str) {
	return djb2(str);
}

static Map* create_map(size_t);
static void delete_tree(struct node*);
static void add_node(Map*, struct node*, int);
static struct node* _new_node(char*, MapValue, hash_t);
static struct node* search(struct node*, hash_t);
static void delete_leaf(struct node**, hash_t);

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

void Map_put(Map* m, char* key, MapValue val) {
	hash_t key_hash = prehash(key);
	int    index = key_hash % m->__size;
 	add_node(m, _new_node(key, val, key_hash), index);
	if (++m->item_count >= m->__size)
		Map_resize(&m, m->__size*2);
}

MapValue Map_get(Map* m, char* key) {
	hash_t k_hash = prehash(key);
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

void Map_delete(Map* m, char* key) {
	hash_t k_hash = prehash(key);
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
	m->item_count--;
}

static void copy_nodes(Map*, struct node*);

void Map_resize(Map** old_m, size_t size) {
	Map* new_m = create_map(size);
	new_m->item_count = (*old_m)->item_count;

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

static int node_keys(struct node*, char**, int);

void Map_keys(Map* m, char** keys) {
	int pos = 0;
	struct node* node;

	for (int i = 0; i < m->__size; i++) {
		node = m->__data[i];
		if (node != NULL)
			pos = node_keys(node, keys, pos);
	}
}

#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

static int node_height(struct node* n) {
	if (n == NULL)
		return -1;
	else
		return n->height;
}

#define MAXHEIGHT(XX, YY) MAX(node_height(XX), node_height(YY))

struct node* node_rotateleft(struct node* n) {
	struct node *head;

	head = n->_right;
	n->_right = head->_left;
	head->_left = n;

	n->height = MAX(node_height(n->_left), node_height(n->_right)) + 1;
	head->height = MAX(node_height(head->_left), node_height(head->_right)) + 1;
	return head;
}

struct node* node_rotateright(struct node* n) {
	struct node *head;

	head = n->_left;
	n->_left = head->_right;
	head->_right = n;

	n->height = MAX(node_height(n->_left), node_height(n->_right)) + 1;
	head->height = MAX(node_height(head->_left), node_height(head->_right)) + 1;
	return head;
}

static struct node* node_rotateright_double(struct node* n) {
	n->_left = node_rotateleft(n->_left);
	return node_rotateright(n);
}

static struct node* node_rotateleft_double(struct node* n) {
	n->_right = node_rotateright(n->_right);
	return node_rotateleft(n);
}

#define HEIGHT_DIFF(NODE_A, NODE_B) (node_height(NODE_A) - node_height(NODE_B))

static void init_height(struct node* n) {
	n->height = MAX(node_height(n->_left), node_height(n->_right)) + 1;
}

void insert_node(struct node** root, struct node* new) {
	if (new->_hash_val < (*root)->_hash_val) {
		/* insert left */
		if ((*root)->_left != NULL) {
			insert_node(&(*root)->_left, new);
			init_height(*root);

			// if left side is double-unbalenced... rotate right
			if (HEIGHT_DIFF((*root)->_left, (*root)->_right) == 2) {
				if (new->_hash_val < (*root)->_left->_hash_val)
					*root = node_rotateright(*root);
				else
					*root = node_rotateright_double(*root);
			}
			return;
		}
		(*root)->_left = new;
	} else if (new->_hash_val > (*root)->_hash_val) {
		/* insert right */
		if ((*root)->_right != NULL) {
			insert_node(&(*root)->_right, new);
			init_height(*root);

			// if right side is double-unbalenced... rotate left
			if (HEIGHT_DIFF((*root)->_right, (*root)->_left) == 2) {
				if (new->_hash_val > (*root)->_right->_hash_val)
					*root = node_rotateleft(*root);
				else
					*root = node_rotateleft_double(*root);
			}
			return;
		}
		(*root)->_right = new;
	}

	init_height(*root);
	return;
}

static struct node* search(struct node* root, hash_t key_hash) {
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

static struct node*
_new_node(char* key, MapValue val, hash_t key_hash) {
	struct node* n = malloc(sizeof(struct node));
	n->key = key;
	n->value = val;
	n->height = 0;
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
		insert_node(&m->__data[index], node);
	}
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

static Map* create_map(size_t size) {
	Map* m = malloc(sizeof(Map));
	m->__size = size;
	m->__data = malloc(sizeof(struct node*) * m->__size);
	for (int i = 0; i < size; i++)
		m->__data[i] = NULL;
	m->item_count = 0;
	return m;
}

static int node_keys(struct node* n, char** keys, int pos) {
	if (n == NULL) {
		return pos;
	}
	keys[pos++] = n->key;

	if (n->_left != NULL)
		pos = node_keys(n->_left, keys, pos);
	if (n->_right != NULL)
		pos = node_keys(n->_right, keys, pos);
	return pos;
}

#ifdef __cplusplus
}
#endif
