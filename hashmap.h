#ifndef HASHMAP_H
#define HASHMAP_H

#include <stddef.h>
#if !defined(MapValue)
#define MapValue void*
#endif

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(1)

#define DEFAULT_MAP_SIZE 31

typedef size_t hash_t;
typedef MapValue mapval_t;

typedef struct hashmap
{
    struct node** __data;
    size_t __size;

    size_t item_count;
} Map;

/**
 * The key struct is used for when you need to use a key that is not a string.
 */
struct key {
    void* value;
    size_t length;
};

/* new_map creates a new map. Allocates the Map on the heap. */
Map* new_map(void);

/* create_map creates a new map but initializes it with a given size */
Map* create_map(size_t);

/**
 * map_close frees the memory allocated by new_map. Only needs to be called
 * if the Map was allocated on the heap.
 */
void map_close(Map*);

/* map_put addes a void pointer at a key. */
void map_put(Map* m, char* key, mapval_t val);

/* map_get returns the void pointer stored at a key. */
mapval_t map_get(Map* m, char* key);

/* map_delete will delete the data stored at a keys. */
void map_delete(Map* m, char* key);

/**
 * map_resize resizes the map to the given size. The Map given will be copied
 * to a new map that has a different size.
 *
 * returns -1 if the resize failed, 0 if all went well.
 */
int map_resize(Map** m, size_t size);

/**
 * map_clear will flush out all the data stored in the map.
 */
void map_clear(Map*);

/**
 * map_keys takes a Map and an array of strings and populates the array with
 * all the keys used in the Map.
 *
 * The string array given should have a length equal to the map's "item_count"
 * field.
 */
void map_keys(Map* m, char** keys);

/**
 * map_close_free_keys is the same as map_close except it assumes that every key
 * has been allocated on the heap and frees that memory.
 */
void map_close_free_keys(Map*);

/**
 * map_delete_free_key will delete data from the hashmap assuming that the key
 * stored internally has been alllocated on the heap.
 */
void map_delete_free_key(Map*, char*);

void map_clear_free_keys(Map*);

#ifdef __cplusplus
}
#endif

#endif /* HASHMAP_H */
