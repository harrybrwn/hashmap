# hashmap

This is my hash table implimentation written in c89 complient C.
I am using djb2 by Dan Bernstein for the string pre-hashing function.

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

### Functions
`Map* Create_Map(size_t size)`<br>
Create_Map creates a new map but initializes it with a given size

`Map* New_Map()`<br>
New_Map creates a new map. Allocates the Map on the heap.

`void Map_close(Map* m)`<br>
Map_close frees the memory allocated by new_map. Only needs to be called if the Map was allocated on the heap.

`void Map_put(Map* m, char* key, MapValue val)`<br>
Map_put addes a void pointer at a key.

`MapValue Map_get(Map* m, char* key)`<br>
Map_get returns the void pointer stored at a key.

`void Map_delete(Map * m, char* key)`<br>
Map_delete will delete the data stored at a keys.

`void Map_resize(Map** m, size_t size)`<br>
Map_resize resizzes the map to the given size. The Map given will be copied
to a new map that has a different size.

`void Map_keys(Map* m, char** keys)`<br>
Map_keys takes a Map and an array of strings and populates the array with
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
	map = New_Map();
	Map_put(map, "working", 1);
}

int main() {
	init();

	MapValue working = Map_get(map, "working");

	if (working) {
		printf("everything is working great :) \n");
	} else {
		printf("map is not working\n");
		return 1;
	}
	return 0;
}
```
