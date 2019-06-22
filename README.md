# hashmap

This is my hash table implimentation.
I am using double hashing with djb2 by Dan Bernstein and sdbm for the string pre-hashing function.

To handle hash collitions, I am using a binary search tree.

## Docs
### Structs
`Map` is the main hashmap object.
```c
typedef struct hashmap {
	// private fields

	size_t item_count;
} Map;
```

### Macros
`MapValue` is a macro that is meant to look like a datatype because it represents
the datatype stored in the hashmap. If `MapValue` is defined before hashmap.h is
included then the hashmap will use that type. If it is not defined by a user, the
default is `void*`.

### Functions
New_Map creates a new map. Allocates the Map on the heap.<br>
`Map* New_Map()`

Map_close frees the memory allocated by new_map. Only needs to be called if the Map was allocated on the heap.<br>
`void Map_close(Map*)`

Map_put addes a void pointer at a key.<br>
`void Map_put(Map* m, char* key, MapValue val)`

Map_get returns the void pointer stored at a key.<br>
`MapValue Map_get(Map* m, char* key)`

Map_delete will delete the data stored at a keys.<br>
`void Map_delete(Map * m, char* key)`

Map_resize resizzes the map to the given size. The Map given will be copied
to a new map that has a different size.<br>
`void Map_resize(Map** m, size_t size)`

Map_keys takes a Map and an array of strings and populates the array with
all the keys used in the Map.
The string array given should have a length equal to the map's "item_count"
field.<br>
`void Map_keys(Map*, char**)`
