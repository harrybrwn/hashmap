#ifndef _HASHMAP_INTERNAL_H
#define _HASHMAP_INTERNAL_H

#if !defined(MapValue)
  #define MapValue void*
#endif

#ifdef __cplusplus
extern "C" {
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

#define HEIGHT_DIFF(NODE_A, NODE_B) (height(NODE_A)-height(NODE_B))
#define BALENCE(NODE) (height((NODE)->left) - height((NODE)->right))

hash_t prehash(char*);
void delete_tree(struct node*);
struct node* search(struct node*, hash_t);


#ifdef __cplusplus
}
#endif

#endif /* _HASHMAP_INTERNAL_H */