#include <stdio.h>
#include <stdlib.h>

#include "hashmap.h"

#pragma pack(1)

struct node
{
#ifndef TRASH_KEY
    char* key;
#endif
    mapval_t value;

    /**
     * Using unsigned char to save space
     *
     * The height of a tree is log2(n), and the max height of
     * this implementation is 255, which means that the maximum number
     * of nodes in each tree is 2^255 or about 5.79e76, more than
     * enough wiggle room seeing as though that is almost as many
     * atoms in the observable universe, I think one byte will be enough.
     */

    unsigned char height;

    struct node *right, *left;
    hash_t _hash_val;
};

/**
 * fnv hashing specification found at http://isthe.com/chongo/tech/comp/fnv/
 */
#if __x86_64__ || __ppc64__ // if compiled for 64 bit os
#define FNV_PRIME 0x100000001b3UL
#define FNV_PRIME_MUL(X) (X << 1) + (X << 4) + (X << 5) + (X << 7) + (X << 8) + (X << 40)
#define FNV_OFFSET 0xcbf29ce484222325UL
#else // for 32 bit machines
#define FNV_PRIME 16777619UL
#define FNV_PRIME_MUL(X) (X << 1) + (X << 4) + (X << 7) + (X << 8) + (X << 24)
#define FNV_OFFSET 2166136261UL
#endif

hash_t prehash(char* str)
{
    unsigned char* s = (unsigned char*)str;
    hash_t hval = FNV_OFFSET;

    while (*s)
    {
        hval += FNV_PRIME_MUL(hval);
        hval ^= (hash_t)*s++;
    }
    return hval;
}

hash_t prehash_key(struct key key)
{
    size_t i;
    unsigned char* s = (unsigned char*)key.value;
    hash_t hval = FNV_OFFSET;

    for (i = 0; i < key.length; i++)
    {
        hval += FNV_PRIME_MUL(hval);
        hval ^= (hash_t)s[i];
    }
    return hval;
}

hash_t djb2(char* str)
{
    hash_t hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;

    return hash;
}

hash_t sdbm(char* str)
{
    hash_t hash = 0;
    int c;

    while ((c = *str++))
        hash = c + (hash << 6) + (hash << 16) - hash;

    return hash;
}

hash_t rshash(char* str)
{
    hash_t a = 63689, b = 378551, hash = 0;
    int c;

    while ((c = *str++))
    {
        hash = hash * a + c;
        a = a * b;
    }
    return (hash & 0x7FFFFFFF);
}

hash_t fnv_1(char* str)
{
    unsigned char* s = (unsigned char*)str;
    hash_t hval = FNV_OFFSET;

    while (*s)
    {
        hval += FNV_PRIME_MUL(hval);
        hval ^= (hash_t)*s++;
    }
    return hval;
}

hash_t fnv_1a(char* str)
{
    unsigned char* s = (unsigned char*)str;
    hash_t hval = FNV_OFFSET;

    while (*s)
    {
        hval ^= (hash_t)*s++;
        hval += FNV_PRIME_MUL(hval);
    }
    return hval;
}

