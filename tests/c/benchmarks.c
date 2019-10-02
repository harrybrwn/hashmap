#include <assert.h>
#include <locale.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MapValue int
#include "hashmap.c"
#undef FNV_PRIME
#undef FNV_PRIME_MUL
#undef FNV_OFFSET
#include "hash.c"
#include "tests/test.h"

hash_t prehash(char*);
hash_t djb2(char* str);
hash_t sdbm(char* str);
hash_t fnv_1(char* str);
hash_t rshash(char* str);

#define KEY_LEN 10
#define N_KEYS   3000017UL
#define MAP_SIZE 4015567UL
#define LOAD_FACTOR ((double)N_KEYS / (double)MAP_SIZE)

static Map* map;
static char* mapkeys[N_KEYS];

void init_globals(void)
{
    srand(time(0));
    map = create_map(MAP_SIZE);
    size_t i;
    for (i = 0; i < N_KEYS; i++)
    {
        mapkeys[i] = randstring(KEY_LEN);
    }
    printf("setting up map...\n");
    printf("load factor: %f\n", LOAD_FACTOR);
    printf("map uses %'lu bytes.\n",
        sizeof(Map) +
        (sizeof(struct node*) * N_KEYS));
}

void teardown_globals(void)
{
    size_t i;
    for (i = 0; i < MAP_SIZE; i++)
        assert(map->__data[i] == NULL);
    assert(map->item_count == 0);

    map_close(map);
    // free_string_arr(mapkeys, N_KEYS);
    for (i = 0; i < N_KEYS; i++)
        free(mapkeys[i]);
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
BENCH(map_stats, ({
    size_t i;
    int max = 0;
    long empty = 0;
    for (i = 0; i < MAP_SIZE; i++) {
        if (map->__data[i] != NULL) {
            if (map->__data[i]->height > max) {
                max = map->__data[i]->height;
            }
        } else {
            empty++;
        }
    }
    printf("    max node height: %d\n", max);
    printf("    empty nodes:     %'ld\n", empty);
    printf("    percend used:    %.2f%%\n", ((double)(MAP_SIZE - empty) / (double)MAP_SIZE) * 100);
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
BENCH(fnv_1a, ({
    size_t i;
    for (i = 0; i < N_KEYS; i++)
        fnv_1a(mapkeys[i]);
}))

// clang-format off
BENCHMARK_SUITE(
    ADD_BENCH(djb2),
    ADD_BENCH(sdbm),
    ADD_BENCH(fnv_1),
    ADD_BENCH(fnv_1a),
    ADD_BENCH(prehash),
    ADD_BENCH(put),
    ADD_BENCH(map_stats),
    ADD_BENCH(get),
    ADD_BENCH(delete)
)

int main()
{
    setlocale(LC_NUMERIC, "");
    init_globals();

    printf("\nStart Benchmarks with %'ld items\n", N_KEYS);

    RunBenchmarks();
    teardown_globals();

    printf("End Benchmarks\n");
    return 0;
}