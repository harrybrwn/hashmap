#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "hashmap.h"

#define ARR_CMP(ARR1, LEN1, ARR2, LEN2) \
    for (int i = 0; i < LEN1; i++) { \
        for (int k = 0; k < LEN2; k++) { \
            if (strcmp(ARR1[i], ARR2[k]) == 0) \
                goto Same_Arr_val; \
        } \
        assert(0); \
    Same_Arr_val:; \
    }

char *randstring(size_t length) {
    static char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!";
    char *str = NULL;
    if (length) {
        str = malloc(sizeof(char) * (length + 1));
        if (str) {
            for (int n = 0;n < length;n++) {
                int key = rand() % (int)(sizeof(charset) - 1);
                str[n] = charset[key];
            }
            str[length] = '\0';
        }
    }
    return str;
}

typedef unsigned long hash_t;
hash_t prehash(char* str);

static char** rand_keys(int len) {
    char** keys = malloc(sizeof(char*)*len);
    for (int i = 0; i < len; i++) {
        keys[i] = randstring(10);
    }
    return keys;
}

static char** collition_keys(size_t str_len,
                             size_t mapsize,
                             hash_t (*hash_fn)(char*),
                             int len)
{
    int k = 0;
    char** arr = malloc(sizeof(char*)*len);
    arr[k++] = randstring(str_len);
    hash_t h, base_hash = hash_fn(arr[0]);
    char* str;

    while (k != len) {
        str = randstring(str_len);
        h = hash_fn(str);
        if ((h % mapsize) == (base_hash % mapsize))
            arr[k++] = str;
        else
            free(str);
    }
    return arr;
}

void free_string_arr(char** arr, int len) {
    for (int i = 0; i < len; i++)
        free(arr[i]);
    free(arr);
}

void test_prehash() {
    hash_t a = prehash("abc");
    hash_t b = prehash("cba");
    hash_t c = prehash("bca");

    assert(a != b);
    assert(c != a);
    assert(c != b);
}

void TestMap() {
	Map* map = New_Map();

    char* val = "this is a value";
	Map_put(map, "key", val);
    assert(strcmp(val, (char*)Map_get(map, "key")) == 0);

    val = "updated value stored in the map";
	Map_put(map, "key", val);
    assert(strcmp(val, (char*)Map_get(map, "key")) == 0);

	Map_close(map);
}

struct node {
	char*    key;
	MapValue value;

	struct
	node*  _right, * _left;
	hash_t _hash_val;
};

void test_collitions() {
	Map* m = New_Map();
	int n = 20;

    // these keys all collide is a hash table of length 'm->__size' using 'prehash'
    char** keys = collition_keys(6, m->__size, prehash, n);

	int x[n];
	for (int i = 0; i < n; i++) {
		x[i] = i;
		Map_put(m, keys[i], &x[i]);
	}

    int nonNullKeys = 0;
    for (int i = 0; i < n; i++) {
        assert(i == *(int*)Map_get(m, keys[i]));
        if (m->__data[i] != NULL)
            nonNullKeys++;
    }
    assert(nonNullKeys == 1);

    free_string_arr(keys, n);
	Map_close(m);
}

void test_resize() {
    Map* m = New_Map();
    int n = 28;
    char** keys = rand_keys(n);

    int x[n];
    for (int i = 0; i < n; i++) {
        x[i] = i;
        Map_put(m, keys[i], &x[i]);
    }
    Map_resize(&m, n + 1);
    assert(m->__size == n + 1);

    char* mapkeys[m->item_count];
    Map_keys(m, mapkeys);
    ARR_CMP(mapkeys, m->item_count, keys, n);

    Map_resize(&m, 3);
    for (int i = 0; i < n; i++)
        assert(i == *(int*)Map_get(m, keys[i]));
    assert(m->__size == 3);
    free_string_arr(keys, n);
    Map_close(m);
}

int main() {
    TestMap();
	test_collitions();
    test_prehash();
    test_resize();

    printf("OK %s\n", __FILE__);
}
