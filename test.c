#include <stdio.h>
#include <stdlib.h>
#define TYPE void*
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

unsigned long hash(char* str);

void new_val() {
	Map* map = new_map();
	put(map, "key", "this is a value");
	printf("%s\n", (char*)get(map, "key"));

	put(map, "key", "updated value stored in the map");
	printf("%s\n", (char*)get(map, "key"));
	close_map(map);
}

void test_collitions() {
	Map* m = new_map();
	int n = 16;
	char* keys[16] = {"7GKu#m'?Hn", "ya?F9JS6HW", "xoO96tWdi-", "c2dvl0gUvm",
					 "AeZB4t", "jB75,f", "L7YD6T",
				 	 "EY?nDc8", "a4ZITOo", "TUibNyo", "rZJnN5C", "7i0o4KL", "di-SRs8", "?StNEiH", "1'7xPkH", "#vKWM11"};

	int x[n];
	for (int i = 0; i < n; i++) {
		x[i] = i;
		put(m, keys[i], &x[i]);
	}
	// printf("%i\n", *(int*)get(m, "di-SRs8"));
	// printf("%i\n", *(int*)get(m, "AeZB4t"));

	print_map(m);
	resize_map(&m, 3);

	// printf("%lu\n", m->__size);
	print_map(m);
	// printf("%i\n", *(int*)get(m, "7i0o4KL"));

	close_map(m);
}

int main() {
	test_collitions();
	printf("%s\n", __FILE__);
}
