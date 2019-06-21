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

	size_t __size;
	size_t item_count;
} Map;

// New_Map creates a new map
Map* New_Map();

// Map_close frees the memory allocated by new_map
void Map_close(Map*);

// Map_put addes a void pointer at a key
void Map_put(Map* m, char* key, MapValue val);

// Map_get returns the void pointer stored at a key.
MapValue Map_get(Map* m, char* key);

// Map_delete will delete the data stored at a keys.
void Map_delete(Map * m, char* key);

// Map_resize changes the map to the given size
void Map_resize(Map** m, size_t size);

// Map_keys takes a Map and an array of strings and populates the array with
// all the keys used in the Map.
//
// The string array given should have a length equal to the map's "item_count"
// field.
void Map_keys(Map*, char**);

#ifdef __cplusplus
}
#endif

#endif /* HASHMAP_H */
