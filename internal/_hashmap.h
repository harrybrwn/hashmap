#ifndef _HASHMAP_INTERNAL_H
#define _HASHMAP_INTERNAL_H

#if !defined(MapValue)
  #define MapValue void*
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __STDC__
# define C_89
# ifdef __STDC_VERSION__
#  define C_90
#  if (__STDC_VERSION__ >= 199409L)
#   define C_94
#  endif
#  if (__STDC_VERSION__ >= 199901L)
#   define C_99
#  endif
#  if (__STDC_VERSION__ >= 201112L)
#   define C_11
#  endif
#  if (__STDC_VERSION__ >= 201710L)
#   define C_18
#  endif
# endif
#endif

#ifdef C_99
# define _inline inline
#else
# define _inline
#endif


#pragma pack(1)

typedef unsigned long hash_t;

struct node {
#ifndef TRASH_KEY
	char*    key;
#endif
	MapValue value;

	/* I'm making some assumtions about the size of each tree here.
	   It should be fine if the heights stay under 255. */
	unsigned char height;

	struct
	node*  right, * left;
	hash_t _hash_val;
};


#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))
#define height(N) (N == NULL ? -1 : N->height)
#define MAXHEIGHT(XX, YY) MAX(height(XX), height(YY))
#define HEIGHT_DIFF(NODE_A, NODE_B) (height(NODE_A)-height(NODE_B))
#define BALENCE(NODE) (height((NODE)->left) - height((NODE)->right))

#define balance_left_side(root, new_hash)              \
	if (new_hash < (*root)->left->_hash_val) {         \
		*root = node_rotateright(*root);               \
	} else {                                           \
		(*root)->left = node_rotateleft((*root)->left);\
		(*root) = node_rotateright(*root);             \
	}                                                  \

#define balance_right_side(root, new_hash)                \
	if (new_hash > (*root)->right->_hash_val) {           \
		*root = node_rotateleft(*root);                   \
	} else {											  \
		(*root)->right = node_rotateright((*root)->right);\
		*root = node_rotateleft(*root);                   \
	}													  \


hash_t prehash(char*);
void delete_tree(struct node*);
void insert_node(struct node**, struct node*);
struct node* node_rotateleft(struct node*);
struct node* node_rotateright(struct node*);
struct node* search(struct node*, hash_t);
struct node* _delete_node(struct node*, hash_t, int);

hash_t djb2(char* str);
hash_t sdbm(char *str);
hash_t fnv_1(char *str);
hash_t rshash(char *str);


#ifdef __cplusplus
}
#endif

#endif /* _HASHMAP_INTERNAL_H */