Map* create_map(size_t size)
{
    size_t i;
    Map* m = malloc(sizeof(Map));
    if (m == NULL)
    {
        perror("Error: ran out of memory allocating a Map");
        return NULL;
    }

    m->__size = size;
    m->__data = malloc(sizeof(struct node*) * m->__size);
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

Map* new_map(void)
{
    return create_map(DEFAULT_MAP_SIZE);
}

static void delete_tree(struct node*);

void map_close(Map* m)
{
    size_t i;
    for (i = 0; i < m->__size; i++)
    {
        delete_tree(m->__data[i]);
    }
    free(m->__data);
    free(m);
}

void map_free(Map*) __attribute__((alias("map_close")));

static inline void put_from_hash(Map*, char*, hash_t, mapval_t);
static inline mapval_t get_from_hash(Map* m, hash_t k_hash);

void map_put(Map* m, char* key, mapval_t val)
{
    return put_from_hash(m, key, prehash(key), val);
}

void map_key_put(Map* m, struct key key, mapval_t val)
{
    return put_from_hash(m, (char*){ '\0' }, prehash_key(key), (mapval_t)val);
}

mapval_t map_get(Map* m, char* key)
{
    return get_from_hash(m, prehash(key));
}

mapval_t map_key_get(Map* m, struct key key)
{
    return get_from_hash(m, prehash_key(key));
}

static void copy_nodes(Map*, struct node*);
static struct node* _delete_node(struct node* root, hash_t k_hash, int free_key);

void map_delete(Map* m, char* key)
{
    hash_t k_hash = prehash(key);
    size_t index = k_hash % m->__size;

    struct node* root = m->__data[index];
    if (root == NULL)
    {
        return;
    }

    m->__data[index] = _delete_node(root, k_hash, 0);
    m->item_count--;
}

int map_resize(Map** old_m, size_t size)
{
    Map* new_m = create_map(size);
    if (new_m == NULL)
        return -1;
    new_m->item_count = (*old_m)->item_count;

    struct node* tmp;
    size_t i;
    for (i = 0; i < (*old_m)->__size; i++)
    {
        tmp = (*old_m)->__data[i];
        if (tmp != NULL)
            copy_nodes(new_m, tmp);
    }
    map_close(*old_m);
    (*old_m) = new_m;
    return 0;
}

static int node_keys(struct node*, char**, int);

void map_keys(Map* m, char** keys)
{
    int pos = 0;
    struct node* node;

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

void map_clear(Map* m)
{
    size_t i;
    for (i = 0; i < m->__size; i++)
    {
        delete_tree(m->__data[i]);
        m->__data[i] = NULL;
    }
    m->item_count = 0;
}

static struct node* _new_node(char*, mapval_t, hash_t);
static void insert_node(struct node** root, struct node* new);
static void add_node(Map*, struct node*, size_t);

static inline void put_from_hash(Map* m, char* key, hash_t k_hash, mapval_t value)
{
    size_t index = k_hash % m->__size;

    struct node* node = _new_node(key, value, k_hash);
    struct node* head_node = m->__data[index];

    if (head_node == NULL)
    {
        m->__data[index] = node;
    }
    else if (head_node->_hash_val == k_hash)
    {
        node->left = head_node->left;
        node->right = head_node->right;
        free(m->__data[index]);
        m->__data[index] = node;
    }
    else
    {
        insert_node(&m->__data[index], node);
    }
    m->item_count++;
}

static struct node* search(struct node* root, hash_t key_hash);

static inline mapval_t get_from_hash(Map* m, hash_t k_hash)
{
    size_t index = k_hash % m->__size;

    struct node* root = m->__data[index];
    if (root == NULL)
        return (mapval_t)0;

    if (k_hash != root->_hash_val)
    {
        struct node* n = search(root, k_hash);
        if (n == NULL)
            return (mapval_t)0;
        return n->value;
    }
    return root->value;
}

static struct node* search(struct node* root, hash_t key_hash)
{
    while (root)
    {
        if (key_hash < root->_hash_val)
        {
            root = root->left;
        }
        else if (key_hash > root->_hash_val)
        {
            root = root->right;
        }
        else
            return root;
    }
    return NULL;
}

static struct node* node_rotateright(struct node* n);
static struct node* node_rotateleft(struct node* n);

#define balance_left_side(root, new_hash)                                                         \
    if (new_hash < (*root)->left->_hash_val)                                                      \
    {                                                                                             \
        *root = (struct node*)node_rotateright(*root);                                            \
    }                                                                                             \
    else                                                                                          \
    {                                                                                             \
        (*root)->left = (struct node*)node_rotateleft((*root)->left);                             \
        (*root) = (struct node*)node_rotateright(*root);                                          \
    }

#define balance_right_side(root, new_hash)                                                        \
    if (new_hash > (*root)->right->_hash_val)                                                     \
    {                                                                                             \
        *root = (struct node*)node_rotateleft(*root);                                             \
    }                                                                                             \
    else                                                                                          \
    {                                                                                             \
        (*root)->right = node_rotateright((*root)->right);                                        \
        *root = (struct node*)node_rotateleft(*root);                                             \
    }

#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))
#define height(N) (N == NULL ? -1 : N->height)
#define MAXHEIGHT(XX, YY) MAX(height(XX), height(YY))
#define HEIGHT_DIFF(NODE_A, NODE_B) (height(NODE_A) - height(NODE_B))
#define BALENCE(NODE) (height((NODE)->left) - height((NODE)->right))

static void insert_node(struct node** root, struct node* new)
{
    if (new->_hash_val < (*root)->_hash_val)
    {
        if ((*root)->left != NULL)
        {
            insert_node(&(*root)->left, new);

            /* if left side is double-unbalenced... rotate right */
            if (HEIGHT_DIFF((*root)->left, (*root)->right) == 2)
            {
                balance_left_side(root, new->_hash_val);
            }
        }
        else
            (*root)->left = new;
    }
    else if (new->_hash_val > (*root)->_hash_val)
    {
        if ((*root)->right != NULL)
        {
            insert_node(&(*root)->right, new);

            /* if right side is double-unbalenced... rotate left */
            if (HEIGHT_DIFF((*root)->right, (*root)->left) == 2)
            {
                balance_right_side(root, new->_hash_val);
            }
        }
        else
            (*root)->right = new;
    }
    else
    {
        new->left = (*root)->left;
        new->right = (*root)->right;
        free(*root);
        *root = new;
    }
    (*root)->height = MAX(height((*root)->left), height((*root)->right)) + 1;
}

static void delete_tree(struct node* leaf)
{
    if (leaf != NULL)
    {
        delete_tree(leaf->right);
        delete_tree(leaf->left);
        leaf->left = NULL;
        leaf->right = NULL;
        free(leaf);
    }
}

static void delete_tree_free_keys(struct node* n)
{
    if (n != NULL)
    {
        delete_tree_free_keys(n->left);
        delete_tree_free_keys(n->right);
        n->left = NULL;
        n->right = NULL;
#ifndef TRASH_KEY
        free(n->key);
#endif
        free(n);
    }
}

static struct node* _delete_node_free_key(struct node*, hash_t);

void map_delete_free_key(Map* m, char* key)
{
    hash_t k_hash = prehash(key);
    size_t index = k_hash % m->__size;

    struct node* root = m->__data[index];
    if (root == NULL)
    {
        return;
    }

    m->__data[index] = _delete_node_free_key(root, k_hash);
    m->item_count--;
}

void map_clear_free_keys(Map* m)
{
    size_t i;
    for (i = 0; i < m->__size; i++)
    {
        delete_tree_free_keys(m->__data[i]);
        m->__data[i] = NULL;
    }
    m->item_count = 0;
}

void map_close_free_keys(Map* m)
{
    size_t i;
    for (i = 0; i < m->__size; i++)
    {
        delete_tree_free_keys(m->__data[i]);
        m->__data[i] = NULL;
    }
    free(m->__data);
    free(m);
}

static struct node* _new_node(char* key, mapval_t val, hash_t key_hash)
{
    struct node* n = malloc(sizeof(struct node));
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

#ifdef __STDC__
#if (__STDC_VERSION__ >= 199901L)
#define _inline inline
#else
#define _inline
#endif
#endif

static void add_node(Map* m, struct node* node, size_t index)
{
    struct node* head_node = m->__data[index];
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
        node->left = head_node->left;
        node->right = head_node->right;

        free(m->__data[index]);
        m->__data[index] = node;
    }
    else
    {
        insert_node(&m->__data[index], node);
    }
}

static struct node* node_rotateleft(struct node* n)
{
    struct node* head;

    head = n->right;
    n->right = head->left;
    head->left = n;

    n->height = MAX(height(n->left), height(n->right)) + 1;
    head->height = MAX(height(head->left), height(head->right)) + 1;
    return head;
}

static struct node* node_rotateright(struct node* n)
{
    struct node* head;

    head = n->left;
    n->left = head->right;
    head->right = n;

    n->height = MAX(height(n->left), height(n->right)) + 1;
    head->height = MAX(height(head->left), height(head->right)) + 1;
    return head;
}

static _inline struct node* min_node(struct node* node)
{
    struct node* curr = node;

    while (curr->left != NULL)
        curr = curr->left;
    return curr;
}

static struct node* _delete_node(struct node* root, hash_t k_hash, int free_key)
{
    if (root == NULL)
        return root;

    if (k_hash < root->_hash_val)
    {
        root->left = _delete_node(root->left, k_hash, free_key);
    }
    else if (k_hash > root->_hash_val)
    {
        root->right = _delete_node(root->right, k_hash, free_key);
    }

    else if (root->_hash_val == k_hash)
    {
        if (!root->left || !root->right)
        {
            struct node* tmp;
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

static _inline struct node* _delete_node_free_key(struct node* root, hash_t k_hash)
{
    return _delete_node(root, k_hash, 1);
}

static void copy_nodes(Map* m, struct node* n)
{
    if (n->left != NULL)
        copy_nodes(m, n->left);

    if (n->right != NULL)
        copy_nodes(m, n->right);

    size_t index = n->_hash_val % m->__size;
#ifndef TRASH_KEY
    add_node(m, _new_node(n->key, n->value, n->_hash_val), index);
#else
    add_node(m, _new_node("", n->value, n->_hash_val), index);
#endif
}

static int node_keys(struct node* n, char** keys, int pos)
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