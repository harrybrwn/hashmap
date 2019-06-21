# hashmap

This is my hash table implimentation.
I am using 'djb2' by Dan Bernstein for the string pre-hashing function.

To handle hash collitions, I am using a binary search tree.


# Setup
The datatype that is stored in the hash table is defined by the MapValue macro. You can define the MapValue macro to be any other type before you include hashmap.h and the map will store that type.
