#include <stdio.h>
#include <stdbool.h>

#define MapValue bool
#include <hashmap.h>

static Map *map = NULL;

void init()
{
	map = New_Map();
	Map_put(map, "working", 1);
}

int main()
{
	init();

	MapValue working = Map_get(map, "working");

	if (working)
	{
		printf("everything is working great :) \n");
	}
	else
	{
		printf("map is not working\n");
		return 1;
	}
	return 0;
}
