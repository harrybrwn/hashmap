#ifndef HASHMAP_TEST_COMMON_H
#define HASHMAP_TEST_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/types.h>

typedef void (*testfunc)(void);

extern char* randstring(size_t);
extern char** rand_keys(size_t n);
extern void free_string_arr(char** arr, int len);
extern double benchmark(testfunc);
extern double nbenchmark(testfunc, int);
extern void Benchmark(const char*, testfunc);
extern void AverageBenchmark(const char*, testfunc, size_t);
void Benchmarkf(const char*, testfunc, const char*);
extern void nBenchmark(const char*, testfunc, size_t);
extern int Run(testfunc*, int);

#ifdef __cplusplus
}
#endif

#endif /* HASHMAP_TEST_COMMON_H */