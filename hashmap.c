#include <stdio.h>
#include <stdlib.h>

#include "hashmap.h"
#include "internal/_hashmap.h"

Map *Create_Map(size_t size)
{
	size_t i;
	Map *m = malloc(sizeof(Map));
	if (m == NULL)
	{
		perror("Error: ran out of memory allocating a Map");
		return NULL;
	}

	m->__size = size;
	m->__data = malloc(sizeof(struct node *) * m->__size);
	if (m->__data == NULL)
	{
		perror("Error: ran out of memory allocating a node array");
		return NULL;
	}

	for (i = 0; i < size; i++)
		m->__data[i] = NULL;
	m->item_count = 0;
	return m;
}

Map *New_Map(void) { return Create_Map(DEFAULT_MAP_SIZE); }

void Map_close(Map *m)
{
	size_t i;
	for (i = 0; i < m->__size; i++)
	{
		delete_tree(m->__data[i]);
	}
	free(m->__data);
	free(m);
}

static void add_node(Map *, struct node *, int);
static struct node *_new_node(char *, MapValue, hash_t);
static void copy_nodes(Map *, struct node *);

void Map_put(Map *m, char *key, MapValue val)
{
	hash_t key_hash = prehash(key);
	int index = key_hash % m->__size;

	add_node(m, _new_node(key, val, key_hash), index);
	m->item_count++;
}

MapValue Map_get(Map *m, char *key)
{
	hash_t k_hash = prehash(key);
	int index = k_hash % m->__size;

	struct node *root = m->__data[index];
	if (root == NULL)
		return NULL;

	if (k_hash != root->_hash_val)
	{
		struct node *n = search(root, k_hash);
		if (n == NULL)
			return NULL;
		return n->value;
	}
	return root->value;
}

void Map_delete(Map *m, char *key)
{
	hash_t k_hash = prehash(key);
	size_t index = k_hash % m->__size;

	struct node *root = m->__data[index];
	if (root == NULL)
	{
		return;
	}

	m->__data[index] = _delete_node(root, k_hash, 0);
	m->item_count--;
}

int Map_resize(Map **old_m, size_t size)
{
	Map *new_m = Create_Map(size);
	if (new_m == NULL)
		return -1;
	new_m->item_count = (*old_m)->item_count;

	struct node *tmp;
	size_t i;
	for (i = 0; i < (*old_m)->__size; i++)
	{
		tmp = (*old_m)->__data[i];
		if (tmp != NULL)
			copy_nodes(new_m, tmp);
	}
	Map_close(*old_m);
	(*old_m) = new_m;
	return 0;
}

static int node_keys(struct node *, char **, int);

void Map_keys(Map *m, char **keys)
{
	int pos = 0;
	struct node *node;

	size_t i;
	for (i = 0; i < m->__size; i++)
	{
		node = m->__data[i];
		if (node != NULL)
		{
			pos = node_keys(node, keys, pos);
		}
	}
}

void Map_clear(Map *m)
{
	size_t i;
	for (i = 0; i < m->__size; i++)
	{
		delete_tree(m->__data[i]);
		m->__data[i] = NULL;
	}
	m->item_count = 0;
}

static void delete_tree_free_keys(struct node *n)
{
	if (n != NULL)
	{
		delete_tree_free_keys(n->left);
		delete_tree_free_keys(n->right);
#ifndef TRASH_KEY
		free(n->key);
#endif
		free(n);
	}
}

static struct node *_delete_node_free_key(struct node *, hash_t);

void Map_delete_free_key(Map *m, char *key)
{
	hash_t k_hash = prehash(key);
	size_t index = k_hash % m->__size;

	struct node *root = m->__data[index];
	if (root == NULL)
	{
		return;
	}

	m->__data[index] = _delete_node_free_key(root, k_hash);
	m->item_count--;
}

void Map_clear_free_keys(Map *m)
{
	size_t i;
	for (i = 0; i < m->__size; i++)
	{
		delete_tree_free_keys(m->__data[i]);
		m->__data[i] = NULL;
	}
	m->item_count = 0;
}

