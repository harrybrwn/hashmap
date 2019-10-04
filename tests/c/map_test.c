#if !defined(HASHMAP_C_INC)
#if !defined(HASHMAP_MAIN_TEST)
#define AUTOTEST
#endif
#define HASHMAP_C_INC
#include "hashmap.c"
#endif
#include "tests/utest.h"
// test.h must come after utest.h
#include "tests/test.h"

TEST(map_test)
{
    Map* map = new_map();

    char* val = "this is a value";
    map_put(map, "key", val);
    assert_eq(0, strcmp(val, (char*)map_get(map, "key")));
    eq(val, (char*)map_get(map, "key"));
    val = "updated value stored in the map";
    map_put(map, "key", val);
    assert_eq(0, strcmp(val, (char*)map_get(map, "key")));
    eq(val, (char*)map_get(map, "key"));
    map_close(map);
}

TEST(map_delete_test)
{
    Map* m = new_map();
    assert(m->item_count == 0);
    int val = 9001;
    map_put(m, "key1", &val);
    assert(m->item_count == 1);
    assert(*(int*)map_get(m, "key1") == val);

    map_delete(m, "key1");
    assert_eq(0UL, m->item_count);
    int* res = (int*)map_get(m, "key1");
    assert(res == NULL);
    map_delete(m, "invalid_key");
    assert(m->item_count == 0); // for that weird line in map_delete
    map_close(m);
}

TEST(map_resize_test)
{
    Map* m = new_map();
    const size_t n = 28;
    char** keys = rand_keys(n);

    int x[n];
    size_t i;
    for (i = 0; i < n; i++)
    {
        x[i] = i;
        map_put(m, keys[i], &x[i]);
    }
    map_resize(&m, n + 1);
    assert_eq((size_t)(n + 1), m->__size);

    map_resize(&m, 3);
    for (i = 0; i < n; i++)
        assert_eq(((int)i), *(int*)map_get(m, keys[i]));
    assert_eq(3UL, m->__size);

    eq(5, *(int*)map_get(m, keys[5]));
    map_delete_free_key(m, keys[5]);
    eq(NULL, map_get(m, keys[5]));

    map_close_free_keys(m);
    free(keys);
}

TEST(map_keys_test)
{
    Map* m = new_map();
    const size_t n = 39;
    char** keys = rand_keys(n);
    int data[n];
    size_t i;
    for (i = 0; i < n; i++)
    {
        data[i] = i;
        map_put(m, keys[i], &data[i]);
    }
    assert(n == m->item_count);
    assert_eq(39UL, n);
    assert_eq(39UL, m->item_count);

    {
        char* mapkeys[39];
        map_keys(m, mapkeys);
        eq(n, m->item_count);
        arr_unordered_eq_s(mapkeys, keys, n);
    }

    map_close(m);
    free_string_arr(keys, n);

    m = create_map(71);
    char** mapkeys = malloc(sizeof(char*) * 71);
    for (i = 0; i < 71; i++)
        mapkeys[i] = NULL;
    for (i = 0; i < 71; i++)
        assert(mapkeys[i] == NULL);
    map_keys(m, mapkeys);
    for (i = 0; i < 71; i++)
        assert(mapkeys[i] == NULL);

    keys = rand_keys(71);
    for (i = 0; i < 71; i++)
        map_put(m, keys[i], keys[i]);

    map_keys(m, mapkeys);
    assert_eq(71UL, m->item_count);
    arr_unordered_eq_s(mapkeys, keys, 71);

    free(mapkeys);
    free_string_arr(keys, 71);
    map_close(m);
}

TEST(map_keys2)
{
    Map* m = create_map(11);
    char* ks[] = { "one", "two", "three", "four", "five" };
    int i, data[5];
    for (i = 0; i < 5; i++)
    {
        data[i] = i;
        map_put(m, ks[i], &data[i]);
    }

    assert_eq(5UL, m->item_count);
    char* keys[5];
    map_keys(m, keys);

    arr_unordered_eq_s(ks, keys, 5);

    map_close(m);
}

TEST(map_clear)
{
    Map* m = new_map();
    map_clear(m); /* shouldn't seg-fault when called on emty map */

    int i;
    int data[6];
    char* keys[] = { "one", "two", "three", "four", "five", "six" };

    for (i = 0; i < 6; i++)
    {
        data[i] = i;
        map_put(m, keys[i], &data[i]);
    }
    map_clear(m);
    size_t k;
    for (k = 0; k < m->__size; k++)
    {
        assert_eq(m->__data[k], NULL);
    }
    map_clear(m); /* shouldn't seg-fault when called on emty map */
    map_close(m);
}

TEST(map_putn_test)
{
    struct map_putn_struct
    {
        int a;
        char b;
        float c;
        struct map_putn_struct* d;
    };

    Map* m = new_map();
    struct map_putn_struct key = { 1, 2, 3.3, NULL };
    eq(sizeof(key), sizeof(struct map_putn_struct));
    map_putn(m, &key, sizeof(key), "hello?");

    char* val = map_getn(m, &key, sizeof(key));
    eq(val, "hello?");
}

TEST(prehash_len_test)
{
    hash_t a = prehash("hello");
    hash_t b = prehash_len("hello", 5);
    eq(a, b);
    hash_t c = prehash_len("hello", 6);
    not_eq (a, c);
}