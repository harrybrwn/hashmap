#include <stdio.h>
#include <stdlib.h>

#include "hashmap.h"

#pragma pack(1)

struct node
{
#ifndef TRASH_KEY
    char* key;
#endif
    MapValue value;

    /* I'm making some assumtions about the size of each tree here.
   It should be fine if the heights stay under 255. */
    unsigned char height;

    struct node *right, *left;
    hash_t _hash_val;
};

hash_t prehash(char* str)
{
    hash_t prime = 16777619;
    hash_t hash = 2166136261;

    char c;
    while ((c = *str++))
        hash = (hash ^ c) * prime;

    return hash;
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
    hash_t prime = 16777619;
    hash_t hash = 2166136261;

    int c;
    while ((c = *str++))
        hash = (hash ^ c) * prime;

    return hash;
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

static void add_node(Map*, struct node*, int);
static struct node* _new_node(char*, MapValue, hash_t);
static void copy_nodes(Map*, struct node*);
static struct node* search(struct node* root, hash_t key_hash);

void map_put(Map* m, char* key, MapValue val)
{
    hash_t key_hash = prehash(key);
    int index = key_hash % m->__size;

    add_node(m, _new_node(key, val, key_hash), index);
    m->item_count++;
}

MapValue map_get(Map* m, char* key)
{
    hash_t k_hash = prehash(key);
    int index = k_hash % m->__size;

    struct node* root = m->__data[index];
    if (root == NULL)
        return NULL;

    if (k_hash != root->_hash_val)
    {
        struct node* n = search(root, k_hash);
        if (n == NULL)
            return NULL;
        return n->value;
    }
    return root->value;
}

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

static struct node* search(struct node* root, hash_t key_hash)
{
    if (root->_hash_val == key_hash)
        return root;

    if (key_hash < root->_hash_val)
        return search(root->left, key_hash);
    else if (key_hash > root->_hash_val)
        return search(root->right, key_hash);

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
    /* insert left */
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
        /* insert right */
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
    (*root)->height = MAX(height((*root)->left), height((*root)->right)) + 1;
}

static void delete_tree(struct node* leaf)
{
    if (leaf != NULL)
    {
        delete_tree(leaf->right);
        delete_tree(leaf->left);
        free(leaf);
    }
}

static void delete_tree_free_keys(struct node* n)
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
    }
    free(m->__data);
    free(m);
}

static struct node* _new_node(char* key, MapValue val, hash_t key_hash)
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

static void add_node(Map* m, struct node* node, int index)
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

#ifdef __STDC__
#if (__STDC_VERSION__ >= 199901L)
#define _inline inline
#else
#define _inline
#endif
#endif

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

static struct node* _delete_node_free_key(struct node* root, hash_t k_hash)
{
    return _delete_node(root, k_hash, 1);
}

static void copy_nodes(Map* m, struct node* n)
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