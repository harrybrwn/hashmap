/**
 * This test file is just a playground for using the api in a maner closer
 * to the way that a user might use it.
 */

#include <hashmap.h>
#include <map_iter.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* keys[] = { "one", "two", "three", "apple", "orange" };

#define N sizeof(keys) / sizeof(keys[0])

struct address
{
    int num;
    char* street;
    char* city;
    char* state;
};

#define INIT_ADDR(addr, N, S, C, ST)                                                              \
    memset(&addr, 0, sizeof(struct address));                                                     \
    addr.num = N;                                                                                 \
    addr.street = S;                                                                              \
    addr.city = C;                                                                                \
    addr.state = ST;

#define setup __attribute__((constructor))
#define teardown __attribute__((destructor))

static Map* m;

void add_jimbo(void)
{
    char* name = malloc(6);
    strncpy(name, "jimbo", 6);

    struct address jimbos_house;
    INIT_ADDR(jimbos_house, 1234, "Maple St.", "San Fransisco", "CA");
    map_key_put(m, (struct key){ &jimbos_house, sizeof(struct address) }, name);
}

setup void start(void)
{
    m = create_map(127);
    add_jimbo();
}

teardown void end(void)
{
    map_free(m);
}

int main(void)
{
    int vals[N];
    size_t i;
    for (i = 0; i < N; i++)
    {
        vals[i] = i;
        map_put(m, keys[i], &vals[i]);
    }

    for (i = 0; i < N; i++)
    {
        if (vals[i] != (*(int*)map_get(m, keys[i])))
            printf("wrong value!\n");
    }

    struct address house;
    INIT_ADDR(house, 1234, "Maple St.", "San Fransisco", "CA");
    struct key addr_key = { &house, sizeof(struct address) };

    char* name = map_key_get(m, addr_key);
    if (name == NULL)
    {
        printf("could not find owners name!!!!\n");
        return 1;
    }
    else
    {
        printf("the owner's name is %s\n", name);
    }

    MapIterator* it;

    tuple_t t;
    it = map_iter(m);
    while (!iter_done(it))
    {
        t = iter_next(it);
        // printf("%lu ", n->hash);
        printf("%s ", t.key);
        printf("\n");
    }
    free(it);
    free(name);
}