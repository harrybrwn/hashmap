#ifndef HASHMAP_H
#define HASHMAP_H

#ifndef DEFAULT_MAP_TYPE
  #define DEFAULT_MAP_TYPE void*
#endif

#if !defined(MapValue)
  #define MapValue DEFAULT_MAP_TYPE
#endif

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
void put(Map* m, char* key, MapValue val);

// get returns the void pointer stored at a key.
MapValue get(Map* m, char* key);

// delete will delete the data stored at a keys.
void delete(Map * m, char* key);

// resize_map changes the map to the given size
void resize_map(Map** m, size_t size);

void print_map(Map*);

#ifdef __cplusplus
}
#endif

#endif /* HASHMAP_H */
