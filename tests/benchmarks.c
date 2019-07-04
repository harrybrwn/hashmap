#include <stdio.h>
#include <assert.h>
#include <time.h>

#include <hashmap.h>
#include "tests/test_common.h"

typedef void (*testfunc)(void);

int usleep(__useconds_t usec);

void millipause(double sec) {
	int milliseconds = sec * 1000;
    usleep(milliseconds * 1000);
}

void test()
{
	millipause(0.001);
}

void rand_keys_bench()
{
	int n = 100000;
	char** keys = rand_keys(n);
	free_string_arr(keys, n);
}

#define N_KEYS 2000000
// #define N_KEYS 10000

void putget_benchmark()
{
	Map* m = New_Map();	
	Map_resize(&m, N_KEYS);
	assert(m != NULL);

	char** keys = rand_keys(N_KEYS);
	int data[N_KEYS];

	for (int i = 0; i < N_KEYS; i++)
	{
		data[i] = 20;
		Map_put(m, keys[i], &data[i]);
	}

	for (int i = 0; i < N_KEYS; i++)
		assert(data[i] == *(int*)Map_get(m, keys[i]));

	assert(m->item_count == N_KEYS);
	Map_close(m);
	free_string_arr(keys, N_KEYS);
}

void putdelete_benchmark()
{
	Map* m = New_Map();
	Map_resize(&m, N_KEYS);

	char** keys = rand_keys(N_KEYS);
	int data[N_KEYS];

	for (int i = 0; i < N_KEYS; i++)
	{
		data[i] = i + 1;
		Map_put(m, keys[i], &data[i]);
	}
	for (int i = 0; i < N_KEYS; i++)
		Map_delete(m, keys[i]);
	for (size_t i = 0; i < m->__size; i++)
		assert(m->__data[i] == NULL);
	assert(m->item_count == 0);
	Map_close(m);
	free_string_arr(keys, N_KEYS);
}

int main()
{
	printf("\nStart Benchmarks\n");

	Benchmark("main_test", test);
	Benchmark("put/get", putget_benchmark);
	Benchmark("put/delete", putdelete_benchmark);
	
	printf("End Benchmarks\n");
}