#define AUTOTEST
#include "tests/utest.h"

#include "hashmap.h"
#include "map_iter.c"

struct node* newnode(hash_t h)
{
    struct node* n = malloc(sizeof(struct node));
    n->hash = h;
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
    struct node* res = iter_next(it);
    assert(res != NULL);
    eq(res->key, "five");

    res = iter_next(it);
    assert(res != NULL);
    eq(res->key, "eleven");

    res = iter_next(it);
    assert(res != NULL);
    eq(res->key, "seven");

    res = iter_next(it);
    assert(res == NULL);
    int i;
    for (i = 0; i < 10; i++)
    {
        res = iter_next(it);
        assert(res == NULL);
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
    struct stack_node* stack = NULL;

    stack = create_stack_node();

    push(&stack, n1);
    eq(stack->node, n1);
    push(&stack, n2);
    eq(stack->node, n2);

    node popped = pop(&stack);
    eq(popped->hash, n2->hash);
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
    struct stack_node* stack;

    stack = create_stack_node();
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
    struct stack_node* stack = create_stack_node();

    push_tree(&stack, n);

    struct node* p = pop(&stack);
    eq(p, n);
    eq(p->hash, n->hash);

    p = pop(&stack);
    eq(p, n->right);
    eq(p->hash, n->right->hash);

    p = pop(&stack);
    eq(p, n->left);
    eq(p->hash, n->left->hash);

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
    {
        map_put(m, keys[i], keys[i]);
    }

    struct node* n;
    MapIterator* it = map_iter(m);
    for (i = 0; !iter_done(it); i++)
    {
        n = iter_next(it);
        assert(map_get(m, n->key) != NULL);
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
    {
        map_put(m, keys[i], keys[i]);
    }

    struct node* node;
    i = 0;
    MapIterator* it = map_iter(m);
    eq(it->pos, 0UL);
    while (!iter_done(it))
    {
        node = iter_next(it);
        assert(node != NULL);
        assert(str_arr_contains(keys, n, node->key));
        i++;
    }
    eq(i, n);

    map_free(m);
    free(it);
    free_string_arr(keys, n);
}