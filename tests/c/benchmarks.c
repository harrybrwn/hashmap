#include <assert.h>
#include <locale.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MapValue int
#include "hashmap.c"
#include "tests/test.h"

hash_t prehash(char*);
hash_t djb2(char* str);
hash_t sdbm(char* str);
hash_t fnv_1(char* str);
hash_t rshash(char* str);

// #define N_KEYS 100000000UL
// #define N_KEYS 5000000UL
#define N_KEYS 3000000UL
// #define N_KEYS 2500000
// #define N_KEYS 2000000
// #define N_KEYS 1500000
// #define N_KEYS 1000000

static Map* map;
static char** mapkeys;

void init_globals(void)
{
    srand(time(0));
    map = create_map(N_KEYS + 293);
    mapkeys = rand_keys(N_KEYS);
}

void teardown_globals(void)
{
    size_t i;
    for (i = 0; i < map->__size; i++)
        assert(map->__data[i] == NULL);
    assert(map->item_count == 0);

    map_close(map);
    free_string_arr(mapkeys, N_KEYS);
}

// clang-format off
BENCH(put, ({
    size_t i;
    for (i = 0; i < N_KEYS; i++)
        map_put(map, mapkeys[i], 33);
}))
BENCH(get, ({
    size_t i;
    for (i = 0; i < N_KEYS; i++)
    {
        assert(33 == map_get(map, mapkeys[i]));
    }
}))
BENCH(delete, ({
    size_t i;
    for (i = 0; i < N_KEYS; i++)
        map_delete(map, mapkeys[i]);
}))
BENCH(djb2, ({
    size_t i;
    for (i = 0; i < N_KEYS; i++)
        djb2(mapkeys[i]);
}))
BENCH(fnv_1, ({
    size_t i;
    for (i = 0; i < N_KEYS; i++)
        fnv_1(mapkeys[i]);
}))
BENCH(sdbm, ({
    size_t i;
    for (i = 0; i < N_KEYS; i++)
        sdbm(mapkeys[i]);
}))
BENCH(prehash, ({
    size_t i;
    for (i = 0; i < N_KEYS; i++)
        prehash(mapkeys[i]);
}))

// clang-format off
BENCHMARK_SUITE(
    ADD_BENCH(put),
    ADD_BENCH(get),
    ADD_BENCH(delete),
    ADD_BENCH(prehash),
    ADD_BENCH(djb2),
    ADD_BENCH(fnv_1),
    ADD_BENCH(sdbm)
)

int main()
{
    setlocale(LC_NUMERIC, "");
    printf("Start Benchmarks with %'ld items\n", N_KEYS);
    
    init_globals();
    RunBenchmarks();
    teardown_globals();

    printf("End Benchmarks\n");
    return 0;
}