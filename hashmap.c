#include <stdlib.h>
#include <stdio.h>
#include "hashmap.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(1)

typedef unsigned long hash_t;

struct node {
	char*    key;
	MapValue value;

	// I'm making some assumtions about the size of each tree here.
	// It should be fine if the heights stay under 255.
	unsigned char height;

	struct
	node*  right, * left;
	hash_t _hash_val;
};

static inline hash_t djb2(char* str) {
	hash_t hash = 5381;
	int    c;

	while ((c = *str++))
		hash = ((hash << 5) + hash) + c;

	return hash;
}

static inline hash_t sdbm(char *str) {
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
void delete_node(struct node**, hash_t);

Map* New_Map() {
	return create_map(DEFAULT_MAP_SIZE);
}

void Map_close(Map* m) {
	for (int i = 0; i < m->__size; i++) {
		delete_tree(m->__data[i]);
	}
	free(m->__data);
	free(m);
}

static void copy_nodes(Map*, struct node*);

void Map_put(Map* m, char* key, MapValue val) {
	hash_t key_hash = prehash(key);
	int    index = key_hash % m->__size;

	add_node(m, _new_node(key, val, key_hash), index);
	m->item_count++;
}

MapValue Map_get(Map* m, char* key) {
	hash_t k_hash = prehash(key);
	int    index = k_hash % m->__size;

	struct node* root = m->__data[index];
	if (root == NULL)
		return NULL;

	if (k_hash != root->_hash_val) {
		struct node* n = search(root, k_hash);
		if (n == NULL)
			return NULL;
		return n->value;
	}
	return root->value;
}

void Map_delete(Map* m, char* key) {
	hash_t k_hash = prehash(key);
	size_t    index = k_hash % m->__size;

	struct node* root = m->__data[index];
	if (root == NULL) {
		m->item_count--; // ok wait, why??
		return;
	}

	/**
	 * This NEEDS to be fixed!
	 * 
	 * It should just work with a single call to delete_node
	 * but i keep getting seg-faults when i do that.
	 */
	if (k_hash != root->_hash_val) {
		delete_node(&root, k_hash);
	} else {
		// if (root->left != NULL || root->right != NULL)
		// 	printf("possible mem leak at %s:%d\n", __FILE__, __LINE__);
		free(m->__data[index]);
		m->__data[index] = NULL;
	}
	m->item_count--;
}

void Map_resize(Map** old_m, size_t size) {
	Map* new_m = create_map(size);
	new_m->item_count = (*old_m)->item_count;

	struct node* tmp;
	for (int i = 0; i < (*old_m)->__size; i++) {
		tmp = (*old_m)->__data[i];
		if (tmp != NULL)
			copy_nodes(new_m, tmp);
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

static inline int height(struct node* n) {
	if (n == NULL)
		return -1;
	else
		return n->height;
}

#define MAXHEIGHT(XX, YY) MAX(height(XX), height(YY))

static struct node* node_rotateleft(struct node* n) {
	struct node *head;

	head = n->right;
	n->right = head->left;
	head->left = n;

	n->height = MAX(height(n->left), height(n->right)) + 1;
	head->height = MAX(height(head->left), height(head->right)) + 1;
	return head;
}

static struct node* node_rotateright(struct node* n) {
	struct node *head;

	head = n->left;
	n->left = head->right;
	head->right = n;

	n->height = MAX(height(n->left), height(n->right)) + 1;
	head->height = MAX(height(head->left), height(head->right)) + 1;
	return head;
}

static inline void
balance_left_side(struct node** root, hash_t new_hash) {
	if (new_hash < (*root)->left->_hash_val) {
		// single rotate right
		*root = node_rotateright(*root);
	} else {
		// double rotate right
		(*root)->left = node_rotateleft((*root)->left);
		(*root) = node_rotateright(*root);
	}
}

static inline void
balance_right_side(struct node** root, hash_t new_hash) {
	if (new_hash > (*root)->right->_hash_val) {
		// single rotate left
		*root = node_rotateleft(*root);
	} else {
		// double rotate left
		(*root)->right = node_rotateright((*root)->right);
		*root = node_rotateleft(*root);
	}
}

#define HEIGHT_DIFF(NODE_A, NODE_B) (height(NODE_A)-height(NODE_B))

// insert_node is exposed as a non-static function for testing purposes only.
void insert_node(struct node** root, struct node* new) {
	if (new->_hash_val < (*root)->_hash_val) {
		/* insert left */
		if ((*root)->left != NULL) {
			insert_node(&(*root)->left, new);

			// if left side is double-unbalenced... rotate right
			if (HEIGHT_DIFF((*root)->left, (*root)->right) == 2)
				balance_left_side(root, new->_hash_val);
		} else
			(*root)->left = new;
	} else if (new->_hash_val > (*root)->_hash_val) {
		/* insert right */
		if ((*root)->right != NULL) {
			insert_node(&(*root)->right, new);

			// if right side is double-unbalenced... rotate left
			if (HEIGHT_DIFF((*root)->right, (*root)->left) == 2)
				balance_right_side(root, new->_hash_val);
		} else
			(*root)->right = new;
	}
	(*root)->height = MAX(height((*root)->left), height((*root)->right)) + 1;
}

static struct node* search(struct node* root, hash_t key_hash) {
	if (root->_hash_val == key_hash)
		return root;

	if (key_hash < root->_hash_val)
		return search(root->left, key_hash);
	else if (key_hash > root->_hash_val)
		return search(root->right, key_hash);
	return NULL;
}

static void delete_tree(struct node* leaf) {
	if (leaf != NULL) {
		delete_tree(leaf->right);
		delete_tree(leaf->left);
		free(leaf);
	}
}

static struct node*
_new_node(char* key, MapValue val, hash_t key_hash) {
	struct node* n = malloc(sizeof(struct node));
	if (n == NULL) {
		perror("Error: out of memory allocating nodes");
		return NULL;
	}
	n->key = key;
	n->value = val;
	n->height = 0;
	n->left = NULL;
	n->right = NULL;
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

#define POP_MINMAX(SIDE_A, SIDE_B)                    \
	while (1)                                         \
	{                                                 \
		if (tmp->SIDE) {                              \
			if (!tmp->SIDE->SIDE) {                   \
				struct node* min_max = tmp->SIDE;     \
				min_max->SIDE_B = tmp->SIDE_A->SIDE_B;\
				tmp->SIDE = NULL;                     \
				return min_max;                       \
			}                                         \
			else {                                    \
				tmp = tmp->SIDE;                      \
			}                                         \
		}                                             \
		else {                                        \
			break;                                    \
		}                                             \
	}

struct node* pop_min(struct node** node) {
	struct node* tmp = *node;
	if (!tmp->right && !tmp->left)
	{
		*node = NULL;
		return tmp;
	}

	while (1)
	{
		if (tmp->left) {
			if (!tmp->left->left) {
				struct node* min = tmp->left;
				min->right = tmp->left->right;
				tmp->left = min->right;
				return min;
			}
			else {
				tmp = tmp->left;
			}
		}
		else {
			break;
		}
	}
	return tmp;
}

/**
 * pop max finds the max value in the tree, pops off the node and returns it.
 */
struct node* pop_max(struct node** node) {
	struct node* tmp = *node;
	if (!tmp->right && !tmp->left)
	{
		*node = NULL;
		return tmp;
	}

	while (1)
	{
		if (tmp->right) {
			if (!tmp->right->right) {
				struct node* max = tmp->right;
				max->left = tmp->right->left;
				tmp->right = max->left;
				return max;
			}
			else {
				tmp = tmp->right;
			}
		}
		else {
			break;
		}
	}
	return tmp;
}

static void delete_right(struct node* node)
{
	/**
	 * node->right is the node that is going to be deleted.
	 * 
	 * Im calling delete_right on the parent so that i don't have to start
	 * storing the parent of each child in every node.
	 */
	if (!node->right->left)
	{
		struct node* tmp = node->right->right;
		free(node->right);
		node->right = tmp;
	}
	else if (!node->right->right)
	{
		struct node* tmp = node->right->left;
		free(node->right);
		node->right = tmp;
	}
	else
	{
		if (node->right->left->height >= node->right->right->height)
		{
			struct node* max = pop_max(&node->right->left);
			if (node->right->left != max)
			{
				max->left = node->right->left;
			}
			max->right = node->right->right;
			free(node->right);
			node->right = max;
		}
		else
		{
			struct node* min = pop_min(&node->right->right);
			if (node->right->right != min)
			{
				min->right = node->right->right;
			}
			min->left = node->right->left;
			free(node->right);
			node->right = min;
		}

		// recalculate heights
	}
}

static void delete_left(struct node* node)
{
	/**
	 * node->left is the node that is going to be deleted.
	 * 
	 * Im calling delete_left on the parent so that i don't have to start
	 * storing the parent of each child in every node.
	 */
	if (!node->left->left)
	{
		struct node* tmp = node->left->right;
		free(node->left);
		node->left = tmp;
	}
	else if (!node->left->right)
	{
		struct node* tmp = node->left->left;
		free(node->left);
		node->left = tmp;
	}
	else
	{
		if (node->left->left->height >= node->left->right->height)
		{
			struct node* max = pop_max(&node->left->left);
			if (node->left->left != max)
			{
				max->left = node->left->left;
			}
			max->right = node->left->right;
			free(node->left);
			node->left = max;
		}
		else
		{
			struct node* min = pop_min(&node->left->right);
			if (node->left->right != min)
			{
				min->right = node->left->right;
			}
			min->left = node->left->left;
			free(node->left);
			node->left = min;
		}
	}

	// recalculate heights
}

#include <assert.h>

void delete_node(struct node** leaf, hash_t key_hash) {
	if ((*leaf)->_hash_val == key_hash) {
		if ((*leaf)->left)
		{
			struct node* max = pop_max(&(*leaf)->left);

			if (max != (*leaf)->left)
				max->left = (*leaf)->left;
			max->right = (*leaf)->right;
			free(*leaf);
			*leaf = max;
		}
		else if ((*leaf)->right)
		{
			struct node* tmp = (*leaf)->right;
			free(*leaf);
			*leaf = tmp;
		}
		else // this is the last node in the tree.
		{
			free(*leaf);
			*leaf = NULL;
		}
		return;
	}

	if (key_hash < (*leaf)->_hash_val) {
		// if (!(*leaf)->left)
		// {
		// 	printf("wat??\n");
		// 	return;
		// }
		if (key_hash == (*leaf)->left->_hash_val) {
			delete_left(*leaf);
		} else {
			return delete_node(&(*leaf)->left, key_hash);
		}
	} else if (key_hash > (*leaf)->_hash_val) {
		// if (!(*leaf)->right)
		// {
		// 	printf("wat??\n");
		// 	return;
		// }
		if (key_hash == (*leaf)->right->_hash_val) {
			return delete_right(*leaf);
		} else {
			return delete_node(&(*leaf)->right, key_hash);
		}
	}
}

static void copy_nodes(Map* m, struct node* n) {
	if (n->left != NULL)
		copy_nodes(m, n->left);

	if (n->right != NULL)
		copy_nodes(m, n->right);

	int index = n->_hash_val % m->__size;
	add_node(m, _new_node(n->key, n->value, n->_hash_val), index);
}

static Map* create_map(size_t size) {
	Map* m = malloc(sizeof(Map));
	if (m == NULL) {
		perror("Error: ran out of memory allocating a Map");
		return NULL;
	}
	m->__size = size;
	m->__data = malloc(sizeof(struct node*) * m->__size);
	if (m->__data == NULL) {
		perror("Error: ran out of memory allocating a node array");
		return m;
	}
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

	if (n->left != NULL)
		pos = node_keys(n->left, keys, pos);
	if (n->right != NULL)
		pos = node_keys(n->right, keys, pos);
	return pos;
}

#ifdef __cplusplus
}
#endif
