#ifndef HASHMAP_H
#define HASHMAP_H

#ifdef __cplusplus
extern "C" {
#endif

struct node; // binary tree node

typedef struct hashmap {
	struct node** __data;
	size_t        __size;
} Map;

// new_map creates a new map
Map* new_map();

// close_map frees the memory allocated by new_map
void close_map(Map*);

// put addes a void pointer at a key
void put(Map* m, char* key, void* val);

// get returns the void pointer stored at a key.
void* get(Map* m, char* key);

// delete will delete the data stored at a keys.
void delete(Map * m, char* key);
void resize_map(Map** m, size_t size);
void print_map(Map*);

#ifdef __cplusplus
}
#endif

#endif
