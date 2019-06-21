#ifndef HASHMAP_H
#define HASHMAP_H

#define DEFAULT_MAP_TYPE void*

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

// New_Map creates a new map
Map* New_Map();

// Map_close frees the memory allocated by new_map
void Map_close(Map*);

// put addes a void pointer at a key
void put(Map* m, char* key, MapValue val);

// get returns the void pointer stored at a key.
MapValue get(Map* m, char* key);

// delete will delete the data stored at a keys.
void delete(Map * m, char* key);

// Map_resize changes the map to the given size
void Map_resize(Map** m, size_t size);

void print_map(Map*);

#ifdef __cplusplus
}
#endif

#endif /* HASHMAP_H */
