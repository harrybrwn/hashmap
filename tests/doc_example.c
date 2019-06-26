#include <stdio.h>
#include <stdlib.h>

#define MapValue unsigned char
#include "hashmap.h"

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
		exit(1);
	}
}
