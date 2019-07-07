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
static struct node* _delete_node(struct node* root, hash_t k_hash);

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

	m->__data[index] = _delete_node(root, k_hash);
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
#define BALENCE(NODE) (height((NODE)->left) - height((NODE)->right))

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
	/**
	 *  If the node at the index is empty, put the node there. If there is a
	 *  node at that index but the raw hash value is the same then the key was
	 *  the same and we are going to free the old node and remplace it. If the
	 *  raw hash is different then we have a hash collition and we are inserting
	 *  a node into the binary tree rooted at the 'head_node'
	 */
	if (head_node == NULL) {
		m->__data[index] = node;
	} else if (head_node->_hash_val == node->_hash_val) {
		/**
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

/**
 * SA (side A) is the primary node if it is left then the loop
 * will get the furthest left node.
 *
 * SB (side B) should be the opposite side.
 *
 * mm is short for min/max.
 */
#define POP_MINMAX_LOOP(SA, SB)           \
	if (!tmp->right && !tmp->left)        \
	{                                     \
	        *node = NULL;                 \
	        return tmp;                   \
	}                                     \
	while (1)                             \
	{                                     \
		if (tmp->SA) {                    \
			if (!tmp->SA->SA) {           \
				struct node* mm = tmp->SA;\
				mm->SB = tmp->SA->SB;     \
				tmp->SA = mm->SB;         \
				return mm;                \
    		}                             \
			else {                        \
        		tmp = tmp->SA;            \
        	}                             \
    	}                                 \
		else {                            \
    		break;                        \
    	}                                 \
	}

struct node* pop_min(struct node** node) {
       struct node* tmp = *node;
       POP_MINMAX_LOOP(left, right);
       return tmp;
}

struct node* pop_max(struct node** node) {
       struct node* tmp = *node;
       POP_MINMAX_LOOP(right, left);
       return tmp;
}

static struct node* min_node(struct node* node) 
{ 
    struct node* curr = node; 
  
    while (curr->left != NULL) 
        curr = curr->left;   
    return curr; 
}

static struct node* _delete_node(struct node* root, hash_t k_hash)
{
    if (root == NULL)
        return root;

    if (k_hash < root->_hash_val)
	{
        root->left = _delete_node(root->left, k_hash);
	}
    else if(k_hash > root->_hash_val)
	{
        root->right = _delete_node(root->right, k_hash);
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
            free(tmp);
        }
        else // node has two children
        {
            struct node* min = min_node(root->right);
            root->_hash_val = min->_hash_val;
			root->key = min->key;
			root->value = min->value;
            root->right = _delete_node(root->right, min->_hash_val);
        }
	}
	if (root == NULL)
      return root;

    root->height = 1 + MAXHEIGHT(root->left, root->right);
	int h_diff = HEIGHT_DIFF(root->left, root->right);

    if (h_diff > 1 && BALENCE(root->left) >= 0)
	{
		return node_rotateright(root);
	}
	else if (h_diff > 1 && BALENCE(root->left) < 0)
    {
		root->left = node_rotateleft(root->left);
		return node_rotateright(root);
	}
    else if (h_diff < -1 && BALENCE(root->right) <= 0)
	{
		return node_rotateleft(root);
	}
	else if (h_diff < -1 && BALENCE(root->right) > 0)
    {
		root->right = node_rotateright(root->right);
		return node_rotateleft(root);
    }

    return root;
}

/**
 * delete_node is a convienience function for testing puposes only.
 */
void delete_node(struct node** root, hash_t k_hash)
{
	*root = _delete_node(*root, k_hash);
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