void Map_close_free_keys(Map *m)
{
	size_t i;
	for (i = 0; i < m->__size; i++)
	{
		delete_tree_free_keys(m->__data[i]);
	}
	free(m->__data);
	free(m);
}

static struct node *_new_node(char *key, MapValue val, hash_t key_hash)
{
	struct node *n = malloc(sizeof(struct node));
	if (n == NULL)
	{
		perror("Error: out of memory allocating nodes");
		return NULL;
	}
#ifndef TRASH_KEY
	n->key = key;
#endif
	n->value = val;
	n->height = 0;
	n->left = NULL;
	n->right = NULL;
	n->_hash_val = key_hash;
	return n;
}

static void add_node(Map *m, struct node *node, int index)
{
	struct node *head_node = m->__data[index];
	/**
   *  If the node at the index is empty, put the node there. If there is a
   *  node at that index but the raw hash value is the same then the key was
   *  the same and we are going to free the old node and remplace it. If the
   *  raw hash is different then we have a hash collition and we are inserting
   *  a node into the binary tree rooted at the 'head_node'
   */
	if (head_node == NULL)
	{
		m->__data[index] = node;
	}
	else if (head_node->_hash_val == node->_hash_val)
	{
		/**
     *  Getting two hash values that are the same is extremly unlikly given
     *  different inputs. This is different that getting a hash collition
     *  which is after you take the modulus of the hash.
     *  If the hash value of the two is the same then we are going to free
     *  the memory from the old node and replace it with the new one.
     */
		free(m->__data[index]);
		m->__data[index] = node;
	}
	else
	{
		insert_node(&m->__data[index], node);
	}
}

#ifdef HASHMAP_TESTING
/**
 * SA (side A) is the primary node if it is left then the loop
 * will get the furthest left node.
 *
 * SB (side B) should be the opposite side.
 *
 * mm is short for min/max.
 */
#define POP_MINMAX_LOOP(SA, SB)            \
	if (!tmp->right && !tmp->left)         \
	{                                      \
		*node = NULL;                      \
		return tmp;                        \
	}                                      \
	while (1)                              \
	{                                      \
		if (tmp->SA)                       \
		{                                  \
			if (!tmp->SA->SA)              \
			{                              \
				struct node *mm = tmp->SA; \
				mm->SB = tmp->SA->SB;      \
				tmp->SA = mm->SB;          \
				return mm;                 \
			}                              \
			else                           \
			{                              \
				tmp = tmp->SA;             \
			}                              \
		}                                  \
		else                               \
		{                                  \
			break;                         \
		}                                  \
	}

struct node *pop_min(struct node **node)
{
	struct node *tmp = *node;
	POP_MINMAX_LOOP(left, right);
	return tmp;
}

struct node *pop_max(struct node **node)
{
	struct node *tmp = *node;
	POP_MINMAX_LOOP(right, left);
	return tmp;
}
#endif

static struct node *_delete_node_free_key(struct node *root, hash_t k_hash)
{
	return _delete_node(root, k_hash, 1);
}

#ifdef HASHMAP_TESTING
/**
 * delete_node is a convienience function for testing puposes only.
 */
void delete_node(struct node **root, hash_t k_hash)
{
	*root = _delete_node(*root, k_hash, 0);
}

#endif

static void copy_nodes(Map *m, struct node *n)
{
	if (n->left != NULL)
		copy_nodes(m, n->left);

	if (n->right != NULL)
		copy_nodes(m, n->right);

	int index = n->_hash_val % m->__size;
#ifndef TRASH_KEY
	add_node(m, _new_node(n->key, n->value, n->_hash_val), index);
#else
	add_node(m, _new_node("", n->value, n->_hash_val), index);
#endif
}

static int node_keys(struct node *n, char **keys, int pos)
{
	if (n == NULL)
	{
		return pos;
	}

#ifndef TRASH_KEY
	keys[pos++] = n->key;
#endif

	if (n->left != NULL)
		pos = node_keys(n->left, keys, pos);
	if (n->right != NULL)
		pos = node_keys(n->right, keys, pos);
	return pos;
}