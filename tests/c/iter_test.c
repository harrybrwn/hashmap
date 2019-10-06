#define AUTOTEST
#include "tests/utest.h"

#include "hashmap.c"

struct node* newnode(hash_t h)
{
    struct node* n = malloc(sizeof(struct node));
    n->_hash_val = h;
    n->height = 0;
    n->left = NULL;
    n->right = NULL;
    return n;
}

TEST(small_iter)
{
    Map* m = create_map(37);
    int primes[] = { 1, 2, 3, 5, 7, 11, 13 };
    map_put(m, "five", &primes[3]);
    map_put(m, "seven", &primes[4]);
    map_put(m, "eleven", &primes[5]);

    MapIterator* it = map_iter(m);
    tuple_t res = iter_next(it);
    assert(res.key != NULL);
    assert(res.value != NULL);
    eq(res.key, "five");

    res = iter_next(it);
    assert(res.key != NULL);
    assert(res.value != NULL);
    eq(res.key, "eleven");

    res = iter_next(it);
    assert(res.key != NULL);
    assert(res.value != NULL);
    eq(res.key, "seven");

    res = iter_next(it);
    assert(res.key == NULL);
    assert(res.value == NULL);
    int i;
    for (i = 0; i < 10; i++)
    {
        res = iter_next(it);
        assert(res.key == NULL);
        assert(res.value == NULL);
        assert(iter_done(it));
    }

    free(it);
    map_free(m);
}

TEST(push)
{
    typedef struct node* node;
    node n1 = newnode(10);
    node n2 = newnode(11);
    struct node_stack* stack = NULL;

    stack = create_node_stack();

    push(&stack, n1);
    eq(stack->node, n1);
    push(&stack, n2);
    eq(stack->node, n2);

    node popped = pop(&stack);
    eq(popped, n2);
    node popped2 = pop(&stack);
    eq(popped2, n1);
    assert(stack == NULL);

    free(n1);
    free(n2);
}

TEST(free_stack_valgrind)
{
    int i;
    const int len = 20;
    struct node* nodes[len];
    struct node_stack* stack;

    stack = create_node_stack();
    for (i = 0; i < len; i++)
    {
        nodes[i] = newnode(i);
        push(&stack, nodes[i]);
    }

    free_stack(stack);

    for (i = 0; i < len; i++)
        free(nodes[i]);
}

TEST(push_tree_to_stack)
{
    struct node* n = newnode(10);
    n->left = newnode(5);
    n->right = newnode(15);
    struct node_stack* stack = create_node_stack();

    push_tree(&stack, n);

    struct node* p = pop(&stack);
    eq(p, n);
    eq(p->_hash_val, n->_hash_val);

    p = pop(&stack);
    eq(p, n->right);
    eq(p->_hash_val, n->right->_hash_val);

    p = pop(&stack);
    eq(p, n->left);
    eq(p->_hash_val, n->left->_hash_val);

    free(n->left);
    free(n->right);
    free(n);
}

#include "tests/test.h"

TEST(iterator)
{
    Map* m = create_map(211);
    char** keys = rand_keys(200);
    int i;
    for (i = 0; i < 200; i++)
        map_put(m, keys[i], keys[i]);

    // struct node* n;
    tuple_t n;
    MapIterator* it = map_iter(m);
    eq(it->counter, 200L);
    for (i = 0; !iter_done(it); i++)
    {
        n = iter_next(it);
        assert(map_get(m, n.key) != NULL);
    }
    eq(i, 200);

    free(it);
    map_close_free_keys(m);
    free(keys);
}

TEST(map_size_of_1)
{
    int n = 10, i;
    char** keys = rand_keys(n);
    Map* m = create_map(1);

    for (i = 0; i < n; i++)
        map_put(m, keys[i], keys[i]);

    i = 0;
    MapIterator* it = map_iter(m);
    eq(it->pos, 0UL);
    tuple_t pair;
    while (iter_hasnext(it))
    {
        pair = iter_next(it);
        assert(pair.key != NULL);
        assert(pair.value != NULL);
        assert(str_arr_contains(keys, n, pair.key));
        i++;
    }
    eq(i, n);

    map_free(m);
    free(it);
    free_string_arr(keys, n);
}

TEST(auto_done_test)
{
    Map* m = create_map(23);
    char* key = malloc(sizeof("key"));
    int* val = malloc(sizeof(int));
    char* key2 = malloc(sizeof("huh"));
    int* val2 = malloc(sizeof(int));

    strncpy(key, "key", 4);
    *val = 11;
    strncpy(key2, "huh", 4);
    *val2 = 69;

    map_put(m, key, val);
    map_put(m, key2, val2);

    MapIterator* it = map_iter(m);
    while (!iter_auto_done(&it))
    {
        tuple_t n = iter_next(it);
        assert(n.key != NULL);
        assert(n.value != NULL);
    }

    assert(it == NULL);
    map_close(m);
    free(key);
    free(val);
    free(key2);
    free(val2);
}

TEST(destroy_iter_test)
{
    Map* m = create_map(111);
    char** keys = rand_keys(100);
    int i;
    for (i = 0; i < 100; i++)
        map_put(m, keys[i], keys[i]);

    MapIterator* it = map_iter(m);
    for (i = 0; i < 25 && iter_hasnext(it); i++)
    {
        tuple_t tuple = iter_next(it);
        assert(tuple.key != NULL);
        assert(tuple.value != NULL);
        assert(str_arr_contains(keys, 100, tuple.key));
        eq(tuple.key, (char*)tuple.value);
    }

    eq(it->counter, 75L);
    assert(it->root != NULL);
    destroy_iter(it);
    free_string_arr(keys, 100);
    map_free(m);
}