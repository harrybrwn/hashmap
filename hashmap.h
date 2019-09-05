#ifndef HASHMAP_H
#define HASHMAP_H

#include <stddef.h>
#if !defined(MapValue)
#define MapValue void *
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#pragma pack(1)

#define DEFAULT_MAP_SIZE 32

	typedef struct hashmap
	{
		struct node **__data;
		size_t __size;

		size_t item_count;
	} Map;

	/* New_Map creates a new map. Allocates the Map on the heap. */
	Map *New_Map(void);

	/* Create_Map creates a new map but initializes it with a given size */
	Map *Create_Map(size_t);

	/**
     * Map_close frees the memory allocated by new_map. Only needs to be called
     * if the Map was allocated on the heap.
     */
	void Map_close(Map *);

	/* Map_put addes a void pointer at a key. */
	void Map_put(Map *m, char *key, MapValue val);

	/* Map_get returns the void pointer stored at a key. */
	MapValue Map_get(Map *m, char *key);

	/* Map_delete will delete the data stored at a keys. */
	void Map_delete(Map *m, char *key);

	/**
	 * Map_resize resizes the map to the given size. The Map given will be copied
 	 * to a new map that has a different size.
 	 * 
 	 * returns -1 if the resize failed, 0 if all went well.
 	 */
	int Map_resize(Map **m, size_t size);

	/**
     * Map_clear will flush out all the data stored in the map.
     */
	void Map_clear(Map *);

	/**
     * Map_keys takes a Map and an array of strings and populates the array with
     * all the keys used in the Map.
     *
     * The string array given should have a length equal to the map's "item_count"
     * field.
     */
	void Map_keys(Map *m, char **keys);

	/**
     * Map_close_free_keys is the same as Map_close except it assumes that every key
     * has been allocated on the heap and frees that memory.
     */
	void Map_close_free_keys(Map *);

	/**
     * Map_delete_free_key will delete data from the hashmap assuming that the key
     * stored internally has been alllocated on the heap.
     */
	void Map_delete_free_key(Map *, char *);

	void Map_clear_free_keys(Map *);

#ifdef __cplusplus
}
#endif

#endif /* HASHMAP_H */
