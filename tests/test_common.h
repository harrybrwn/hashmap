#ifndef HASHMAP_TEST_COMMON_H
#define HASHMAP_TEST_COMMON_H

#include <sys/types.h>

typedef void (*testfunc)(void);

extern char* randstring(size_t);
extern char** rand_keys(size_t n);
extern void free_string_arr(char** arr, int len);
extern double benchmark(testfunc);
extern double nbenchmark(testfunc, int);
extern void Benchmark(const char*, testfunc);
void Benchmarkf(const char*, testfunc, const char*);
extern void nBenchmark(const char*, testfunc, int);

#endif