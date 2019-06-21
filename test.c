#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "hashmap.h"

char *randstring(size_t length) {
    static char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!";
    char *str = NULL;
    if (length) {
        str = malloc(sizeof(char) * (length +1));
        if (str) {
            for (int n = 0;n < length;n++) {
                int key = rand() % (int)(sizeof(charset) -1);
                str[n] = charset[key];
            }
            str[length] = '\0';
        }
    }
    return str;
}


typedef unsigned long hash_t;
hash_t sdbm(char *str);
hash_t prehash(char* str);

void test_prehash() {
    hash_t a = prehash("abc");
    hash_t b = prehash("cba");
    hash_t c = prehash("bca");

    assert(a != b);
    assert(c != a);
    assert(c != b);
}

void new_val() {
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
    char* key;
	void* value;

	struct node* _right, * _left;
	unsigned long _hash_val;
};

void test_collitions() {
	Map* m = New_Map();
	int n = 16;

    // these keys all collide when the map has a length of 32 useing djb2 (the default length)
	char* keys[16] = {
        "7GKu#m'?Hn", "ya?F9JS6HW", "xoO96tWdi-", "c2dvl0gUvm",
		"AeZB4t",     "jB75,f",     "L7YD6T",     "EY?nDc8",
        "a4ZITOo",    "TUibNyo",    "rZJnN5C",    "7i0o4KL",
        "di-SRs8",    "?StNEiH",    "1'7xPkH",    "#vKWM11"
    };

	int x[n];
	for (int i = 0; i < n; i++) {
		x[i] = i;
		Map_put(m, keys[i], &x[i]);
	}

    for (int i = 0; i < m->__size; i++) {
        if (m->__data[i] == NULL) {
            printf("null, ");
        } else {
            printf("%s, ", m->__data[i]->key);
        }
    }

    int nonNullKeys = 0;
    for (int i = 0; i < n; i++) {
        assert(i == *(int*)Map_get(m, keys[i]));
        if (m->__data[i] != NULL)
            nonNullKeys++;
    }
    // this test is invalid because of new double hashing
    // assert(nonNullKeys == 1);

    Map_resize(&m, 3);
    for (int i = 0; i < n; i++)
        assert(i == *(int*)Map_get(m, keys[i]));
    assert(m->__size == 3);

    char* mkeys[m->item_count];
    Map_keys(m, mkeys);
    for (int i = 0; i < m->item_count; i++) {
        for (int k = 0; k < n; k++) {
            if (strcmp(mkeys[i], keys[k]) == 0)
                goto Found;
        }
        assert(0);
    Found:;
    }
	Map_close(m);
}

int main() {
	test_collitions();
    new_val();
    test_prehash();

    printf("OK %s\n", __FILE__);
}
