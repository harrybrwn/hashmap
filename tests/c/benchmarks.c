#include <assert.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MapValue int
#include "tests/test_common.h"
// #include <hashmap.h>
#include "hashmap.c"

hash_t prehash(char*);
hash_t djb2(char* str);
hash_t sdbm(char* str);
hash_t fnv_1(char* str);
hash_t rshash(char* str);

// #define N_KEYS 2200000
#define N_KEYS 2000000
// #define N_KEYS 1000000
// #define N_KEYS 10000

static Map* map;
static char** mapkeys;

void init_globals()
{
    srand(time(0));
    map = create_map(N_KEYS + 293);
    mapkeys = rand_keys(N_KEYS);
}

void teardown_globals()
{
    size_t i;
    for (i = 0; i < map->__size; i++)
        assert(map->__data[i] == NULL);
    assert(map->item_count == 0);

    map_close(map);
    free_string_arr(mapkeys, N_KEYS);
}

void put_benchmark()
{
    int i;
    for (i = 0; i < N_KEYS; i++)
        map_put(map, mapkeys[i], 33);
}

void get_benchmark()
{
    int i;
    for (i = 0; i < N_KEYS; i++)
    {
        assert(33 == map_get(map, mapkeys[i]));
    }
}

void delete_benchmark()
{
    int i;
    for (i = 0; i < N_KEYS; i++)
        map_delete(map, mapkeys[i]);
}

void djb2_benchmark()
{
    int i;
    for (i = 0; i < N_KEYS; i++)
        djb2(mapkeys[i]);
}

void fnv_1_benchmark()
{
    int i;
    for (i = 0; i < N_KEYS; i++)
        fnv_1(mapkeys[i]);
}

void sdbm_benchmark()
{
    int i;
    for (i = 0; i < N_KEYS; i++)
        sdbm(mapkeys[i]);
}

void prehash_benchmark()
{
    int i;
    for (i = 0; i < N_KEYS; i++)
        prehash(mapkeys[i]);
}

void putget_benchmark()
{
    Map* m = create_map(N_KEYS + 293);
    assert(m != NULL);

    srand(time(0));
    char** keys = rand_keys(N_KEYS);
    MapValue data[N_KEYS];
    int i;

    for (i = 0; i < N_KEYS; i++)
    {
        data[i] = i + 1;
        map_put(m, keys[i], data[i]);
    }

    for (i = 0; i < N_KEYS; i++)
        assert(data[i] == map_get(m, keys[i]));

    assert(m->item_count == N_KEYS);
    map_close(m);
    free_string_arr(keys, N_KEYS);
}

void putdelete_benchmark()
{
    Map* m = create_map(N_KEYS + 293);

    srand(time(0));
    char** keys = rand_keys(N_KEYS);
    MapValue data[N_KEYS];

    size_t i;
    for (i = 0; i < N_KEYS; i++)
    {
        data[i] = i + 1;
        map_put(m, keys[i], data[i]);
    }

    for (i = 0; i < N_KEYS; i++)
        map_delete(m, keys[i]);
    for (i = 0; i < m->__size; i++)
        assert(m->__data[i] == NULL);

    assert(m->item_count == 0);
    map_close(m);
    free_string_arr(keys, N_KEYS);
}

int main()
{
    setlocale(LC_NUMERIC, "");
    printf("Start Benchmarks with %'d items\n", N_KEYS);
    init_globals();

    Benchmark("put", put_benchmark);
    Benchmark("get", get_benchmark);
    Benchmark("delete", delete_benchmark);
    printf("\n");

    Benchmark("djb2", djb2_benchmark);
    Benchmark("fnv_1", fnv_1_benchmark);
    Benchmark("sdbm", sdbm_benchmark);
    Benchmark("prehash", prehash_benchmark);

    // printf("\n");
    // Benchmark("put/get", putget_benchmark);
    // Benchmark("put/delete", putdelete_benchmark);

    teardown_globals();
    printf("End Benchmarks\n");
    return 0;
}