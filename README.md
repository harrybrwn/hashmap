# hashmap

This is my hash table implimentation written in c89 complient C.

To handle hash collitions, I am using auto balancing binary search trees (avl trees).

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

By default, the internal node struct stores the key, if `TRASH_KEY` is defines then
the key is not stored. This gives the Map a small performance boost however
it makes some of the features like map_keys and map_resize unusable.

### Functions
`Map* create_map(size_t size)`<br>
create_map creates a new map but initializes it with a given size

`Map* new_map()`<br>
new_map creates a new map. Allocates the Map on the heap.

`void map_close(Map* m)`<br>
map_close frees the memory allocated by new_map. Only needs to be called if the Map was allocated on the heap.

`void map_put(Map* m, char* key, MapValue val)`<br>
map_put addes a void pointer at a key.

`MapValue map_get(Map* m, char* key)`<br>
map_get returns the void pointer stored at a key.

`void map_delete(Map * m, char* key)`<br>
map_delete will delete the data stored at a keys.

`void map_resize(Map** m, size_t size)`<br>
map_resize resizzes the map to the given size. The Map given will be copied
to a new map that has a different size.

`void map_keys(Map* m, char** keys)`<br>
map_keys takes a Map and an array of strings and populates the array with
all the keys used in the Map.
The string array given should have a length equal to the map's "item_count"
field.

### Example
```c
#include <stdio.h>
#include <stdbool.h>

#define MapValue bool
#include <hashmap.h>

static Map* map = NULL;

void init() {
	map = new_map();
	map_put(map, "working", 1);
}

int main() {
	init();

	MapValue working = map_get(map, "working");

	if (working) {
		printf("everything is working great :) \n");
	} else {
		printf("map is not working\n");
		return 1;
	}
	return 0;
}
```
