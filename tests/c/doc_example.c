#include <stdbool.h>
#include <stdio.h>

#define MapValue bool
#include <hashmap.h>

static Map* map = NULL;

void init()
{
    map = new_map();
    map_put(map, "working", 1);
}

int main()
{
    init();

    MapValue working = map_get(map, "working");

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
